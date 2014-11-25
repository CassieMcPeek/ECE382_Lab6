/*
 * main.c
 *
 *  Created on: Nov 23, 2014
 *      Author: C15Cassandra.McPeek
 */

#include <msp430.h>

void main(void) {
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


    while (1) {

    	moveForward();							//call function to move forward
    	_delay_cycles(1000000);
    	stopMovement();
    	_delay_cycles(1000000);

    	turnLeftLittle();						// call function to turn left less than 45 degrees
    	_delay_cycles(1000000);

//		moveForward();
//    	_delay_cycles(1000000);
//    	stopMovement();
//    	_delay_cycles(1000000);

    	turnRightLittle();						// call function to turn right less than 45 degrees
    	_delay_cycles(1000000);

//    	moveForward();
//    	_delay_cycles(1000000);
//    	stopMovement();
//    	_delay_cycles(1000000);

    	moveBackward();							// call function to move backwards
    	_delay_cycles(1000000);
    	stopMovement();
    	_delay_cycles(1000000);

    	turnLeftBig();							// call function to turn left greater than 45 degrees
    	_delay_cycles(1000000);

//    	moveForward();
//    	_delay_cycles(1000000);
//    	stopMovement();
//    	_delay_cycles(1000000);

    	turnRightBig();							// call function to turn right greater than 45 degrees
    	_delay_cycles(1000000);
    }
}

// -----------------------------------------------------------------------
// Each of the functions below correspond to the proper bits that control the movement
// of the robot. The functions for turning the rebot include the delay cycles that correspond to
// turns either less than 45 degrees, or greater than 45 degrees.
// -----------------------------------------------------------------------

void moveForward() {

    P2DIR |= BIT5;							// Left motor forward
    P2OUT &= ~BIT5;

    P2DIR |= BIT1;							// Right motor forward
    P2OUT &= ~BIT1;
}

void stopMovement(){

    P2DIR |= BIT5;						// Left motor stop forward
    P2OUT |= BIT5;

    P2DIR |= BIT1;						// Right motor stop forward
    P2OUT |= BIT1;

    P2DIR |= BIT0;						// Right motor stop reverse
    P2OUT |= BIT0;

    P2DIR |= BIT3;						// Left motor stop forward
    P2OUT |= BIT3;
}

void turnLeftLittle(){

	P2DIR |= BIT1;							// Right motor forward
	P2OUT &= ~BIT1;
	_delay_cycles(10000000/80);				// implements turn less than 45 degrees
	stopMovement();

}

void turnRightLittle(){

	P2DIR |= BIT5;							// Left motor forward
	P2OUT &= ~BIT5;
	_delay_cycles(10000000/80);				// implements turn less than 45 degrees
    stopMovement();

}

void turnLeftBig(){

	P2DIR |= BIT1;							// Right motor forward
	P2OUT &= ~BIT1;
	_delay_cycles(10000000/40);				// implements turn greater than 45 degrees
   	stopMovement();

}

void turnRightBig(){

	P2DIR |= BIT5;							// Left motor forward
	P2OUT &= ~BIT5;
	_delay_cycles(10000000/40);				// implements turn greater than 45 degrees
   	stopMovement();
}

void moveBackward(){

    P2DIR |= BIT3;							// Left motor reverse
    P2OUT &= ~BIT3;

    P2DIR |= BIT0;							// Right motor reverse
    P2OUT &= ~BIT0;

}

