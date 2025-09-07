/*Commands
1 + {Gcode} = executes that Gcode
2 = Saves the current x,y,z coordinates to the offset and considers the current position as the origin
3 = writes in the console the x,y,z coordinates, the offsets and some more stuff
4 = lowers the z axys until it finds contact
5 = starts the "route.cnc" file
50 + file name = starts the given file (if present in the SD card)
51 = starts the "mask.cnc" file, does not disable plane probing and does not probe, you need the previous probing (so don't shut it down after route.cnc) <- could be improved
52 = starts  the "drill.cnc" file, disables plane probing
53 = starts  the "cut.cnc" file, disables plane probing
6 = tries to initiate the SD card (also done at start) <- to do
7 = prints SD card content 
8 = homes the z axys alone
9 = toggles plane probing 
10 = toggles debug mode 
11 = toggles enabled and disabled holding torque



Change relative system
21 save solder mask relative system
22 save drill relative system
23  save cutting relative system
211 load solder mask relative system
221 load drill relative system 
231 load cutting relative system 
*/

#include <SPI.h>
#include <SD.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#define PROBING 3 // 1= taks the lowest point as estimate, 2 = means the 5 points, 3 = mean and filter outliers

void update_limit();
int find_string(char, char[]);
void readline(char[]);
void home_x();
void home_y();
void home_z();
void home_z2();
void file_read(int);
void find_boundaries();
void probe_corners();
int which_plane(long[]);

File myFile;
AccelStepper stepper_x(AccelStepper::HALF4WIRE, 8, 6, 7, 5);
AccelStepper stepper_y(AccelStepper::HALF4WIRE, A0, A2, A1, A3);
AccelStepper stepper_z(AccelStepper::HALF4WIRE, 3, 4, 2, 9);  
MultiStepper steppers;
const int steps_per_mm_xy = 512;
const int steps_per_mm_z = 512;
bool limit_x,limit_y,limit_z;
bool ready = false;
bool is_probed = false;
bool plane_probing = false;
bool enabled = true; 
bool alt = false;
long offset_x = 0; long offset_y = 0; long offset_z = 0;
float x_boundary = 0; float y_boundary = 0;
const float max_x = 120; 
const float max_y = 92;
const float min_z = -21.5;
int strikes = 0; 
bool x_backlash = false; bool y_backlash = false; bool z_backlash = false; //indicates the last rotation: to the switch = false, away from the switch is true (away is negative ->true, towards is positive ->false)
const long x_m_backlash = 0; //i segni sono un po' strani, il plane probing tecnicamente in x e y non lo usa, non è così grave credo, è un po' intricato da fixxare
const long y_m_backlash = 0; //-8
const long z_m_backlash = 0; //-8


long offset_x_mask = 0; long offset_y_mask = 0; long offset_z_mask = 0; 
long offset_x_drill = 0; long offset_y_drill = 0; long offset_z_drill = 0;
long offset_x_edge = 0; long offset_y_edge = 0; long offset_z_edge = 0;

long z_map[5];
float plane_coefficients1[3], plane_coefficients2[3],plane_coefficients3[3], plane_coefficients4[3];
const int x_y_speed0 = 800; //veloce era 500
int x_y_speed1 = 800; // 250
const int z_speed0 = 500; //500
const int z_speed1 = 140; //140

void setup() {
  pinMode(A4, OUTPUT); //relé
  pinMode(A5, OUTPUT); //limit z2 HIGH when active, this means that the plane acts as a switch only during the probing 
  //A7 is the analog pin that reads the z2, I use two cables to avoid junk on the pin while cutting
  //A6 is the analog pin for the limit switches
  digitalWrite(A5,LOW);
  digitalWrite(A4, LOW);
  Serial.begin(9600);
  Serial.println(F("Hi"));
  stepper_x.setMaxSpeed(x_y_speed0);
  stepper_x.setAcceleration(100);
  stepper_x.setSpeed(x_y_speed0);
  stepper_y.setMaxSpeed(x_y_speed0);
  stepper_y.setAcceleration(100);
  stepper_y.setSpeed(x_y_speed0);
  stepper_z.setMaxSpeed(z_speed0);
  stepper_z.setAcceleration(100);
  stepper_z.setSpeed(z_speed0);
  steppers.addStepper(stepper_x);
  steppers.addStepper(stepper_y);
  steppers.addStepper(stepper_z);

  update_limit();
  if (!SD.begin(10)) { //tries to connect to the SDcard
    Serial.println(F("SD f")); 
  }
}

