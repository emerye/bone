/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430G2xx3 Demo - P1 Interrupt from LPM4 with Internal Pull-up
//
//  Description: A hi/low transition on P1.4 will trigger P1_ISR which,
//  toggles P1.0. Normal mode is LPM4 ~ 0.1uA.
//  Internal pullup enabled on P1.4.
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//               MSP430G2xx3
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//     /|\   |      R          |
//      --o--| P1.4-o      P1.0|-->LED
//     \|/
//
//  D. Dang
//  Texas Instruments Inc.
//  December 2010
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************

#include <msp430.h>

volatile unsigned int tickCount = 0;
volatile unsigned int startEvent = 0;
volatile unsigned int delayStart = 0;


int main(void)
{
  WDTCTL = WDT_ADLY_1_9;								// WDT 1.9 msec interval timer
  IE1 |= WDTIE;                             // Enable WDT interrupt
 // WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
  P1DIR = 0x01;                             // P1.0 output  Red LED
  P1DIR |= 0x40; 							// P1.6 output  Green LED
  P1OUT |= 0x41;							// Green LED on

  P2DIR |= 0x08;							//P2.3 Control Output
  P2OUT &= 0xF7;							//P2.3 Output High


  P1OUT =  0x10;                            // P1.4 set, else reset
  P1REN |= 0x10;                            // P1.4 pullup
  P1IE |= 0x10;                             // P1.4 interrupt enabled
  P1IES |= 0x10;                            // P1.4 Hi to low edge triggered
  P1IFG &= ~0x10;                           // P1.4 IFG cleared

  _BIS_SR(GIE);
  while(1) {
	  if (startEvent == 1)
	  {
		  P1IE &= 0xEF;                             // P1.4 disable interrupt
		  delayStart = tickCount;
		  P2OUT &= 0xF7; 			//Set control line low  (ON)
		//  while (tickCount != delayStart + 10);   //25 id too high
		  while (tickCount != delayStart + 15);   //15 is too high on 3-21-2019
		    P2OUT |= 0x08;			//Set control line high (OFF)
		    P1OUT &= 0xFE;			//Red LED Off
		  startEvent = 0;
		  while (tickCount != delayStart + 1000);
		  P1IFG &= ~0x10;                           // P1.4 IFG cleared
		  P1IE |= 0x10;                             // P1.4 interrupt enabled
	  }

  }
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  P1OUT |= 0x01;                            // P1.0 = toggle
  startEvent = 1;
  P1IE &= 0xEF;                             // P1.4 disable interrupt
  P1IFG &= ~0x10;                           // P1.4 IFG cleared
}


#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer (void)
{
    _BIC_SR_IRQ(LPM3_bits);                 // Clear LPM3 bits from 0(SR)
    tickCount += 1;
    P1OUT ^= 0x40;

}
