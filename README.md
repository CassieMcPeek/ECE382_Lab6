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

The next challenging part was taking the pieces of Dr. Coulson's code and fitting it to my design. 

# A Functionality


# Conclusion


# Documentation