void loop() {
  if (Serial.available()) {
    int a = Serial.parseInt();
    if (a == 1) {
      String command = Serial.readString(); //I should really just ditch the string but I'm lazy
      command.trim();
      char command_arr[40];
      command.toCharArray(command_arr, 40);
      readline(command_arr);
    }else

    if (a == 2) {
      offset_x = offset_x -(stepper_x.currentPosition());
      offset_y = offset_y -(stepper_y.currentPosition());
      offset_z = offset_z + (stepper_z.currentPosition());
      stepper_x.setCurrentPosition(0);
      stepper_y.setCurrentPosition(0);
      stepper_z.setCurrentPosition(0);
    }else
    
    if (a == 21) {
      offset_x_mask = offset_x - (stepper_x.currentPosition());
      offset_y_mask = offset_y - (stepper_y.currentPosition());
      offset_z_mask = offset_z + (stepper_z.currentPosition());
      Serial.print(F("y"));
    }else
    if (a == 22) {
      offset_x_drill = offset_x - (stepper_x.currentPosition());
      offset_y_drill = offset_y - (stepper_y.currentPosition());
      offset_z_drill = offset_z + (stepper_z.currentPosition());
      Serial.print(F("y"));
    } else
    if (a == 23) {
      offset_x_edge = offset_x - (stepper_x.currentPosition());
      offset_y_edge = offset_y - (stepper_y.currentPosition());
      offset_z_edge = offset_z + (stepper_z.currentPosition());
      Serial.print(F("y"));
    }else

    if (a == 211) {
      long real_pos_x = offset_x - stepper_x.currentPosition();
      stepper_x.setCurrentPosition(-real_pos_x + offset_x_mask); //current position è negativo quindi deve riflettere

      long real_pos_y = offset_y - stepper_y.currentPosition();
      stepper_y.setCurrentPosition(-real_pos_y + offset_y_mask);

      long real_pos_z = offset_z + stepper_z.currentPosition();
      stepper_z.setCurrentPosition(real_pos_z - offset_z_mask);
      offset_x = offset_x_mask;
      offset_y = offset_y_mask;
      offset_z = offset_z_mask;

    }else
    if (a == 221) {
      long real_pos_x = offset_x - stepper_x.currentPosition();
      stepper_x.setCurrentPosition(-real_pos_x + offset_x_drill);

      long real_pos_y = offset_y - stepper_y.currentPosition();
      stepper_y.setCurrentPosition(-real_pos_y + offset_y_drill);

      long real_pos_z = offset_z + stepper_z.currentPosition();
      stepper_z.setCurrentPosition(real_pos_z - offset_z_drill);

      offset_x = offset_x_drill;
      offset_y = offset_y_drill;
      offset_z = offset_z_drill;
    }else
    if (a == 231) {
      long real_pos_x = offset_x - stepper_x.currentPosition();
      stepper_x.setCurrentPosition(-real_pos_x + offset_x_edge);

      long real_pos_y = offset_y - stepper_y.currentPosition();
      stepper_y.setCurrentPosition(-real_pos_y + offset_y_edge);

      long real_pos_z = offset_z + stepper_z.currentPosition();
      stepper_z.setCurrentPosition(real_pos_z - offset_z_edge);

      offset_x = offset_x_edge;
      offset_y = offset_y_edge;
      offset_z = offset_z_edge;
    }else

    if (a == 3) {
      float x = (float(stepper_x.currentPosition())/steps_per_mm_xy);  
      float y = (float(stepper_y.currentPosition())/steps_per_mm_xy);
      float z = (float(stepper_z.currentPosition())/steps_per_mm_z);
      Serial.println(F(""));
      Serial.print(F("X:"));
      Serial.println(-x);
      Serial.print(F("Y:"));
      Serial.println(-y);
      Serial.print(F("Z:"));
      Serial.println(z);
      Serial.print(F("tr_X:"));
      Serial.println(-x + float(float(offset_x)/steps_per_mm_xy));
      Serial.print(F("tr_Y:"));
      Serial.println(-y + float(float(offset_y)/steps_per_mm_xy));
      Serial.print(F("tr_Z:"));
      Serial.println(z + float(float(offset_z)/steps_per_mm_z));
      Serial.print(F("stk:"));
      Serial.println(strikes);
    }else
    if (a == 4) {
      if (!enabled) {
        Serial.print(F("\n mtr dis"));
        return;
      }
      home_z2();
    }else
    if (a ==5) {
      if (!enabled) {
        Serial.print(F("\n mtr dis"));
        return;
      }
      alt = false;
      if (plane_probing) {
      find_boundaries();
      probe_corners();
      find_plane(plane_coefficients1, 0);
      find_plane(plane_coefficients2, 1);
      find_plane(plane_coefficients3, 2);
      find_plane(plane_coefficients4, 3);
      }
      file_read("route.cnc");
    } else
    if (a == 50) {
      String path = Serial.readString(); //I should really just ditch the string but I'm lazy
      path.trim();
      char path_arr[16];
      path.toCharArray(path_arr, 16);
      file_read(path_arr);
    }
    if (a == 52) {
      if (!enabled) {
        Serial.print(F("\n mtr dis"));
        return;
      }
      file_read("drill.cnc");
    } else
    if (a == 51) {
      if (!enabled) {
        Serial.print(F("\n mtr dis"));
        return;
      }
      file_read("mask.cnc");
    } else
    if (a == 53) {
      if (!enabled) {
        Serial.print(F("\n mtr dis"));
        return;
      }
      file_read("edge.cnc");
    } else
    if (a == 6) {
      if (!SD.begin(10))
        Serial.println(F("SD f"));
    } else
    if (a == 7) {
      File root = SD.open("/");
      while (File entry = root.openNextFile()) {
        Serial.println(entry.name());
      }
    root.close();
    } else
    if(a == 8) {
      if (!enabled) {
        Serial.print(F("\n mtr dis"));
        return;
      }
      update_limit();
      home_z();
      stepper_z.setCurrentPosition(0);
      offset_z = 0;
    } else
    if(a == 9) {
      if (plane_probing) {
        plane_probing = false;
        Serial.print(F("\nProb:off"));
      }
      else {
        plane_probing = true;
        Serial.print(F("\nProb:on"));
      }
    } else
    if(a == 10) {
      ready = true;
      Serial.print(F("\ndebug"));
    } else
    if (a == 11) {
      if (enabled) {
        stepper_x.disableOutputs();
        stepper_y.disableOutputs();
        stepper_z.disableOutputs();
        enabled = false; 
        Serial.print(F("\n mtr dis"));
      }
      else {
        stepper_x.enableOutputs();
        stepper_y.enableOutputs();
        stepper_z.enableOutputs();
        enabled = true; 
        Serial.print(F("\n mtr en"));
      }
    }
  }
}

