#include <msp430.h> 

//          --|RST          XOUT|-
//            |                 |
//            |     P1.2/UCA0TXD|------------>
//            |                 | 9600 - 8N1
//            |     P1.1/UCA0RXD|<------------
//
//   D. Dang
//   Texas Instruments Inc.
//   February 2011
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
#include  "msp430g2553.h"
#include "stdio.h"

void txUART_string(char *);

int sendString = 0;
int tickCount = 0;
unsigned int totalCount = 0;

//rfcomm bind rfcomm0 98:D3:31:80:1E:B8

char *message = "This is the message.";

void main(void) {
	// WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	WDTCTL = WDT_ADLY_1000;                   // WDT 250ms, ACLK, interval timer
	IE1 |= WDTIE;                             // Enable WDT interrupt
	P1DIR |= 0x01;                            // Set P1.0 to output direction

	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
	P1SEL = BIT1 + BIT2;                     // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2;                     // P1.1 = RXD, P1.2=TXD
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
//	UCA0BR0 = 104;                            // 1MHz 9600
	UCA0BR0 = 26;							// 1MHz 38400
	UCA0BR1 = 0;                              // 1MHz 9600 and 38400
	UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt

//	__bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
	__bis_SR_register(GIE);       // Enter LPM0, interrupts enabled

	for (;;) {
		char sbuf[20];

		if (sendString > 0) {
			sendString = 0;
			totalCount++;
			txUART_string(message);
			sprintf(sbuf, "  Count %u\r\n", totalCount);
			txUART_string(sbuf);
		}
	}
}

//------------------------------------------------------------------------------
// Transmit a string over the UART
//------------------------------------------------------------------------------
void txUART_string(char *string) {

	while (*string) {
		while (!(IFG2 & UCA0TXIFG))
			; // USCI_A0 TX buffer ready?

		UCA0TXBUF = *string++;
	}

}

//  Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
	while (!(IFG2 & UCA0TXIFG))
		;                // USCI_A0 TX buffer ready?
	UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
}

// Watchdog Timer interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void) {
	P1OUT ^= 0x01;                            // Toggle P1.0 using exclusive-OR
	/*
	 UCA0TXBUF = 'D';
	 while (!(IFG2 & UCA0TXIFG))
	 ;                // USCI_A0 TX buffer ready?
	 UCA0TXBUF = 'E';
	 */
	tickCount++;
	if (tickCount > 0) {
		tickCount = 0;
		sendString = 1;
	}
}

