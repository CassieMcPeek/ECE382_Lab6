/*
 * A_Funct.c
 *
 *  Created on: Nov 24, 2014
 *      Author: C15Cassandra.McPeek
 */

#include <msp430.h>
#include "A_Funct.h"

int8	newIrPacket = FALSE;
int16	packetData[40];
int16	bitString;
int8	packetIndex2 = 0;
int8	packetIndex = 0;

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

    initMSP430();				// Setup MSP to process IR and buttons

    	_enable_interrupt();	//enable interrupt for IR

    	while(1)  {
    		packetIndex2 = 0;
    		if (newIrPacket)
    		{
    			_disable_interrupt();
    			while ((packetData[packetIndex2] != 2) && (packetIndex2 < 40)){
    				packetIndex2++;
    			}
    			int length = 0;

    			while(length < 16){
    				bitString += packetData[packetIndex2++];
    				bitString <<= 1;
    				length++;
    			}

    				if(bitString == CH_UP){				// if statements for movement of robot using buttons on remote
    					moveForward();
    				}

    				if(bitString == CH_DW){
    					stopMovement();
    				}

    				if(bitString == ONE){
    					moveBackward();
    				}

    				if(bitString == TWO){
   						turnRightBig();
    				}

    				if(bitString == THR){
    					turnLeftBig();
    				}

    				if(bitString == PWR){
    					turnRightLittle();
    				}

    				bitString = 0;
    				packetIndex = 0;
    				newIrPacket = FALSE;

    				int i;
    				for(i = 0; i < 0xFFFF; i++);
    		}
    		_enable_interrupt();

    		}
    	}


// -----------------------------------------------------------------------
// In order to decode IR packets, the MSP430 needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The P2.6 pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// The timer must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void initMSP430() {

	IFG1=0; 					// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 		// stop WD

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	P2SEL  &= ~BIT6;						// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	HIGH_2_LOW;
	P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
	P1OUT &= ~(BIT0 | BIT6);			// An turn the LED off

	TA0CCR0 = 0x8000;					// create a 16mS roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts

	_enable_interrupt();
}

// -----------------------------------------------------------------------
// Since the IR decoder is connected to P2.6, we want an interrupt
// to occur every time that the pin changes - this will occur on
// a positive edge and a negative edge.
//
// Negative Edge:
// The negative edge is associated with end of the logic 1 half-bit and
// the start of the logic 0 half of the bit.  The timer contains the
// duration of the logic 1 pulse, so we'll pull that out, process it
// and store the bit in the global irPacket variable. Going forward there
// is really nothing interesting that happens in this period, because all
// the logic 0 half-bits have the same period.  So we will turn off
// the timer interrupts and wait for the next (positive) edge on P2.6
//
// Positive Edge:
// The positive edge is associated with the end of the logic 0 half-bit
// and the start of the logic 1 half-bit.  There is nothing to do in
// terms of the logic 0 half bit because it does not encode any useful
// information.  On the other hand, we going into the logic 1 half of the bit
// and the portion which determines the bit value, the start of the
// packet, or if the timer rolls over, the end of the ir packet.
// Since the duration of this half-bit determines the outcome
// we will turn on the timer and its associated interrupt.
// -----------------------------------------------------------------------
#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file

__interrupt void pinChange (void) {

	int8	pin;
	int16	pulseDuration;			// The timer is 16-bits

	if (IR_PIN)		pin=1;	else pin=0;

	switch (pin) {					// read the current pin level
		case 0:						// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
			pulseDuration = TAR;

			if((pulseDuration < maxStartPulse) && (pulseDuration > minStartPulse)){
				pulseDuration = 2;
			}
			else if ((pulseDuration < maxLogic1Pulse) && (pulseDuration > minLogic1Pulse)){
				pulseDuration = 1;
			}
			else if ((pulseDuration < maxLogic0Pulse) && (pulseDuration > minLogic0Pulse)){
				pulseDuration = 0;
			}
			packetData[packetIndex++] = pulseDuration;
			LOW_2_HIGH; 				// Setup pin interrupt on positive edge
			break;

		case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
			TAR = 0x0000;						// time measurements are based at time 0
			HIGH_2_LOW; 						// Setup pin interrupr on positive edge
			break;
	} // end switch

	if (packetIndex > 40){
		newIrPacket = TRUE;
	}

	P2IFG &= ~BIT6;			// Clear the interrupt flag to prevent immediate ISR re-entry

} // end pinChange ISR



// -----------------------------------------------------------------------
//			0 half-bit	1 half-bit		TIMER A COUNTS		TIMER A COUNTS
//	Logic 0	xxx
//	Logic 1
//	Start
//	End
//
// -----------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
__interrupt void timerOverflow (void) {

	TACTL &= ~TAIFG;
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

	_delay_cycles(10000000);
	stopMovement();
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
	_delay_cycles(10000000/80);
	stopMovement();

}

void turnRightLittle(){

	P2DIR |= BIT5;							// Left motor forward
	P2OUT &= ~BIT5;
	_delay_cycles(10000000/80);
    stopMovement();

}

void turnLeftBig(){

	P2DIR |= BIT1;							// Right motor forward
	P2OUT &= ~BIT1;
	_delay_cycles(1000000);
   	stopMovement();

}

void turnRightBig(){

	P2DIR |= BIT5;							// Left motor forward
	P2OUT &= ~BIT5;
	_delay_cycles(1000000);
   	stopMovement();
}

void moveBackward(){

    P2DIR |= BIT3;							// Left motor reverse
    P2OUT &= ~BIT3;

    P2DIR |= BIT0;							// Right motor reverse
    P2OUT &= ~BIT0;

	_delay_cycles(10000000);
	stopMovement();
}