void update_limit() {
  int mean = 0;
  limit_x = false;
  limit_y = false;
  limit_z = false;
  for (int i = 0; i<10;i++) {
    mean = mean + analogRead(A6);
  }
  mean = mean / 10;
  if (mean <= 404) { //000
    return;
  } 
  if (405 <= mean && mean < 450) { //100
    limit_z = true;
    return;
  }
  if (450 <= mean && mean <= 500) { //001
    limit_x = true;
    return;
  }
  if (501 <= mean && mean < 580) { //101
    limit_x = true;
    limit_z = true;
    return;
  }
  if (580 <= mean && mean <= 670) { //010
    limit_y = true;
    return;
  } 
  if (670 < mean && mean <= 800) { //110
    limit_y = true;
    limit_z = true;
    return;
  }
  if (801 <= mean && mean < 1015) { //011
    limit_x = true;
    limit_y = true;
    return;
  } 
  if (mean >= 1015) { //111
    limit_x = true;
    limit_y = true;
    limit_z = true;  
    return;
  }
}

void readline(char line[]) {
  if (strlen(line) >= 3) { 
		if ((line[0] != 'G' && line[0] != 'M')) //I have to filter out strings that are not commands
			return;
		if (line[1] < '0' || line[1] > '9' || line[2] < '0' || line[2] > '9') //if it starts with G or M followed by two numbers it's probably an actual command
			return;
	  if (line[0] == 'G' && line[1] == '0' && (line[2] == '1' || line[2] == '0')) {	//G1 and G0 (movement)
      if (enabled == false) {
        Serial.print(F("\n mtr dis"));
        return;
      } 
      if (ready) {
//        bool quick = false; 
//        if (line[2] == '0') {
//          quick = true;
//        }
        float move_x = 0;
        float move_y = 0;
        float move_z = 0; 
        float* plane_coefficients;
        int pos_x,pos_y,pos_z;
        pos_x = find_string('X', line);
        pos_y = find_string('Y', line);
        pos_z = find_string('Z', line);
        if ((pos_x <0) && (pos_y < 0) && (pos_z < 0))
          return;
        int moves = 0;
        long old_z = 0;
        long move_x_y_z[3];
        move_x_y_z[0] = stepper_x.currentPosition();
        move_x_y_z[1] = stepper_y.currentPosition();
        move_x_y_z[2] = stepper_z.currentPosition();
        if (plane_probing) {
          int plane = which_plane(move_x_y_z);
          if (plane == 1) {
            plane_coefficients = plane_coefficients1;
          } else
          if (plane ==2) {
            plane_coefficients = plane_coefficients2;
          } else
          if (plane == 3) {
            plane_coefficients = plane_coefficients3;
          } else
          if (plane == 4) {
            plane_coefficients = plane_coefficients4;
          }
          float x_mm = -((float)move_x_y_z[0]) / steps_per_mm_xy;
          float y_mm = -((float)move_x_y_z[1]) / steps_per_mm_xy;
          float z_comp = x_mm * plane_coefficients[0] + 
               y_mm * plane_coefficients[1] + 
               plane_coefficients[2] + 0.5;
        old_z = z_comp;
        }

        if(pos_x > 0) {
          moves++;
          char* sub_x = &line[pos_x + 1];
          move_x = atof(sub_x);
          if(((offset_x + move_x*steps_per_mm_xy) < max_x*steps_per_mm_xy) && ((offset_x + move_x*steps_per_mm_xy) >= 0)) {
            move_x_y_z[0] = (-move_x)*steps_per_mm_xy;
          } else {
            Serial.println(F("out X"));
            strikes++;
          }
        }

        if(pos_y> 0) {
          moves++;
          char* sub_y = &line[pos_y + 1];
          move_y = atof(sub_y);
          if(((offset_y + move_y*steps_per_mm_xy) < max_y*steps_per_mm_xy) && ((offset_y + move_y*steps_per_mm_xy) >= 0)) {
            move_x_y_z[1] = (-move_y)*steps_per_mm_xy;
          }else {
            strikes++;
            Serial.println(F("out Y"));
          }
        }
        
        if(pos_z> 0) { 
          moves++;
          char* sub_z = &line[pos_z + 1];
          move_z = atof(sub_z);
          if(((offset_z + move_z*steps_per_mm_z) > min_z*steps_per_mm_z) && ((offset_z + move_z*steps_per_mm_z) <= 0)) {
            move_x_y_z[2] = ((move_z)*steps_per_mm_z + float(old_z));
          }else {
            strikes++;
            Serial.println(F("out Z"));
          }
        }
        if((pos_z < 0) && (offset_z != 0) && (is_probed) && (plane_probing)) {
          int plane = which_plane(move_x_y_z);
          if (plane == 1) {
            plane_coefficients = plane_coefficients1;
          } else
          if (plane == 2) {
            plane_coefficients = plane_coefficients2;
          } else
          if (plane == 3) {
            plane_coefficients = plane_coefficients3;
          } else
          if (plane == 4) {
            plane_coefficients = plane_coefficients4;
          }
          moves++;
          long new_z;
          float x_mm = -((float)move_x_y_z[0]) / steps_per_mm_xy;
          float y_mm = -((float)move_x_y_z[1]) / steps_per_mm_xy;
          float z_comp = x_mm * plane_coefficients[0] + 
               y_mm * plane_coefficients[1] + 
               plane_coefficients[2] + 0.5;
        new_z = z_comp;
          move_x_y_z[2] = move_x_y_z[2] - old_z + new_z;  
        }
        int z_s;
        if (line[2] == '0') {
          stepper_x.setSpeed(x_y_speed0);
          stepper_y.setSpeed(x_y_speed0);
          stepper_z.setSpeed(z_speed0);
          stepper_x.setMaxSpeed(x_y_speed0+1);
          stepper_y.setMaxSpeed(x_y_speed0+1);
          stepper_z.setMaxSpeed(z_speed0+1);
          z_s = z_speed0;
        } else {
          stepper_x.setSpeed(x_y_speed1);
          stepper_y.setSpeed(x_y_speed1);
          stepper_z.setSpeed(z_speed1);
          stepper_x.setMaxSpeed(x_y_speed1+1);
          stepper_y.setMaxSpeed(x_y_speed1+1);
          stepper_z.setMaxSpeed(z_speed1+1);
          z_s = z_speed1;
        }
          if(stepper_x.currentPosition() > move_x_y_z[0]) { //if this is true it will be going away from the switch
            if (x_backlash == false) {
              long c_p_x = stepper_x.currentPosition();
              move_now(1, -x_m_backlash);
              stepper_x.setCurrentPosition(c_p_x);
            }
            x_backlash = true;
          }else if (stepper_x.currentPosition() < move_x_y_z[0]){
            if(x_backlash == true) {
              long c_p_x = stepper_x.currentPosition();
              move_now(1, x_m_backlash);
              stepper_x.setCurrentPosition(c_p_x);
            }
            x_backlash = false;
          }

          if(stepper_y.currentPosition() > move_x_y_z[1]) { //if this is true it will be going away from the switch
            if (y_backlash == false) {
              long c_p_y = stepper_y.currentPosition();
              move_now(2, -y_m_backlash);
              stepper_y.setCurrentPosition(c_p_y);
            }
            y_backlash = true;
          }else if (stepper_y.currentPosition() < move_x_y_z[1]){
            if(y_backlash == true) {
              long c_p_y = stepper_y.currentPosition();
              move_now(2, y_m_backlash);
              stepper_y.setCurrentPosition(c_p_y);
            }
            y_backlash = false;
          }
/*
          if(stepper_z.currentPosition() > move_x_y_z[2]) { //if this is true it will be going away from the switch
            if (z_backlash == false) {
              long c_p_z = stepper_z.currentPosition();
              move_now(3, -z_m_backlash);
              stepper_z.setCurrentPosition(c_p_z);
            }
            z_backlash = true;
          }else if (stepper_z.currentPosition() < move_x_y_z[2]){
            if(z_backlash == true) {
              long c_p_z = stepper_z.currentPosition();
              move_now(3, z_m_backlash);
              stepper_z.setCurrentPosition(c_p_z);
            }
            z_backlash = false;
          }
*/
        if (moves > 1) {
          steppers.moveTo(move_x_y_z);
          steppers.runSpeedToPosition();
          stepper_x.setCurrentPosition(move_x_y_z[0]);  
          stepper_y.setCurrentPosition(move_x_y_z[1]);
          stepper_z.setCurrentPosition(move_x_y_z[2]);
        }else {
          if (pos_x > 0)
            stepper_x.moveTo(move_x_y_z[0]);
          if (pos_y > 0)
            stepper_y.moveTo(move_x_y_z[1]);
          if (pos_z > 0)
            stepper_z.moveTo(move_x_y_z[2]);
          stepper_z.setSpeed(z_s);
          while (stepper_x.isRunning() || stepper_y.isRunning() || (stepper_z.distanceToGo() != 0)) {
            stepper_x.run();
            stepper_y.run();
            stepper_z.runSpeedToPosition();
          }
        }
      }else {
      Serial.println(F("Home!"));
      alt = true;
      return;
      }
    } else
    if (line[0] == 'G' && line[1] == '2' && line[2] == '8') {	//G28 (auto-home)
      if (!enabled) {
        Serial.print(F("\n mtr dis"));
        return;
      }
      update_limit(); 
      home_z(); 
      home_x();
      home_y();
      stepper_x.setCurrentPosition(0);
      stepper_y.setCurrentPosition(0);
      stepper_z.setCurrentPosition(0);
      offset_x = 0;
      offset_y = 0;
      offset_z = 0;
      ready = true; 
//      saveData();
    }else
    if (line[0] == 'M' && line[1] == '0' && line[2] == '3') {	//M3 
      digitalWrite(A4, HIGH);
    }else
    if (line[0] == 'M' && line[1] == '0' && line[2] == '5') {	//M5 
      digitalWrite(A4,LOW);
    }else
    if (line[0] == 'M' && line[1] == '2' && line[2] == '2' && line[3] == '0') {
  char* numPtr = strchr(line, ' ');
      if (numPtr != NULL) {
        if (numPtr[1] == 'S' || numPtr[1] == 's') {
          numPtr++;
         }
        int a = atoi(numPtr + 1);  // converte il numero dopo lo spazio
        if (a >= 0 && a <= 100) {
          x_y_speed1 = x_y_speed0 * a / 100;
          Serial.print(F("\n New spd set to "));
          Serial.println(x_y_speed1);
        }
      }
    }
  }
}

