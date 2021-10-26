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
//  MSP430G2xx3 Demo - USCI_B0 I2C Master TX single bytes to MSP430 Slave
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave. This is the master code. It continuously
//  transmits 00h, 01h, ..., 0ffh and demonstrates how to implement an I2C
//  master transmitter sending a single byte using the USCI_B0 TX interrupt.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.2MHz
//
//  *** to be used with "msp430g2xx3_uscib0_i2c_07.c" ***
//
//                                /|\  /|\
//               MSP430G2xx3      10k  10k     MSP430G2xx3
//                   slave         |    |        master
//             -----------------   |    |  -----------------
//           -|XIN  P1.7/UCB0SDA|<-|---+->|P1.7/UCB0SDA  XIN|-
//            |                 |  |      |                 |
//           -|XOUT             |  |      |             XOUT|-
//            |     P1.6/UCB0SCL|<-+----->|P1.6/UCB0SCL     |
//            |                 |         |                 |
//
//  D. Dang
//  Texas Instruments Inc.
//  February 2011
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <intrinsics.h>
#include "i2clcd.h"

volatile unsigned char TXData;
volatile unsigned char TXByteCtr;
volatile unsigned char activeRow;
volatile unsigned char activeCol;
volatile unsigned char pressedCol;
volatile unsigned char keyValid;

char buffer[10] = { 0 };

void initKeyPadIO(void);
void scanColumns(void);
void setColumnState(uint8_t, uint8_t);

const char keymap[] = { '1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9',
                        'C', '*', '0', '#', 'D' };
const char message[] = "0123456789ABCDEF";

int main(void)
{
    //Init WD Counter 128msec
    BCSCTL1 |= DIVA_3;                        // ACLK/8
    WDTCTL = WDT_ADLY_16;                     // WDT 16msec * 8 = 128msec
    IE1 |= WDTIE;                             // Enable WDT interrupt
    P1DIR = 0xFF;                             // All P1.x outputs
    P1OUT = 0;                                // All P1.x reset
    P2DIR = 0xFF;                             // All P2.x outputs
    P2OUT = 0;                                // All P2.x reset
    P3DIR = 0xFF;                             // All P3.x outputs
    P3OUT = 0;                                // All P3.x reset

    initKeyPadIO();

    //I2C init
    P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
    P1SEL2 |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
    UCB0CTL1 |= UCSWRST;                      // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
    UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;
    UCB0I2CSA = 0x27;                         // Slave Address is 027h
    UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
    IE2 |= UCB0TXIE;                          // Enable TX interrupt

    LCDinit();
    WriteString(0, 0, message);
    WriteString(1, 0, "     ");
    while (1)
    {
        scanColumns();
        if (keyValid)
        {
            sprintf(buffer, "%c", keymap[activeRow + pressedCol]);
            WriteString(1, 0, buffer);
            P1OUT &= ~BIT0;
            keyValid = 0;
        }
        __bis_SR_register(LPM3_bits | GIE);     // Re-enter LPM0
    }
}

void setColumnState(uint8_t colnum, uint8_t state)
{
    switch (colnum)
    {
        case 0:
        if(state)
        {
            P1OUT |= BIT1;
        }
        else
        {
            P1OUT &= ~BIT1;
        }
        break;
        case 1:
        if(state)
        {
            P1OUT |= BIT2;
        }
        else
        {
            P1OUT &= ~BIT2;
        }
        break;
        case 2:
        if(state)
        {
            P2OUT |= BIT0;
        }
        else
        {
            P2OUT &= ~BIT0;
        }
        break;
        case 3:
        if(state)
        {
            P2OUT |= BIT1;
        }
        else
        {
            P2OUT &= ~BIT1;
        }
        break;
    }
}


void scanColumns()
{
    //Col 0
    setColumnState(0, 1);
    activeCol = 0;
    __delay_cycles(10);
    setColumnState(0, 0);
    //Col 1
    setColumnState(1, 1);
    activeCol = 1;
    __delay_cycles(10);
    setColumnState(1, 0);;
    //Col 2
    setColumnState(2, 1);;
    activeCol = 2;
    __delay_cycles(10);
    setColumnState(2, 0);;
    //Col 3
    setColumnState(3, 1);
    activeCol = 3;
    __delay_cycles(10);
    setColumnState(3, 0);
}


