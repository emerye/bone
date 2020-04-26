/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
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
 * --/COPYRIGHT--*/
//******************************************************************************
//!
//! WDT - Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
//!
//!  Toggle P1.0 using software timed by WDT ISR. Toggle rate is exactly 250ms
//!  based on 32kHz ACLK WDT clock source. In this example the WDT is configured
//!  to divide 32768 watch-crystal(2^15) by 2^13 with an ISR triggered
//!  @ 4Hz = [WDT CLK source/32768].
//!  ACLK = REFO , MCLK = SMCLK = default DCO ~1.045MHz
//!
//!  Tested On: MSP430F5529,MSP430FR5739
//!             -----------------
//!         /|\|                 |
//!          | |                 |
//!          --|RST              |
//!            |             P1.0|-->LED
//!            |                 |
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - WDT peripheral
//! - GPIO Port peripheral
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - WDT_A_VECTOR
//!
//******************************************************************************
#include "driverlib.h"

#define COMMAND 0
#define DATA 1


void ToggleEnable() {
	 GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN0);
	 __delay_cycles(1000);
	 GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN0);
	 __delay_cycles(500);
}

void WriteByte(int cmdType, uint8_t data) {
	uint8_t highNibble = (data & 0xF0) >> 4;
	uint8_t lowNibble = data & 0x0F;

	if (cmdType) {
		GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5);
	} else {
		GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  //Command
	}

	P6OUT = highNibble;
	ToggleEnable();
	P6OUT = lowNibble;
	ToggleEnable();
}


void InitDisplay(void) {

	__delay_cycles(32 * 1000);
	__delay_cycles(32 * 1000);
	__delay_cycles(32 * 1000);

	GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN0 | GPIO_PIN1);
	GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN2 | GPIO_PIN3);
	GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  //Command
	ToggleEnable();
	__delay_cycles (15 * 1000);
	ToggleEnable();
	__delay_cycles (5 * 1000);
	ToggleEnable();
	__delay_cycles (5 * 1000);
	P6OUT = 0x02;
	ToggleEnable();
	__delay_cycles (5 * 1000);

	WriteByte(COMMAND, 0x20 | 0x08);  // Number of lines and font

	WriteByte(COMMAND, 0x08);   	// Display Off  Cursor OFF  Blink OFF

	WriteByte(COMMAND, 0x01);   // Clear Screen Cursor Home

	__delay_cycles(5000);

	WriteByte(COMMAND, 0x06);   //Inc cursor to right. Don't shift screen

	WriteByte(COMMAND, 0x0F);   //Display on

	WriteByte(DATA, 'A');

	WriteByte(DATA, 'B');

	WriteByte(DATA, 'C');

	WriteByte(DATA, 0x45);

	WriteByte(DATA, 0x45);

	WriteByte(DATA, 0x45);

	WriteByte(DATA, 'D');

	WriteByte(DATA, 'E');

	WriteByte(DATA, 'F');
	WriteByte(DATA, 'G');
	WriteByte(DATA, 'H');
	WriteByte(DATA, 'I');
	WriteByte(DATA, 'J');

	WriteByte(COMMAND, 0xC0);

	WriteByte(DATA, 'K');
	WriteByte(DATA, 'L');
	WriteByte(DATA, 'M');
	WriteByte(DATA, 'N');
	WriteByte(DATA, 'O');
	WriteByte(DATA, 'P');
	WriteByte(DATA, 'Q');

}

void main (void)
{
    //Initialize WDT module in timer interval mode,
    //with ACLK as source at an interval of 250 ms.
    WDT_A_initIntervalTimer(WDT_A_BASE,
        WDT_A_CLOCKSOURCE_ACLK,
        WDT_A_CLOCKDIVIDER_32K);   //1 second

   // WDT_A_CLOCKDIVIDER_512K);
  //  #define WDT_A_CLOCKDIVIDER_32K

	WDT_A_start(WDT_A_BASE);

    //Enable Watchdog Interupt
	SFR_clearInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
    SFR_enableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);

    //Set P1.0 to output direction
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN0
        );

    GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN0);   //LCD Enable  Pin 6
    GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN0);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);   //R/S Register Select  Pin 4
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);

    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN0);	//D4  Pin 11
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN0);

    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN1);	//D5  Pin 12
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN1);

    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN2);	//D6  Pin 13
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN2);

    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN3);	//D7  Pin 14
    GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN3);

    InitDisplay();


    //Enter LPM3, enable interrupts
    __bis_SR_register(LPM3_bits + GIE);
    //For debugger
    __no_operation();
}

//Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(WDT_VECTOR)))
#endif
void WDT_A_ISR (void)
{
    //Toggle P1.0
    GPIO_toggleOutputOnPin(
        GPIO_PORT_P1,
        GPIO_PIN0);
}