int find_string(char car, char line[]) {
	int i; 
	i = 0;
	while(line[i] != '\0') {
		if (line[i] ==  car)
			return i;
		i++;
	}
	return -1;
}

void home_x() {
  stepper_x.setSpeed(700);
  while (!limit_x) {
    stepper_x.runSpeed();
    update_limit();         
  }
  stepper_x.stop();
  stepper_x.setSpeed(x_y_speed0);
  x_backlash = false;
}
void home_y() {
  stepper_y.setSpeed(700);
  while (!limit_y) {
    stepper_y.runSpeed();
    update_limit();         
  }
  stepper_y.stop();
  stepper_y.setSpeed(x_y_speed0);
  y_backlash = false;
}

void home_z() {
  stepper_z.setSpeed(700);
  while (!limit_z) {
    stepper_z.runSpeed();
    update_limit();         
  }
  stepper_z.stop();
  stepper_z.setSpeed(z_speed0);
  z_backlash = false;
}

void home_z2() {
  digitalWrite(A5,HIGH);
  delay(10);
  stepper_z.setSpeed(-z_speed0);
  while((analogRead(A7) >= 512) && ((stepper_z.currentPosition()+offset_z) > min_z*steps_per_mm_z)) {
    stepper_z.runSpeed();
  }
  stepper_z.stop();
  stepper_z.setSpeed(z_speed0);
  if (z_backlash == false) {
  long tmp = stepper_z.currentPosition();
  stepper_z.setCurrentPosition(tmp - z_m_backlash);
  }
  z_backlash = true;
  digitalWrite(A5,LOW);
}

