ECE382_Lab6
===========

# Lab 6 - PWM - "Robot Motion"

# Purpose
  The point of this lab was to utilize pulse-width modulation features from the MSP430 in order to control the movement of my robot. By programming the MSP430 and generating pulse-width modulated waveforms, I was able to control both the speed and direction of the robot. The robot moved forward, backward, and did both small and large right and left turns. 
  

# Prelab
  Motor requires 12 v, and motor controller chip has 4 5v inputs and 4 12v outputs. I decided to use pins P2.0, P2.1, P2.3 and P2.5 to control the motors. Below is a schematic of how I wired the robot to the MSP430 chip. 
  
 ![alt text] (https://raw.github.com/CassieMcPeek/ECE382_Lab6/master/Lab6.jpg "Schematic")
  
  
  As shown in the schematic above, I used the four pins to control the two motors movements. 
  
  
      P2.0 controlled the right motor in reverse
      P2.1 controlled the right motor forward
      P2.3 controlled the left motor in reverse
      P2.5 controlled the left motor forward
  

I utilized PWM to control the voltage to the pins. I decided to use the reset/set mode for TACCTL1 and the set/reset mode for TACCTL2. Below is the code I determined would work best. I began with Dr. Coulston's code and integrated my designs into that. 

      
     P2DIR |= BIT2;							// P2.2 is associated with TACCTL1
     P2SEL |= BIT2;

     P2DIR |= BIT4;							// P2.4 is associated with TACCTL2
     P2SEL |= BIT4;

     TA1CTL = ID_3 | TASSEL_2 | MC_1;		//set duty cycle and MCLK
     TA1CCR0 = 100;

     TA1CCR1 = 50;
     TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

     TA1CCR2 = 50;						// set TACCTL2 to Set / Reset mode
     TA1CCTL2 = OUTMOD_3;


I also knew that the voltage from the regulator is 3.3v. 

# Required Functionality

Required functionality was a little challenging for me, because I didn't fully understand how to utilize the PWM to control the motors. I began with Dr. Coulston's code and tried to implement my own designs into that. At first, my design was to control the Left motor functions utilizing P1.0 and P1.1. This seemed like a logical design, until I began implementing it, and wiring the MSP430. I soon found that those pins did not produce the result I had intended, so I switched to P2.3 and P2.5 as shown in the schematic. 

I first wired up the robot to work for the code provided by Dr. Coulston. That took me a few tries, as I forgot to add a capacitor from the 12V source to ground, and also from the 5V source to ground. I also had wired the pin outputs to the wrong motors (i.e. I wired up the output for P2.3 to R BLK instead of L BLK). Once I made those fixes to my wiring, I was able to get the robot to work with Dr. Coulston's code. 

The next challenging part was taking the pieces of Dr. Coulson's code and fitting it to my design. After many attempts to create code that moved my robot, I finally determined that the code below turned the motors on in the forward directions.

     P2DIR |= BIT5;							// Left motor forward
     P2OUT &= ~BIT5;

     P2DIR |= BIT1;							// Right motor forward
     P2OUT &= ~BIT1;
     

Once I had that working, I figured out how to turn the motors off. I uesd a _delay_cycles() statement to leave the motors on for a short period of time before shutting them off with the following code. 

     P2DIR |= BIT5;						// Left motor stop forward
     P2OUT |= BIT5;

     P2DIR |= BIT1;						// Right motor stop forward
     P2OUT |= BIT1;
     

I tested this code mutliple times to ensure that it did in fact turn both of the motors on in the forward direction and then delayed it for as long as I had defined and then turned them off. Once I was confident in this, I moved on to the motors in the reverse direction. After some testing, I discovered that the movement in the backwards direction was the same code as above, just with the other two pins (P2.0 and P2.3). 

Once I had the motors moving in all directions and stopping, I moved onto implementing those movements in a sequence. I knew that I wanted to utilize a while() loop in order to run the sequence of movements. At first, I attempted to place all lines of code in the while() loop, but that soon became overwhelming and un-organized. C2C Dusty Weisner gave me the idea to create functions below the main code to define the movements of the motors and then call those subfunctions in the while loop. Once I had that idea, it was simple creating seperate functions for each movement and calling them in the while loop. 

The only part I had trouble with was doing a turn >45 degrees and <45 degrees. For this, I simple decresed the _delay_cycles() until the wheels turned less than 45 or greater than 45 degrees. These were placed into the functions below the main code as well. 

I demonstrated required functionality on Monday, 24 NOV 2014 at 1500 to Capt Trimble. 


# A Functionality

Once I understood the required functionality, the A functionality was pretty straight forward. I used the same remote as I did in Lab 5, so I just copied over the header file from that with no changes. For the main file, I kept the same declarations as before, which are shown below.

     WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

     P2DIR |= BIT2;							// P2.2 is associated with TACCTL1
     P2SEL |= BIT2;

     P2DIR |= BIT4;							// P2.4 is associated with TACCTL2
     P2SEL |= BIT4;

     TA1CTL = ID_3 | TASSEL_2 | MC_1;		//set duty cycle and MCLK
     TA1CCR0 = 100;

     TA1CCR1 = 50;
     TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

     TA1CCR2 = 50;						// set TACCTL2 to Set / Reset mode
     TA1CCTL2 = OUTMOD_3;
     

I also placed the subfunctions that I used in required functionality that controlled the movements at the bottom of the code. The rest was copied over from my Lab5.c file. The only change I needed to make was to call the subfunctions in the while loop to ensure that the remote buttons controlled the movements instead of it being in sequence. 

This code worked right away, but I did run into a few problems while testing. 



# Conclusion


# Documentation

