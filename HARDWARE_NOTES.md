# I've experimented quite a bit with different hardware and configurations. I would like to share my findings and ideas here:

## Collets:  
<img width="1268" height="548" alt="image" src="https://github.com/user-attachments/assets/e0739cb0-1c7c-43a8-bf17-a19937fa7738" />  
Here are all the collets (all 3.17mm to 3.17mm couplers or collets) I've tried:
1. The hole had quite a big tolerance; the bit ended up being wonky most of the times it was very frustrating to correct.
2. Same as 1
3. It was too tight, I could not fit the bits.
4. It had similar tollerances to 1 and 2 but the coupler nature makes it a bit easier to adjust, it's also quite heavy.
5. This fit the bit perfectly, the only problem was that it was too tall, this prevented me from mounting 
6. This is my current choice, it has similar problems to 4 but it's quite a lot lighter and a bit smaller.

I suggest marking one side and tightening mostly that side to both motor shaft and cutting bit to improve probability of working.  
EVERY TIME you change bit you need to check for runout, I don't have anything fancy I jsut spin the bit with my fingers and check if I see it wobble.   
Rule of thumb: if you think there is an imperceptible wobble but you're not sure, there probably is.  

## Motor springs:
These cheap stepper motors have quite a bit of play on the shaft, at the start I tought with the forces involved and the vibrations it would have been necessary to fit very stiff springs, turns out you don't need much.
I strongly suggest quick release skewer springs from bikes, they are very weak and they have the right dimensions, the larger side should be facing the motor. I've tested (and am currently running) much stiffer tower springs
and I did not notice any difference, I am a bit afraid they will wear out the motor quicker but I can't be bothered to change them back.  

## Possible alternative motors:
I've found a couple of alternative stepper motors that could fit this machine. With minor modifications you could even fit fit NEMA17 motors, this would require changing the drivers and code a bit tho. 
I've also found similar motors to these common 28byj-48 with all metal gears, those might be a good upgrade.

## Motor driver:
I'm using a relay with a big DC motor driver I had lying around but it's overkill and cluncky, one could probably fit a much cheaper and effective driver that could be piloted with the same pin as the relay and allow 
for the addition of a speed command, I will work on that in the future.

## Motor to leadscrew couplers:
I've tested a couple, I am currently mounting the clamp style ones but I think the ones with grubscrews would be better. I think the springy ones are not too good for this application I would avoid those. 

Example images of these couplers (source: AliExpress listings):
Springy couplers:  
<img width="327" height="357" alt="image" src="https://github.com/user-attachments/assets/a3d7c423-69aa-4aca-a680-8c9c03c1ebd0" />   
Clamp style coupler:  
<img width="350" height="262" alt="image" src="https://github.com/user-attachments/assets/b94d1003-45ec-4d18-9507-041596ff6196" />  
Recommended couplers:  
<img width="141" height="213" alt="image" src="https://github.com/user-attachments/assets/47ff873c-ac91-49c8-b371-64576c9fe848" />  


## Cutting bits:
There are a lot of different kinds of bits. I've found these to be the best (most reliable and durable):  
<img width="290" height="618" alt="image" src="https://github.com/user-attachments/assets/4f4e076d-0eb6-45f9-b9f0-b49677733c86" />


## Electronics:
I've tried a couple of different configurations; my CMS setup was the easiest to mount but pretty messy.  
I've also tested with making it with perfboard, it took me a lot of time to make, I don't really thinK it was worth it. If you still want to try this route the pcb I designed should be a good reference.  
I've also made a custom PCB, you could pay a service to make you one or, after mounting the machine with a different setup mount it yourself, I'll incluede a CMS module for this board in the files.  

In my design I've used 3 ULN2003; one could consider using only 2 but I did not want to push the current limits of the ICs and just decided to copy the cheap common boards. It is also quite improbable you have 3 motors and only 2 drivers.