void file_read(char* path) {
  if (!SD.begin(10)) {
    Serial.println(F("SD non pronta"));
    return;
  }
  if (SD.exists(path)) {
    myFile = SD.open(path, FILE_READ);
    if (myFile) {
      while (myFile.available()) {
        if (ready == false)
          break;
        String line = myFile.readStringUntil('\n');
        char curr_command[40];
        line.toCharArray(curr_command, 40);
        Serial.print(F("\n"));
        Serial.print(stepper_z.currentPosition());
        Serial.print(F("\n"));
        Serial.print(line);
        readline(curr_command);
        Serial.print(F("\n"));
        Serial.print(stepper_z.currentPosition());
        Serial.print(F("\n strikes:"));
        Serial.println(strikes);
        delay(100);
        if(alt) {
          alt = false;
          Serial.print(F("Milling alted"));
          myFile.close();
          return;
        }
      }
      myFile.close();
      Serial.println(F("end of file"));
    }else
    Serial.println(F("tf"));
  }else
  Serial.println(F("!File"));
}

void find_boundaries() { //finds the biggest X and Y values in the file
  if (!SD.begin(10)) {
    Serial.println(F("SD non pronta"));
    return;
  }
  x_boundary = 0;
  y_boundary = 0;
  if (SD.exists("route.cnc")) {
    myFile = SD.open("route.cnc", FILE_READ);
    if (myFile) {
      while (myFile.available()) {
        String line = myFile.readStringUntil('\n');
        char curr_command[40];
        line.toCharArray(curr_command, 40);
        int pos_x = find_string('X', curr_command);
        int pos_y = find_string('Y',curr_command);
        if (pos_x > 0) { 
          char* sub_x = &curr_command[pos_x + 1];
          float move_x = atof(sub_x);
          if (move_x > x_boundary) {
            x_boundary = move_x;
          }
        }
        if (pos_y > 0) { 
          char* sub_y = &curr_command[pos_y + 1];
          float move_y = atof(sub_y);
          if (move_y > y_boundary) {
            y_boundary = move_y;
          }
        }
      }
    myFile.close();
    }
  }else {
    Serial.println(F("!File"));
  }
}

