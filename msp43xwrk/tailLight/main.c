#include <msp430.h>				


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
//  D. Dang
//  Texas Instruments Inc.
//  December 2010
//  Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************

#include <msp430g2553.h>
unsigned char runCount = 0;

void main(void)
{

  WDTCTL = WDT_ADLY_250;                    // WDT 250ms, ACLK, interval timer
  IE1 |= WDTIE;                             // Enable WDT interrupt
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  P2DIR = (BIT0 | BIT1 | BIT2) & 0x07;		// Port Direction


  P2OUT = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5;
  P2REN = 0x38;
  __delay_cycles(1);
  _BIS_SR(LPM3_bits + GIE);                 // Enter LPM3 w/interrupt
}


// Watchdog Timer interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
  P1OUT ^= 0x01;                            // Toggle P1.0 using exclusive-O

  /*
  if (runCount < 3) {
	  P2OUT |= BIT2;
	  runCount += 1;
  } else {
	  P2OUT &= ~BIT2;
	  if(runCount > 5) {
		  runCount = 0;
	  }
	  runCount =+1 ;
  }
  */
  P2OUT ^= 0x07;
}