void initKeyPadIO()
{
    //Columns as output
    P1OUT &= ~(BIT1 | BIT2);
    P1DIR |= BIT1 | BIT2;           //Set P1.1 and P1.2 as output.
    P2OUT &= ~(BIT0 | BIT1);
    P2DIR |= BIT0 | BIT1;          //Set P2.0 and P2.1 as output.

    //Rows as input with pulldown
    P2DIR &= ~(BIT2 | BIT3 | BIT4 | BIT5);   //Set Rows as input
    P2REN |= BIT2 | BIT3 | BIT4 | BIT5;      //Enable PU/PD
    P2OUT &= ~(BIT2 | BIT3 | BIT4 | BIT5);   //Pull down
    P2IES |= BIT2 | BIT3 | BIT4 | BIT5;    //Interrupt on high to low transition
    P2IE |= BIT2 | BIT3 | BIT4 | BIT5;      //Enable interrupt
    P2IFG &= ~(BIT2 | BIT3 | BIT4 | BIT5);  //Clear interrupt
}


// Port 2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) Port_2 (void)
#else
#error Compiler not supported!
#endif
{
    if (P2IFG & 0x04)
    {
        activeRow = 0;
        pressedCol = activeCol;         //Store column
        __disable_interrupt();
        P2IFG &= ~BIT2;                 //Clear interrupt bit
        setColumnState(pressedCol, 1);  //Set column high
        while((P2IN & 0x04) == 0x04);   //Loop until key depressed
        setColumnState(pressedCol, 0);  //Set column low
        keyValid = 1;                   //Set key valid flag
        P1OUT |= BIT0;                  //Turn on LED on board
        __enable_interrupt();           //enable interrupts
    }
    else if (P2IFG & 0x08)
    {
        activeRow = 4;
        pressedCol = activeCol;
        __disable_interrupt();
        P2IFG &= ~BIT3;
        setColumnState(pressedCol, 1);
        while((P2IN & 0x08) == 0x08);
        setColumnState(pressedCol, 0);
        keyValid = 1;
        P1OUT |= BIT0;
        __enable_interrupt();
    }
    else if (P2IFG & 0x10)
    {
        activeRow = 8;
        pressedCol = activeCol;
        __disable_interrupt();
        P2IFG &= ~BIT4;
        setColumnState(pressedCol, 1);
        while((P2IN & 0x10) == 0x10);
        setColumnState(pressedCol, 0);
        keyValid = 1;
        P1OUT |= BIT0;
        __enable_interrupt();
    }
    else if (P2IFG & 0x20)
    {
        activeRow = 12;
        pressedCol = activeCol;
        __disable_interrupt();
        P2IFG &= ~BIT5;
        setColumnState(pressedCol, 1);
        while((P2IN & BIT5) == BIT5);
        setColumnState(pressedCol, 0);
        keyValid = 1;
        P1OUT |= BIT0;
        __enable_interrupt();
    }
    P2IFG = 0;        //Clear interrupts
    __bic_SR_register_on_exit(LPM3_bits);
}

//------------------------------------------------------------------------------
// The USCIAB0TX_ISR is structured such that it can be used to transmit any
// number of bytes by pre-loading TXByteCtr with the byte count.
//------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCIAB0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if (TXByteCtr)                            // Check TX byte counter
    {
        UCB0TXBUF = TXData;                     // Load TX buffer
        TXByteCtr--;                            // Decrement TX byte counter
    }
    else
    {
        UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
        IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
      __bic_SR_register_on_exit(LPM3_bits);      // Exit LPM3
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(WDT_VECTOR))) watchdog_timer (void)
#else
#error Compiler not supported!
#endif
{
    __bic_SR_register_on_exit(LPM3_bits);   // Clear LPM3 bits from 0(SR)
}