void probe_corners() {
  if(x_boundary == 0 || y_boundary == 0) {
    Serial.print(F("e2"));
    return;
  }
  if((x_boundary*steps_per_mm_xy+offset_x > max_x*steps_per_mm_xy) || (y_boundary*steps_per_mm_xy+offset_y > max_y*steps_per_mm_xy) || (x_boundary*steps_per_mm_xy+offset_x < 0) || (y_boundary*steps_per_mm_xy+offset_y < 0)) {
    Serial.print(F("e1"));
    return;
  }
  z_clear();
  z_map[0] = multiprobe();
  z_clear();
  moveTo_now(1,(-x_boundary*steps_per_mm_xy));
  z_map[1] = multiprobe();
  z_clear();
  moveTo_now(2,(-y_boundary*steps_per_mm_xy));
  z_map[2] = multiprobe();
  z_clear();
  moveTo_now(1,0);
  z_map[3] = multiprobe();
  z_clear();
  moveTo_now(1,-x_boundary*steps_per_mm_xy/2);
  moveTo_now(2,-y_boundary*steps_per_mm_xy/2);
  z_map[4] = multiprobe();
  z_clear();
  moveTo_now(2,0);
  moveTo_now(1,0);
  home_z2();
  is_probed = true;
}

void find_plane(float plane_coefficients[], int mode) {
  if(is_probed) {
    float v1x,v1y,v1z;
    float v2x,v2y,v2z;
    float p1x,p1y,p1z;
    float p2x,p2y,p2z;
    float p3x,p3y,p3z;
    
  if (mode == 0) { //piano 1
    p1x = 0; //punto 0
    p1y = 0;
    p1z = z_map[0];
    p2x = x_boundary; //punto 1
    p2y = 0;
    p2z = z_map[1];

  }else if (mode == 1) { //piano 2
    p1x = x_boundary; //punto 1
    p1y = 0;
    p1z = z_map[1];
    p2x = x_boundary; //punto 2
    p2y = y_boundary;
    p2z = z_map[2];
  }else if (mode == 2) { //piano 3
    p1x = x_boundary; //punto 2
    p1y = y_boundary;
    p1z = z_map[2];
    p2x = 0; //punto 3
    p2y = y_boundary;
    p2z = z_map[3];
  }else if (mode == 3) { //piano 4
    p1x = 0; //punto 3
    p1y = y_boundary;
    p1z = z_map[3];
    p2x = 0; //punto 0
    p2y = 0;
    p2z = z_map[0];
  }
  else 
    return;
  p3x = x_boundary/2; //punto 4
  p3y = y_boundary/2;
  p3z = z_map[4];
  v1x = p2x - p1x;
  v1y = p2y - p1y;
  v1z = p2z - p1z;
  v2x = p3x - p1x;
  v2y = p3y - p1y;
  v2z = p3z - p1z;
  float A = v1y * v2z - v1z * v2y;
  float B = v1z * v2x - v1x * v2z;
  float C = v1x * v2y - v1y * v2x;
  float D = -(A * p1x + B * p1y + C * p1z);
  plane_coefficients[0] = -A/C;
  plane_coefficients[1] = -B/C;
  plane_coefficients[2] = -D/C; 
  }
}

