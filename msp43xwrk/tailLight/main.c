#include <msp430.h>
#include <stdio.h>


//******************************************************************************
//  MSP430G2xx2 Demo - WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
//
//  Description: Toggle P1.0 using software timed by WDT ISR. Toggle rate is
//  exactly 250ms based on 32kHz ACLK WDT clock source.  In this example the
//  WDT is configured to divide 32768 watch-crystal(2^15) by 2^13 with an ISR
//  triggered @ 4Hz.
//  ACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//
//
//		  MSP430G2xx2
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//
//******************************************************************************
/*
 * Left turn signal input P2 Bit 3
 * Right turn signal input P2 Bit 4
 *
 * P2.2 Running
 * P2.1 Left Turn
 * P2.0 Right Turn
 */

#include <msp430g2553.h>

#define LEFTTURNIN (0x0008)
#define RIGHTTURNIN (0x0010)

enum states {
	RUNNING,
	LEFTTURN,
	RIGHTTURN,
	BRAKE
};
enum states state;

unsigned char clkCount = 0;

void main(void)
{

  WDTCTL = WDT_ADLY_16;                    // WDT 250ms, ACLK, interval timer
  IE1 |= WDTIE;                             // Enable WDT interrupt
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  P2DIR = (BIT0 | BIT1 | BIT2) & 0x07;		// Port Direction

  P2OUT = BIT2 | BIT3 | BIT4 | BIT5;     	//Inputs pullup enabled.
  P2REN = BIT3 | BIT4 | BIT5;				//Enable pullups

  _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/interrupt
}


// Watchdog Timer interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void) {
	P1OUT ^= 0x01;                            // Toggle P1.0 using exclusive-O

	if ((P2IN & LEFTTURNIN) == 0) {
		state = LEFTTURN;
		clkCount += 1;
	} else if ((P2IN & RIGHTTURNIN) == 0) {
		state = RIGHTTURN;
		clkCount += 1;
	} else {
		state = RUNNING;
		clkCount += 3;
	}

	if (clkCount > 16) {
		clkCount = 0;
		switch (state) {
		case RUNNING:
			P2OUT &= ~P2OUT | ~(BIT0 | BIT1);
			P2OUT ^= BIT2;
			break;
		case LEFTTURN:
			P2OUT &= ~P2OUT | ~BIT2;
			P2OUT ^= BIT0;
			break;
		case RIGHTTURN:
			P2OUT &= ~P2OUT | ~(BIT2 | BIT0);
			P2OUT ^= BIT1;
			break;
		}
	}
  //P2OUT ^= 0x07;
}

