# This is currently a WIP, I still have to fix some stuff

# PCB MILLING CNC OVERVIEW  

<img width="1236" height="574" alt="image" src="https://github.com/user-attachments/assets/af06d7e1-19c9-42f3-aa06-0edbf7c719b0" />


I've decided to make this CNC from spare parts to see if I was up to the challenge. This was very time-consuming, and it is still a work in progress, but it is enough for a first publication, to see if people like it and maybe contribute.
****The total cost is around 150€, it also requires several hours of printing and assembly.****  

This project aims to create an affordable CNC that can be built with many common parts you might already have lying around: [here you can find the bill of materials.](https://docs.google.com/spreadsheets/d/1UYvKP2MYEV8sfaGlQmCoOGCL_2WACBQjZGL0-ml8eyc/edit?usp=sharing)

This project also needs some tools like:  
- A soldering iron
- Something to cut M8 and M4 threaded rods, like a hacksaw or Dremel (or even a bolt cutter for the M4 rod)
- M3 and M1 allen key (usually provided with the screw sets and collet).
- M8 wrench.
- Common household supplies like tweezers and scissors.
- A drill (even a weak one) to drive the M8 rods into PLA pieces and make holes.
- A hammer + small screwdriver (it is sometimes useful to drive the M4 rods into the PLA).
- Phillips screwdrivers for the screw terminals of the relay and motor driver.
- A hard brush to clean up the boards after milling. 

This machine is made to accept two common formats of copperclad board I can find online, which are 10x7cm and 5x7cm.
It is a relatively slow machine ([but it could be improved](https://github.com/Agasaworkshop/Agasa_CNC_PCB?tab=readme-ov-file#plans-for-upgrades)), and results also depends on the board quality.  
It is also very loud and requires eye protection, ear protection, and a respirator mask.

Here is a quick preview of the process of making a board (more info below):

| Step | Description    | Time taken   | Needs operator surveillance |
|:---------|:---------:|---------:|---------:|
| Make the PCB    | use [Kicad](https://www.kicad.org/) or some other software to make the gerber files | depends on the board    | Yes |
| Convert Gerber to CNC files    | use [flatCam](http://flatcam.org/) (or similar) to export a gcode with the proper format | 3 minutes     | Yes |
| Jog the machine to the starting point and change the bit | use gcode and the commands to set a relative origin, enable plane probing and change the bit | 5 minutes | Yes|
| Start milling | Use the commands to start the route.cnc file** | 6 minutes for plane probing and 22 minutes for the example PCB(the loud part) | No|
| Jog the machine to the starting point and change the bit | use gcode and the commands to set a relative origin, disable plane probing and change the bit| 3 minutes | Yes|
| Start drilling | Start the drill.cnc** file | 8 minutes for the example PCB | No| 
| Jog the machine to the starting point and change the bit | use gcode and the commands to set a relative origin and change the bit| 5 minutes | Yes|
| Start cutting | Start the edge.cnc** file | 20 minutes for the PCB shown*| Yes |
| Add UV mask | I would not advise making this on the CNC, I did try a bit  but it is not worth it time-wise | 10 minutes [(without CNC)](https://youtu.be/wvU2yyfH-XE?si=ILHJE20edIJDHiam) | Yes |  

*I was afraid to go too fast, so I didn’t stress test this. I used 0.3 mm passes at 250 speed (31% of max) with a 2 mm bit not meant for PCBs. Test with caution.
**You can also run other file names. I made some specific names and commands to make it quicker for my testing. 

Here are the PCBs I refer to  

<img width="640" height="288" alt="image" src="https://github.com/user-attachments/assets/795ba771-b2aa-42c6-8f0c-7b0da3591444" />
<img width="180" height="320" alt="image" src="https://github.com/user-attachments/assets/750a5886-00cc-42cc-824e-7d9b47f7383c" />  

The third one was done by milling the solder mask, but I do not recommend it as it is quite unreliable. 

Here are some standard results on FR1 board made with the latest release (note that you can get better results, but I tried to ensure reliability):
![20250907_205925](https://github.com/user-attachments/assets/6f5c4cfd-22de-419e-ba62-a8ca8f0d729e)
(they all work fine)

## What to expect
I REALLY do not want to deceive anyone, this machine is not perfect at all.  

This was a project made to challenge myself and allow me to make PCBs at home, the process is a bit convoluted, the arduino code could be less clunky (even though I am limited by the Arduino Nano’s memory, I should migrate this project to a more capable board in the future).  

Maybe I just need to loosen some screws, add more grease, go slower, or mount the Y rails better. But over long incisions, my machine does lose some steps on the Y axis. I haven’t tested this enough with larger boards to properly diagnose and fix it. It might simply be solved by adding a way to ensure proper Y alignment.  
For reference, during a 50-minute incision (almost the largest this machine can handle), it lost about 1 mm on the Y axis. I have no more boards to cut, so I can’t say if it was just a fluke (and I want to stop working on this machine for a while).   
(Note: the 50 minutes refer to the engraving time, during which the machine lost a significant amount of steps. Drilling took another ~40 minutes due to the large number of holes, plus a couple of minutes for the mounting holes).  

Here is the board: the traces were very nice, but they drifted by 1 mm over time, making the holes misaligned. (When I noticed the drift, I decided to re-home between cutting and drilling).  
<img width="657" height="458" alt="image" src="https://github.com/user-attachments/assets/f7de2bdd-1453-4b75-a794-e7135a4344fe" />  

Due to the size of the board, I suggest cutting deeper to ensure proper isolation. This worked reasonably well, but I had to refine 5 pads with a box cutter. (I used –0.12 mm; –0.15 mm would probably have been better).  

There is also a second kind of distortion (not related to lost steps) that causes some traces to deform, this is usually harmless unless it happens on the segment that closes the shape, to prevent this from causing problems I've added the "add squares" option to the patcher. It seems to work fine. 


# Assembly
Being a budget machine, it requires considerable effort on the user's part for assembly.  
Additional hardware notes can be found [here](HARDWARE_NOTES.md).
## Frame
The frame is made of hollow PLA segments reinforced with M8 threaded rods and filled with concrete.  
This creates a very rigid structure with decent tolerances, good vibration damping, and low cost. The M8 rods also help with assembly.  
<img width="435" height="384" alt="image" src="https://github.com/user-attachments/assets/9d834906-fd3d-41d3-b579-b86fed3c607e" />
<img width="301" height="342" alt="image" src="https://github.com/user-attachments/assets/0a4a1318-8cf1-455b-94ce-deca73f43f02" />
<img width="359" height="301" alt="image" src="https://github.com/user-attachments/assets/e30da32b-471b-47de-95ca-91e0f61a9815" />

## Axes
Describing the assembly of each small piece in detail would be complex and not very clear. That’s why I prepared four files—Preview.blend, Preview_x.blend, Preview_y.blend, and Preview_z.blend—each containing all 3D-printed pieces and hardware in their correct positions, with [color-coded](https://docs.google.com/spreadsheets/d/1UYvKP2MYEV8sfaGlQmCoOGCL_2WACBQjZGL0-ml8eyc/edit?gid=2135254340#gid=2135254340) hardware. 
Apply grease to all screws and guides.
Unfortunately, I am dumb enough to model in blender.  
I didn’t find a better way to display the assembly, so you will need to [download Blender](https://www.blender.org/download/) (free) to view the files.  
This also means I don’t have convenient CAD files for the project. Even the STL files come from messy Blender projects, so I will consider the STL files themselves as the source for licensing purposes.  
<img width="378" height="339" alt="image" src="https://github.com/user-attachments/assets/fb206a9c-3215-41fb-9aa6-13b725eed116" />
<img width="486" height="206" alt="image" src="https://github.com/user-attachments/assets/66115400-01d0-4332-8afb-8a9262f7b302" />
<img width="336" height="282" alt="image" src="https://github.com/user-attachments/assets/1419acae-98ce-44d2-ba9b-e910e4a3c0b1" />
<img width="228" height="233" alt="image" src="https://github.com/user-attachments/assets/b4b075bf-19ed-477a-8c75-959a1e54f7b3" />



### Bellows
Unfortunately, this was a bit rushed.  
You can print the bellows flat on your bed with a 0.12mm layer height. 
This will result in a sheet that can easily be folded back and forth to make the bellows. I did not add some of the features to make them easier to detach from the plate, but they can be easily cut out.
Here is a short video that shows how I installed them.  

## Electronics
The electronics require some work, but it should not be a problem for anyone building PCBs.  
Here is the list of boards I used:  
3x ULN2003 stepper motor driver  
1x micro SD card reader  
1x Arduino Nano  
1x LM2596 DC to DC Buck Converter module   
1x relay module  
1x custom board (see below)  
1x custom board to distribute power (just a couple of lines of solder with all the cables going out to the various modules)
1x metal M3x10cm washer used for the motor.  
1x metal washer with a bigger diameter used on the bed.  

-I power this with a computer power supply (the 12V line). I suggest using this type of power supply, as they can be obtained cheaply or even for free from old computers and online marketplaces such as eBay or Facebook.
-Power the ULN2003 with 12 V, along with the motor driver and the step-down module (if you use a computer PSU, you can simply connect the 5 V line).
-Set the step-down output to 5 V, and connect the Arduino, relay, and SD card boards to it.
-The relay is used to switch on and off the motor driver.    
-The motor driver is connected to the motor with a flyback diode and large capacitors rated for more than 12 V.  

This is the pinout of the Arduino:  

| Arduino pin | Conn. | Arduino pin | Conn. |
| :---------: | :---: | :---------: | :---: |
|     TX1     |   NC   |     D13     |   SCK sd card board   |
|     RX0     |   NC   |     3v3     |   NC   |
|     RST     |   NC   |     REF     |   NC   |
|     GND     |   OUT- stepdown   |      A0     |   MZ IN4   |
|      D2     |   MY IN4   |      A1     |   MZ IN3   |
|      D3     |   MY IN3   |      A2     |   MZ IN2   |
|      D4     |   MY IN2   |      A3     |   MZ IN1   |
|      D5     |   MY IN1   |      A4     |   Relay S |
|      D6     |   MX IN4   |      A5     |   Z2 enable pin custom board   |
|      D7     |   MX IN3   |      A6     |   switch pin on custom board  |
|      D8     |   MX IN2   |      A7     |   Z2 pin on custom board |
|      D9     |   MX IN1   |      5V     |   OUT+ stepdown   |
|     D10     |   CS sd card board   |     RST     |   NC   |
|     D11     |   MOSI sd card board   |     GND     |   NC   |
|     D12     |   MISO sd card board   |     Vin     |   NC   |  

NC = not connected  
MX = ULN2003 connected to the x motor  
MY = ULN2003 connected to the y motor   
MZ = ULN2003 connected to the z motor   

To check all the limit switches and the contact between the board and the motor with a few pins, I have made a little custom board on perfboard.  
Here is the schematic:  
<img width="640" height="429" alt="image" src="https://github.com/user-attachments/assets/55893924-4c61-4fee-ae61-7b21390d2746" />   
I suggest adding connectors (I used JST connectors) at the positions indicated by the red boxes.  

I mounted the electronics with my [CMS](https://www.printables.com/@AgasasWorksho_391302/collections/863243) system on a printed plate (that you can find in the files)  

<img width="1280" height="576" alt="image" src="https://github.com/user-attachments/assets/ba09b5ae-814f-45e1-bd35-952cb0c2a404" />  

Alternatively, you could put together the circuit on perfboard like shown here:
<img width="640" height="288" alt="image" src="https://github.com/user-attachments/assets/79159e76-e36b-4f37-a818-8d021695b795" />

Once the machine is ready, you could mill your own board. 
Probably the most convenient way of doing this would be to buy one from a site from the start.  
I created a schematic for this, which is also useful if you want to build the perfboard version.  

How the washers are  mounted:   
<img width="408" height="339" alt="image" src="https://github.com/user-attachments/assets/01417ccc-a4d2-4eb2-bcac-8ee069456f55" />   
You can solder tin to steel; it will flow nicely once the washer reaches a sufficient temperature. This may take some time, during which the solder will not adhere.  

# Using the machine
The code does the minimum required to make the machine work. It can read some G-code, probe the plane to compensate for a skewed board or other imperfections. Here is the list of commands:  
1 + {Gcode} = executes that Gcode.  
2 = Saves the current X, Y, Z coordinates to the offset, and considers the current position as the origin.  
3 = writes in the console the X,Y,Z coordinates, the offsets and some more stuff.  
4 = lowers the Z axis until it finds contact.  
5 = starts the "route.cnc" file.  
50 + "file_name.cnc" = starts the "file_name.cnc" file. For example, 50 route.cnc is equivalent to command 5 (max 16 characters).
51 = starts the "mask.cnc" file. It does not disable plane probing and does not perform probing; you need to have done the previous probing. (Do not shut it down after route.cnc).
52 = starts  the "drill.cnc" file, disables plane probing.  
53 = starts  the "cut.cnc" file, disables plane probing.  
6 = tries to initiate the SD card (also done at start).  
7 = prints SD card content.   
8 = homes the Z axis alone.   
9 = toggles plane probing.   
10 = toggles debug mode, which enables moving without homing.   
11 = toggles enabled and disabled holding torque.  

❕Note that the 51/52/53 commands are already tailored for their purpose and should not need you to toggle plane probing, if you use the 50 command you must disable the plane probing if you don't want it (for example if you decided to make a different drill_1.cnc file make sure to have plane probing off).

Change relative system  
21 = save solder mask relative system.  
22 = save drill relative system.  
23  = save cutting relative system.  
211 = load solder mask relative system.  
221 = load drill relative system.   
231 = load cutting relative system.  

The machine currently cannot change spindle speed. You can manually adjust it on some motor drivers. I always run it at 100%. I plan to update this; it should be fairly simple, but I’m burned out.  
The machine currently only recognizes  
G00/G01  (movement)  
M03/M05  (spindle on/off)  
G28  (home all axes)  
G220 (change stepper speed, for example "G220 80" (or "G220 S80 it's the same) will set the G01 speed as 80% of the G00 speed which is considered the fastest speed (can't go over 100% or under 0%)  
I did attempt some extra features, but the Arduino Nano is already running out of memory.  

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
This machine involves mechanical, electrical, and operational risks. I strongly recommend using appropriate personal protective equipment (the above mentioned PPE) and ensuring proper ventilation during operation.  
Keep your hands and body parts away from moving axes and the cutting tool, as the system can generate significant force.  
Be careful when wiring the power supply, especially when working with mains voltage.

This project is not certified for compliance with any safety or regulatory standards. By building, modifying, or operating this machine, you acknowledge and accept that you do so entirely at your own risk.
I shall not be held liable for any injury, damage, loss, or legal issues resulting from the use or misuse of this project.

## Extra
- This machine operates on two sizes of boards, which are 5x7 and 10x7. [I've made a little jig to score and snap the boards](https://www.printables.com/model/1377196-107-pcb-splitter), it works well with FR1, not too well with FR4 (it isn't impossible tho) 
- I did attempt some backlash compensation with disappointing results, the current code still has the remains of that (but the parameters are set to 0)