void moveTo_now(int a,long steps) { //guarda ci sarebbe da implementare il backlash ma è un po' scomodo perché la uso nel backlash di altre
  AccelStepper* stepper = nullptr; // Puntatore a un oggetto AccelStepper
  if (a == 1) {
    stepper = &stepper_x; // Riferimento a stepper_x
  } else if (a == 2) {
    stepper = &stepper_y; // Riferimento a stepper_y
  }

  if (stepper != nullptr) {
    stepper->moveTo(steps);
    while (stepper->isRunning())
      stepper->run();
  }
}

void move_now(int a, long steps) {//guarda ci sarebbe da implementare il backlash ma è un po' scomodo perché la uso nel backlash di altre
    AccelStepper* stepper = nullptr; // Puntatore a un oggetto AccelStepper
  if (a == 1) {
    stepper = &stepper_x; // Riferimento a stepper_x
  } else if (a == 2) {
    stepper = &stepper_y; // Riferimento a stepper_y
  } else if (a == 3) {
    stepper = &stepper_z;
  }

  if (stepper != nullptr) {
    stepper->move(steps);
    while (stepper->isRunning())
      stepper->run();
  }
}

void z_clear() {
  long add = 0;
  long tmp = stepper_z.currentPosition();
  if (z_backlash) {
    add = -z_m_backlash;
  }
  stepper_z.move(steps_per_mm_z + add);
  stepper_z.setSpeed(z_speed0);
  while(stepper_z.distanceToGo() != 0)
    stepper_z.runSpeedToPosition();
  stepper_z.setCurrentPosition(tmp + steps_per_mm_z);
  z_backlash = false;
}

