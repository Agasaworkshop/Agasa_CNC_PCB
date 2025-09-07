# This is currently a WIP, I still have to fix some stuff

# PCB MILLING CNC OVERVIEW  

<img width="1236" height="574" alt="image" src="https://github.com/user-attachments/assets/af06d7e1-19c9-42f3-aa06-0edbf7c719b0" />


I've decided to make this CNC from spare parts to see if I was up to the challenge. This was very, time-consuming and it is still a work in progress, but it is enough to make a first publication to see if people like it and maybe even contribute to it.  

****The total cost is around 150€, it also requires several hours of printing and assembly.****  

This project aims at making an affordable CNC that can be built with many common parts you might already have lying around: [here you can find the bill of materials.](https://docs.google.com/spreadsheets/d/1UYvKP2MYEV8sfaGlQmCoOGCL_2WACBQjZGL0-ml8eyc/edit?usp=sharing)

This project also needs some tools like:  
- A soldering iron
- Something to cut M8 and M4 threaded rod like a hacksaw or Dremel (or even a bolt cutter for the M4 rod)
- M3 and M1 allen key (usually provided with the screw sets and collet).
- M8 wrench.
- Common household supplies like tweezers and scissors.
- A drill (even the weak ones) to drive the M8 rods into PLA pieces and drill.
- A hammer + small screwdriver (it is sometimes useful to drive the M4 rods into the PLA)
- Phillips screwdrivers for the screw terminals of the relay and motor driver
- A hard brush to clean up the boards after milling. 

This machine is made to accept two common formats of copperclad board I can find online, which are 10x7cm and 5x7cm.
It is a relatively slow machine ([but it could be improved](https://github.com/Agasaworkshop/Agasa_CNC_PCB?tab=readme-ov-file#plans-for-upgrades)), and it can achieve some decent results but it also depends on the quality of the board itself.  
It is also a very loud machine and requires eye protection, ear protection, and a respirator mask.  


Here is a quick preview of the process of making a board (more info below):

| Step | Description    | Time taken   | Needs operator surveillance |
|:---------|:---------:|---------:|---------:|
| Make the PCB    | use [Kicad](https://www.kicad.org/) or some other software to make the gerber files | depends on the board    | Yes |
| Convert Gerber to CNC files    | use [flatCam](http://flatcam.org/) (or similar) to export a gcode with the proper format | 3 minutes     | Yes |
| Jog the machine to the starting point and change the bit | use gcode and the commands to set a relative origin, enable plane probing and change the bit | 5 minutes | Yes|
| Start milling | Use the commands to start the route.cnc file** | 6 minutes for plane probing and 22 minutes for the example PCB(the loud part) | No|
| Jog the machine to the starting point and change the bit | use gcode and the commands to set a relative origin, disable plane probing and change the bit| 3 minutes | Yes|
| Start drilling | Use  the commands to start the drill.cnc** file | 8 minutes for the example PCB | No| 
| Jog the machine to the starting point and change the bit | use gcode and the commands to set a relative origin and change the bit| 5 minutes | Yes|
| Start cutting | Use the commands to start the edge.cnc** file | 20 minutes for the PCB shown*| Yes |
| Add UV mask | I would not advise making this on the CNC, I did try a bit  but it is not worth it time-wise | 10 minutes [(without CNC)](https://youtu.be/wvU2yyfH-XE?si=ILHJE20edIJDHiam) | Yes |  

*I was afraid to go too fast, I did not stress test this, I did 0.3mm passes at 250 speed (31% speed) with a bit that is not meant for PCBs and was 2mm in diameter, test with caution. 
**you can also run other file names. I made some specific names and commands to make it quicker for my testing. 

Here are the PCBs I refer to  

<img width="640" height="288" alt="image" src="https://github.com/user-attachments/assets/795ba771-b2aa-42c6-8f0c-7b0da3591444" />
<img width="180" height="320" alt="image" src="https://github.com/user-attachments/assets/750a5886-00cc-42cc-824e-7d9b47f7383c" />  

Note that the first image contains an exceptionally good result (the second board), depending on the bit and the board you don't always end up with such clear lines, look at the one on the left for comparison.  
The third one was done by milling the solder mask, but I do not recommend it as it is quite finicky. 

Here are some standard results on FR1 board made with the latest release (note that you can get better results, but I tried to ensure reliability):
![20250907_205925](https://github.com/user-attachments/assets/6f5c4cfd-22de-419e-ba62-a8ca8f0d729e)
(they all work fine)

## What to expect
This was a project made to challenge myself and allow me to make PCBs at home, it seems to work kinda fine, it is not perfect tho, the process is a bit convoluted, the arduino code could be less cluncky (even tho I am limited by the arduino nano memory, I should migrate this project to a more capable board in the future).  
The end result is interesting, the boards are functional, the spacing of the pads is consistent enough for the UV mask(for the solder mask) to line up properly but it does seem to have some kind of distortion that I would attribute to either play in the X and Y axis or the bit flexing a bit, I have tried to mitigate this a bit but with little success, the board works because I added some extra features to make the machine reliable but it might mangle a bit some shapes. I do not think it is missing steps because I can run the same code (that presents this distortion) twice on the same board and have the lines perfectly overlap. The pattern does not seem to degenerate over time (so it does not seem like it's losing steps). 
The holes also do not come out perfectly centered, not sure why, but it's not a big deal, ICs still fit more than fine.

# Assembly
Being a budget machine, it requires considerable effort on the user's part for assembly.
## Frame
The frame is made out of hollow PLA segments reinforced with M8 threaded rods and filled with concrete, which makes for a very rigid material with decent tolerances, good vibration dampening and very cheap, the M8 rods also help with assembling them together.  

<img width="435" height="384" alt="image" src="https://github.com/user-attachments/assets/9d834906-fd3d-41d3-b579-b86fed3c607e" />
<img width="301" height="342" alt="image" src="https://github.com/user-attachments/assets/0a4a1318-8cf1-455b-94ce-deca73f43f02" />
<img width="359" height="301" alt="image" src="https://github.com/user-attachments/assets/e30da32b-471b-47de-95ca-91e0f61a9815" />

## Axes
It would be quite complex and wasteful to painstakingly describe the assembly of each little piece; it also would not be that easy to understand, which is why I decided to prepare 4 files called Preview.blend, Preview_x.blend, Preview_y.blend, and Preview_z.blend, each containing every 3d printed piece and hardware positioned in the right place and with [color coded](https://docs.google.com/spreadsheets/d/1UYvKP2MYEV8sfaGlQmCoOGCL_2WACBQjZGL0-ml8eyc/edit?gid=2135254340#gid=2135254340) hardware.  
Unfortunately I am dumb enough to model in blender. I did not find a much better way of displaying this so you will have to [download Blender](https://www.blender.org/download/) (which is free) to access those files.  
<img width="325" height="335" alt="image" src="https://github.com/user-attachments/assets/9df7ed10-f0a5-4033-9d78-faac04b87ae6" />
<img width="486" height="206" alt="image" src="https://github.com/user-attachments/assets/66115400-01d0-4332-8afb-8a9262f7b302" />
<img width="336" height="282" alt="image" src="https://github.com/user-attachments/assets/1419acae-98ce-44d2-ba9b-e910e4a3c0b1" />
<img width="228" height="233" alt="image" src="https://github.com/user-attachments/assets/b4b075bf-19ed-477a-8c75-959a1e54f7b3" />

### Bellows
This was unfortunately a bit rushed, you can print the bellows flat on your bed with a 0.12mm layer height. 
This will result in a sheet that can easily be folded back and forth to make the bellow, I did not add some features to make them easier to detach from the plate; they can be easily cut out. 
Here is a short video that shows how I added them 

## Electronics
For the electronics, you will have to work a bit; it should not be a problem for a user who wants to make PCBs.  
Here is the list of boards I used:  
3x ULN2003 stepper motor driver  
1x micro SD card reader  
1x Arduino Nano  
1x LM2596 DC to DC Buck Converter   
1x relay module  
1x custom board (see below)  
1x custom board to distribute power (just a couple of lines of solder with all the cables going out to the various modules)
1x metal M3x10cm washer that will go to the motor  
1x metal washer with a bigger diameter that will go on the bed  

-I power this with a computer power supply (the 12V line). I suggest using this kind of power supply because you can get them for very cheap or even free from old computers and ebay/facebook marketplace.  
-Power the ULN2003 with 12V, the motor driver, and the stepdown.  
-Set the stepdown output to 5V and connect the Arduino, relay and SD card boards to it. 
-The relay is used to switch on and off the motor driver.
-The motor driver is connected to the motor with a flyback diode and some large capacitors (that are rated for more than 12V).

This is the pinout of the Arduino:

| Arduino pin | Conn. | Arduino pin | Conn. |
| :---------: | :---: | :---------: | :---: |
|     TX1     |   NC   |     D13     |   SCK sd card board   |
|     RX0     |   NC   |     3v3     |   NC   |
|     RST     |   NC   |     REF     |   NC   |
|     GND     |   OUT- stepdown   |      A0     |   MY IN1   |
|      D2     |   MZ IN2   |      A1     |   MY IN2   |
|      D3     |   MZ IN1   |      A2     |   MY IN3   |
|      D4     |   MZ IN3   |      A3     |   MY IN4   |
|      D5     |   MX IN4   |      A4     |   Relay S |
|      D6     |   MX IN3   |      A5     |   Z2 enable pin custom board   |
|      D7     |   MX IN2   |      A6     |   switch pin on custom board  |
|      D8     |   MX IN1   |      A7     |   Z2 pin on custom board |
|      D9     |   MZ IN4   |      5V     |   OUT+ stepdown   |
|     D10     |   CS sd card board   |     RST     |   NC   |
|     D11     |   MOSI sd card board   |     GND     |   NC   |
|     D12     |   MISO sd card board   |     Vin     |   NC   |  

NC = not connected  
MX = ULN2003 connected to the x motor  
MY = ULN2003 connected to the y motor   
MZ = ULN2003 connected to the z motor   

To use just one pin to check all the limit switches and the contact between the board and the motor, I have made a little custom board on proof board.  
Here is the schematic:  
<img width="640" height="429" alt="image" src="https://github.com/user-attachments/assets/55893924-4c61-4fee-ae61-7b21390d2746" />   
I suggest adding connectors (I used JsT connectors) where I put red boxes.  

I mounted the electronics with my [CMS](https://www.printables.com/@AgasasWorksho_391302/collections/863243) system on a printed plate (that you can find in the files)  

<img width="1280" height="576" alt="image" src="https://github.com/user-attachments/assets/ba09b5ae-814f-45e1-bd35-952cb0c2a404" />  

How the washers are  mounted, you can solder tin to steel, it will flow nicely once the washer gets pretty hot (it takes a bit, during that time it will not stick at all)  

<img width="408" height="339" alt="image" src="https://github.com/user-attachments/assets/01417ccc-a4d2-4eb2-bcac-8ee069456f55" />

# Using the machine
The code does the least possible work to get this kinda working, it can read some Gcode, it can probe the plane to compensate for a skewed board or other imperfections, it is meant to only read some files with a specific name, here is the list of commands:
1 + {Gcode} = executes that Gcode  
2 = Saves the current x,y,z coordinates to the offset and considers the current position as the origin  
3 = writes in the console the x,y,z coordinates, the offsets and some more stuff  
4 = lowers the Z axis until it finds contact  
5 = starts the "route.cnc" file  
50 + "file_name.cnc" = starts the "file_name.cnc" file; so 50 route.cnc is the same as 5. (max 16 characters) 
51 = starts the "mask.cnc" file, does not disable plane probing and does not probe, you need the previous probing (so don't shut it down after route.cnc) <- could be improved  
52 = starts  the "drill.cnc" file, disables plane probing  
53 = starts  the "cut.cnc" file, disables plane probing  
6 = tries to initiate the SD card (also done at start)
7 = prints SD card content   
8 = homes the Z axis alone  
9 = toggles plane probing   
10 = toggles debug mode, which enables moving without homing   
11 = toggles enabled and disabled holding torque  

❕Note that the 51/52/53 commands are already tailored for their purpose and should not need you to toggle plane probing, if you use the 50 command you must disable the plane probing if you don't want it (for example if you decided to make a different drill_1.cnc file make sure to have plane probing off).

Change relative system  
21 save solder mask relative system  
22 save drill relative system  
23  save cutting relative system  
211 load solder mask relative system  
221 load drill relative system   
231 load cutting relative system  

The machine can not currently change spindle speed, you can manually change it from some motor drivers, I always run it at 100%, I want to update this, it should be fairly simple but I'm burned out.  
The machine currently only recognizes  
G00/G01  (movement)  
M03/M05  (spindle on/off)  
G28  (home all axes)  
G220 (change stepper speed, for example "G220 80" (or "G220 S80 it's the same) will set the G01 speed as 80% of the G00 speed which is considered the fastest speed (can't go over 100% or under 0%)  
I did attempt some extra features but the arduino nano is running out of memory already.   

## How to prepare the files
- Create the gerber file.  
- Import the gerber files of the engraving, holes and cuts in flatcam.  
- Select the engraving, go to isolation routing, set the thickness of the bit, I use 0.2mm(sometimes 0.1), generate the geometry and set the bit as a C bit (even if it is a V bit), select a cutting depth, I found that different boards call for different values so you will have to experiment a bit, I found good results on FR1 boards with a depth of -0.1mm.
- Set the end move and tool change Z to a low value (so that you do not exceed the limit) and generate the cnc object, save the file as route.cnc, open the CNC_file_patcher I provided and drag and drop the file onto it, it will get patched for this machine. (note that you can call the file whatever you want if you intend on using the 50 command (max 16 characters tho))  
- Select the drill file, set tool change z and end move to 3mm and the cutting depth a bit short of the PCB thickness, i use -1.35mm on 1.4mm boards, this will ensure you do not ruin the bed, it will also mean that you'll have to complete the holes yourself but it is a very easy and quick job, save the file as drill.cnc (you can call it whatever you want if you use the 50 command).  
-  Select the edge  file, set the bit diameter, set the depth to the same you chose for the drill file, I did -0.3mm passes max, I did not want to stress it too much, export it as edge.cnc (you can call it whatever you want if you use the 50 command).
-  Select the mask file, set the bit diameter, use the paint tool, set an offset, I'd recommend 0.5mm, but you'll have to check if with your bit diameter the mask gets completely cut out (the software won't do areas that would be too small for the bit + offset), export as mask.cnc (you can call it whatever you want if you use the 50 command).   


## How to operate the CNC  
‼️‼️You MUST wear protective equipment (PPE) when dealing with this machine, eye protection, ear protection, a respirator mask(especially for edge cutting and drilling) and work in a well ventilated area ESPECIALLY with FR4 boards and during cutting. I don't even recommend cutting edges for FR4 board; I would advise only to cut edges on FR1 boards.   
 
Follow the commands with the M only if you're also planning on milling the soldermask, N only if you're not planning on milling the solder mask; **I do not recommend it**.  
!!As it stands, disconnecting the computer and reconnecting will lead to the Arduino resetting and losing the ability to keep going with what you were doing. Use a reliable method, I found out my phone is great at this; it can go in standby and still not lose the serial connection and thus not reset the Arduino. 
- 1)Power up the machine with the external power supply and connect your laptop.  
- 2)Secure a board with all bolts, make sure the bed washer is tight; you risk damaging the bit if you forget.
- 3)Before moving you must home(the machine will not accept movement if not homed), so send over serial (9600 baud rate) "1 G28" 1 means that what follows has to be interpreted as GCODE ad G28 is the home all axes gcode.  
- 4)Jog the machine to what you want to be the starting position, use G00 to do so (G00 is the quick movement), a good starting point is in the bottom left corner, it's at around X20 Y20    
for example: "1 G00 X20 Y20".  
- M5.1.1)Secure the mask milling bit and use the 4 command to bring down the motor to the copperclad board, save the mask relative system with 21 and home the Z axis with the 8 command.   
- M5.1.2)Secure the drilling bit and use the 4 command to bring down the motor to the copperclad board, save the drill relative system with 22 and home the Z axis with the 8 command.  
- M5.1.3)Secure the cutting bit and use the 4 command to bring down the motor to the copperclad board, save the cutting relative system with 23 and home the Z axis with the 8 command.  
- 5.2)Secure the engraving bit and use the 4 command to bring down the motor to the copperclad board, save the current position as a new origin with the 2 command.
- 6)Enable plane probing with the 9 command; it should say "Prob:on" in the console.  
- 7)Start the engraving process with the 5 command. (or 50 + file name if you decided to call it differently)
- 8)Wait for the engraving to finish, in my experience, it is relatively safe and hands off but really loud.  
- 9)Remove the dust with a mini vacuum cleaner.  
- M10)Mount the mask milling bit, load the relative system with 211, go to the starting point, since you saved the relative system in step 5.2 you will have to use "1 G00 X0 Y0".  
- M11)Start the mask milling with 51, set the motor speed to 3% (to mill the mask you will need to be able to control the motor).  
- M12)Wait for the mask milling to end, this is also hands off, less loud tho.
- 13)Mount the drilling bit, set the speed to max again, disable plane probing with the 9 command.  
- M14)Load the drill relative system with 221 and go back to the origin (1 G00 X0 Y0).  
- N14)Go back to the origin (1 G00 X0 Y0), lower the z axis with the 4 command and set a new relative system with the 2 command.  
- 15)Start the drilling process with the 52 command.
- 16)Wait for the drilling to finish, in my experience it is relatively safe and hands off but really loud.  
- 17)Remove dust.  
- 18)Secure the cutting bit.  
- M19)Load the cutting relative system with 231,change the speed to 30%(M220 30), and go back to the origin(1 G00 X0 Y0).  
- N19)Go back to the origin, lower the z axis with the 4 command and set a new relative system with the 2 command.  
- 20)Start the cutting process with the 53 command.  
- 21)Wait for the cut to finish, while you're at it keep an eye on it, and remove some dust, I suggest moving it with a brush and vacuuming it off the board.
- 23)dismount the board, give it a vigorous scrub with a hard brush, check for continuity, completely drill out the holes and break the remaining piece of board attached after cutting with a box cutter.
- N24)Add the solder mask like in [this video](https://youtu.be/wvU2yyfH-XE?si=ILHJE20edIJDHiam).

❕❕Every time you secure a new bit you will have to check for the runout, I just look at it and spin it slowly to check a bit, this is very finicky; it will depend on the quality of your coupler or collet, I still fight a lot with my coupler to get it right, sometimes it looks fine but it isn't, it's the biggest problem for me and it would be solved just by finding good quality 3.17mm couplers, I yet have to find them tho. 

## Post processing
I vigorously scrub the board with a hard brush that usually does the trick, in some cases I had to send the board a bit with 500 grit sandpaper(for metals) to fix some not properly isolated pads, it takes less than a minute in total.  
I do advise checking for continuity on your board always.  
In the worst case scenario, you can use an exacto knife or box cutter to fix traces but I worked a ton to avoid having to do it I hope it does not happen.  
Soldering without a solder mask is a harrowing process, so I would advise adding it as shown [here](https://youtu.be/wvU2yyfH-XE?si=ILHJE20edIJDHiam). I am not very pleased with the tests I've made by milling the mask myself, but it does depend a lot on the spring-loaded bit.


## File patcher
The file patcher takes a file (drag and drop on the window) patches it and overwrites it with the new version. I currently have 4 features that you can toggle:  
- Split segments: subdivides long segments into shorter ones (max 2mm). This is done to improve the plane compensation (cause it currently creates 4 planes to approximate the board and you don't want long segments over multiple planes).
- Add overlap: adds some overlap between the end of the trace and the start to improve the odds of isolating it correctly (the copper can sometimes deform and leave a bridge)
- Add squares: due to some errors (I think either the bit is flexing or some play in the axys) sometimes when it has to close a shape it does not properly reconnect, to improve reliabilty this option adds a very small square at the end to cut a bit around the end.
- Simplify shapes: this could speed up the cut a bit.  
The settings that I do recommend are pre-checked.  
I will probably at some point add the ability to customize the squares and some other stuff.    

# Plans for upgrades
I decided to finalize this current version of the project because I wanted to get a break from it, but I do have A LOT of things I want to improve.  
I'll suggest them here:  
1. One could tinker with the speeds, I pushed the x and y motors quite a bit already but I did not consider the z speed to be that impactful on the time. 
2. Considering the little additional cost of nema motors one could decide to mount a pancake NEMA17 motor to the Z and X axis and even a standard NEMA17 to the Y axis, the code should be fairly simple, I would mostly care about fitting nemas to X and Y and try to get a better speed.
3. I've tested quite a bit with milling the solder mask and did not have perfect results. I currently suspect my DIY bit is not up to the task, I should investigate further but it felt like a fool's errand, knowing the other way of adding the solder mask.
4. I've fitted the bellows at the end and did not do the best job, I would love to improve on them.
5. Currently, the spindle is very loud. I'd like to figure out if I can do something about it.
6. The current system of regulating spindle speed is a bit crude due to the fact that, if you avoid milling the mask, you can just have 1 speed, but fitting an H-bridge would be a no-brainer (the motor should draw around 2.5A).
7. I would like to test different types of plane probing to maybe make it quicker (it currently takes a total of 25 points 5 at each spot, maybe I could do just 1 for each spot) 
8. I did not tinker enough with the cutting speeds because I was a bit afraid of wrecking the machine at the finish line, I'll have to test a bit after I recover from burnout. 
9. I might want to migrate the code to a better microcontroller, I used arduino to be more accessible but its limitations did show.
10. I tinkered with adding a saving system to avoid homing all the times and to avoid losing progress if one ended up resetting the Arduino by accident, the problem was the arduino memory that was running out.
11. If I do end up changing some of the above I would probably implement some extra G code like spindle speed.
12. Find the best working coupler to get good alignment every time, it's the biggest bottleneck.
13. I could also consider adding alignment holes to the bed to try implementing double sided boards.

# Disclamer
This machine involves mechanical, electrical, and operational risks. I strongly recommend using appropriate personal protective equipment (the above mentioned PPE) and ensuring proper ventilation during operation. Keep your hands and body parts away from moving axes and the cutting tool, as the system can generate significant force. Be careful when wiring the power supply, especially when working with mains voltage.

This project is not certified for compliance with any safety or regulatory standards. By building, modifying, or operating this machine, you acknowledge and accept that you do so entirely at your own risk.
I shall not be held liable for any injury, damage, loss, or legal issues resulting from the use or misuse of this project.

## Extra
This machine operates on two sizes of boards, which are 5x7 and 10x7. [I've made a little jig to score and snap the boards](https://www.printables.com/model/1377196-107-pcb-splitter), it works well with FR1, not too well with FR4 (it isn't impossible tho) 