long multiprobe() {
  long points[5];
  move_now(1, steps_per_mm_xy);
  move_now(2, steps_per_mm_xy);
  home_z2();
  points[0] = stepper_z.currentPosition();
  z_clear();
  move_now(1, -2*steps_per_mm_xy);
  home_z2();
  points[1] = stepper_z.currentPosition();
  z_clear();
  move_now(2, -2*steps_per_mm_xy);
  home_z2();
  points[2] = stepper_z.currentPosition();
  z_clear();
  move_now(1, 2*steps_per_mm_xy);
  home_z2();
  points [3] = stepper_z.currentPosition();
  z_clear();
  move_now(1, -steps_per_mm_xy);
  move_now(2, steps_per_mm_xy);
  home_z2();
  points [4] = stepper_z.currentPosition();
  long result = mean(points);
  return result;
}
#if PROBING == 3
long findMedian(long arr[]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4 - i; j++) {
      if (arr[j] > arr[j + 1]) {
        long temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
  return arr[2];
}

long mean(long arr[]) {

  long med = findMedian(arr);
  int sub = 0;
  for (int i = 0; i < 5; i++) {
    if (abs(arr[i] - med) > 20) {
      arr[i] = 0;
      sub++;
    }
  }

  long mean = 0;
  for (int i = 0; i <5; i++) {
    mean = arr[i] + mean;
  }
//  mean = (mean + 5/2) / 5;
  mean = (mean + (5 - sub) / 2) / (5 - sub);
  return mean;
}

#elif PROBING == 2
long mean(long arr[]) {
  long mean = 0;
  for (int i = 0; i <5; i++) {
    mean = arr[i] + mean;
  }
  mean = (mean + 5/2) / 5;
  return mean;
}

#elif PROBING == 1
long findMedian(long arr[]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4 - i; j++) {
      if (arr[j] > arr[j + 1]) {
        long temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
  return arr[2];
}
long mean(long arr[]) {
  long med = findMedian(arr);
  long res;
  for (int i = 0; i <5; i++) {
    if (res > arr[i] && arr[i] > med-20)
      res = arr[i];
  }
  return res;
}
#endif 


int which_plane(long coords[]) {
  bool tmp_a = false; //over the line from point 0 to 2 
  bool tmp_b = false;
  if( (coords[0]*y_boundary/(x_boundary*-steps_per_mm_xy)-coords[1]/-steps_per_mm_xy) <= 0) {
    tmp_a = true;
  }
  if (-coords[0]*y_boundary/(x_boundary*-steps_per_mm_xy)-coords[1]/-steps_per_mm_xy+y_boundary <= 0) {
    tmp_b = true;
  }
  if (tmp_b && tmp_a)
    return 3;
  if(tmp_b && !tmp_a) 
    return 2;
  if(tmp_a && !tmp_b)
    return 4;
  if(!tmp_a && !tmp_b)  
    return 1;
}

 /*
void saveData() { <-- seems to mess with the arduino (?) _-_
  if (SD.exists("save.bin")) {
    SD.remove("save.bin");
  }

  File file = SD.open("save.bin", FILE_WRITE);
  if (file) {
    long posX = stepper_x.currentPosition();
    long posY = stepper_y.currentPosition();
    long posZ = stepper_z.currentPosition();
    file.write((byte*)&posX, sizeof(posX));
    file.write((byte*)&posY, sizeof(posY));
    file.write((byte*)&posZ, sizeof(posZ));
    file.write((byte*)&offset_x, sizeof(offset_x));
    file.write((byte*)&offset_y, sizeof(offset_y));
    file.write((byte*)&offset_z, sizeof(offset_z));
    file.write(ready);
    file.write(x_backlash);
    file.write(y_backlash);
    file.write(z_backlash);
    uint8_t checksum = 0;
    checksum ^= (uint8_t)(posX ^ posY ^ posZ ^ offset_x ^ offset_y ^ offset_z ^ ready ^ x_backlash ^ y_backlash ^ z_backlash);
    file.write(checksum);
    file.close();
    Serial.println("curr. pos saved");
  } else {
    Serial.println("Errore apertura file per scrittura");
  }
}

void loadData() {
  File file = SD.open("save.bin", FILE_READ);
  if (file) {
    uint8_t checksumRead;
    long posX;
    long posY;
    long posZ;
    file.read((byte*)&posX, sizeof(posX));
    file.read((byte*)&posY, sizeof(posY));
    file.read((byte*)&posZ, sizeof(posZ));
    file.read((byte*)&offset_x, sizeof(offset_x));
    file.read((byte*)&offset_y, sizeof(offset_y));
    file.read((byte*)&offset_z, sizeof(offset_z));
    stepper_x.setCurrentPosition(posX);
    stepper_y.setCurrentPosition(posY);
    stepper_z.setCurrentPosition(posZ);
    ready = file.read();
    x_backlash = file.read();
    y_backlash = file.read();
    z_backlash = file.read();
    checksumRead = file.read();

    uint8_t checksumCalc = (uint8_t)(posX ^ posY ^ posZ ^ offset_x ^ offset_y ^ offset_z ^ ready ^ x_backlash ^ y_backlash ^ z_backlash);
    if (checksumRead != checksumCalc) {
      Serial.println("Checksum non valido, dati corrotti?");
    }else
      Serial.println("save loaded");
    file.close();
  } else {
    Serial.println("Errore apertura file per lettura");
  }
}
*/
