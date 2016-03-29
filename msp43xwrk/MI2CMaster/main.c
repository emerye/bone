//*******************************************************************************
//  MSP430F552x Demo - USCI_B0 I2C Master TX single bytes to MSP430 Slave
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave. This is the master code. It continuously
//  transmits 00h, 01h, ..., 0ffh and demonstrates how to implement an I2C
//  master transmitter sending a single byte using the USCI_B0 TX interrupt.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.045MHz
//
// ***to be used with "MSP430F55xx_uscib0_i2c_07.c" ***
//
//                                /|\  /|\
//                MSP430F5529     10k  10k     MSP430F5529
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//           -|XIN  P3.0/UCB0SDA|<-|----+->|P3.0/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//           -|XOUT             |  |       |             XOUT|-
//            |     P3.1/UCB0SCL|<-+------>|P3.1/UCB0SCL     |
//            |                 |          |                 |
//
// ******************************************************************************
//
#include  <msp430.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "lcdchar.h"

unsigned char TXData;
unsigned char TXByteCtr;
unsigned int  elapsedTime = 0;
unsigned int elapsedSeconds = 0;
unsigned long lifeTimeMinutes;
unsigned char updateDisplay = 0;

/**
 * Delay 1 msec with a 4MHz clock
 */
void DelayMsec(int delay) {
	volatile int dummy;
	int i, outcount;
	for (outcount = 0; outcount < delay; outcount++) {
		for (i = 0; i < 471; i++) {
			dummy++;
		}
	}
}

void SetupUART() {

	  P3SEL = BIT3+BIT4;                        // P3.4,5 = USCI_A0 TXD/RXD
	  UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
	  UCA0CTL1 |= UCSSEL_1;                     // CLK = ACLK
	  UCA0BR0 = 0x03;                           // 32kHz/9600=3.41 (see User's Guide)
	  UCA0BR1 = 0x00;                           //
	  UCA0MCTL = UCBRS_3+UCBRF_0;               // Modulation UCBRSx=3, UCBRFx=0
	  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	  UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
}


//4 MHz from crystal
void XT2_4MHz() {

	//Setup AClock from 32Khz Crystal
	P5SEL |= BIT4 + BIT5;                       // Select XT1

	UCSCTL6 &= ~(XT1OFF);                     // XT1 On
	UCSCTL6 |= XCAP_3;                        // Internal load cap
	UCSCTL3 = 0;                              // FLL Reference Clock = XT1
	//End

	P2DIR |= BIT2;                            // SMCLK set out to pins
	P2SEL |= BIT2;
	P7DIR |= BIT7;                            // MCLK set out to pins
	P7SEL |= BIT7;

	P5SEL |= BIT2 + BIT3;                       // Port select XT2

	UCSCTL6 &= ~XT2OFF;                       // Enable XT2
	UCSCTL3 |= SELREF_2;                      // FLLref = REFO
											  // Since LFXT1 is not used,
											  // sourcing FLL with LFXT1 can cause
											  // XT1OFFG flag to set
	UCSCTL4 |= SELA_2;                        // ACLK=REFO,SMCLK=DCO,MCLK=DCO

	// Loop until XT1,XT2 & DCO stabilizes - in this case loop until XT2 settles
	do {
		UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
		// Clear XT2,XT1,DCO fault flags
		SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	} while (SFRIFG1 & OFIFG);                   // Test oscillator fault flag

	UCSCTL6 &= ~XT2DRIVE0;                    // Decrease XT2 Drive according to
											  // expected frequency
	UCSCTL4 |= SELS_5 + SELM_5;               // SMCLK=MCLK=XT2
}

/**
 * Write one byte to I2C. Target address is globally set.
 *
 */
void WriteI2CByte(unsigned char data) {

	TXData = data;
	TXByteCtr = 1;                          // Load TX byte counter

	while (UCB0CTL1 & UCTXSTP)
		;             // Ensure stop condition got sent
	UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition

	__bis_SR_register(LPM0_bits + GIE);     // Enter LPM0 w/ interrupts
	__no_operation();                       // Remain in LPM0 until all data
											// is TX'd
}

/**
 * Write I2C Nibble
 */
void WriteI2CNibble(unsigned char msbtoWrite, int cmd) {

	unsigned char bytetoWrite = BACKLED;

	bytetoWrite = bytetoWrite | (msbtoWrite & 0xF0) | ENABLE | cmd;
	WriteI2CByte(bytetoWrite);
	bytetoWrite &= ~ENABLE;
	WriteI2CByte(bytetoWrite);
	bytetoWrite |= ENABLE;
	WriteI2CByte(bytetoWrite);
}

/**
 * Command = 0  Data = 1
 */
void WriteLCDByte(unsigned char bytetoWrite, int cmd) {
	unsigned char lower = (bytetoWrite << 4) & 0b11110000;
	unsigned char upper = bytetoWrite & 0b11110000;

	WriteI2CNibble(upper, cmd);
	WriteI2CNibble(lower, cmd);
}

/***
 * Initializes and sets up the display as described in the display data sheets.
 * This setup routine sets up a 4-bit display (sets 4-bit mode), sets the cursor,
 * turns on the display and sets entry mode.
 */
void Setup4bit() {
	DelayMsec(20);
	WriteI2CNibble(0x30, 0);  //Manual Command of Wake up!(first)
	DelayMsec(15);     //Sleep for at least 5ms
	WriteI2CNibble(0x30, 0);  //Toggle the E bit, sends on falling edge
	DelayMsec(1);    //Sleep for at least 160us
	WriteI2CNibble(0x30, 0);  //Manual Command of Wake up! (second)
	DelayMsec(1);    //Sleep for at least 160us
	WriteI2CNibble(0x20, 0);  //Set 4-bit mode 2 line display with 8 bit write.
	DelayMsec(1);
	WriteLCDByte(0x28, 0);  //Set 4-bit/2-line
	DelayMsec(3);
	WriteLCDByte(LCD_CLEAR_DISPLAY, 0);  //Clear Display
	DelayMsec(2);
//	WriteLCDByte(LCD_DISPLAY_ON_OFF | DISPLAY_ON | DISPLAY_CURSOR | DISPLAY_CURSOR_BLINK , 0);   //Set cursor visible
	DelayMsec(1);
	WriteLCDByte(LCD_DISPLAY_ON_OFF | DISPLAY_ON, 0);   //Set cursor visible
	DelayMsec(1);
	WriteLCDByte(LCD_ENTRY_MODE_SET | ENTRY_MODE_LEFT, 0);
	DelayMsec(1);
	WriteLCDByte(LCD_RETURN_HOME, 0);
	DelayMsec(3);
}

//Write a string to display
void WriteString(int row, int ypos, char message[]) {
	int stLength = strlen(message);
	int i, address;

	switch (row) {
	case 0:
		address = ypos;
		break;
	case 1:
		address = 0x40 + ypos;
		break;
	case 2:
		address = 20 + ypos;
		break;
	case 3:
		address = 0x54 + ypos;
		break;
	}
	address += 0x80;
	WriteLCDByte((unsigned char) address, 0);
	for (i = 0; i < stLength; i++) {
		if (message[i] > 0x1f) {
			WriteLCDByte(message[i], 1);
		}
	}
}


/**
 * Write one long integer to flash segment D
 */
void WriteFlash(unsigned long ldata)
{
  unsigned long * Flash_ptrD;               // Initialize Flash pointer Seg D
  unsigned long value;
  WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

  Flash_ptrD = (unsigned long *) 0x1800;    // Initialize Flash pointer
  value = 0x12345678;                       // Initialize Value
  FCTL3 = FWKEY;                            // Clear Lock bit
  FCTL1 = FWKEY+ERASE;                      // Set Erase bit
  *Flash_ptrD = 0;                          // Dummy write to erase Flash seg
  FCTL1 = FWKEY+BLKWRT;                     // Enable long-word write
  *Flash_ptrD = ldata;                      // Write to Flash
  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY+LOCK;                       // Set LOCK bit
  DelayMsec(100);
}


unsigned long ReadFlashWord() {
	unsigned long * FlashPtr;

	FlashPtr = (unsigned long *) 0x1800;    // Initialize Flash pointer
	return *FlashPtr;
}

int main(void) {

	char timeBuffer[30] = {0};
	char lifeBuffer[30] = {0};
	unsigned long *FlashPtr;
	unsigned long lValue;

	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

	P1DIR |= 0x01;                            // P1.0 output
	TA1CTL = TASSEL_1 + MC_2 + TACLR + TAIE;  // ACLK, contmode, clear TAR

	XT2_4MHz();
	SetupUART();
	P3SEL |= 0x03;                            // Assign I2C pins to USCI_B0
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
	UCB0BR0 = 40;                             // fSCL = SMCLK/40 = ~100kHz
	UCB0BR1 = 0;
	UCB0I2CSA = 0x27;                         // Slave Address
	UCB0CTL1 &= ~UCSWRST;                    // Clear SW reset, resume operation
	UCB0IE |= UCTXIE;                         // Enable TX interrupt

	TXData = 0x01;                            // Holds TX data
	Setup4bit();

	lifeTimeMinutes = ReadFlashWord(); 			//Read lifetime minutes from flash

	WriteLCDByte('A', 1);
	DelayMsec(3);
	WriteLCDByte('B', 1);

	sprintf(timeBuffer, "%3d minutes elapsed", 0);
	sprintf(lifeBuffer, "%3lu hours lifetime", lifeTimeMinutes/60);
	WriteString(0,0,timeBuffer);
	WriteString(1,0,lifeBuffer);

//	WriteFlash(0x0);
//	FlashPtr = (unsigned long *) 0x1800;    // Initialize Flash pointer

	lValue = ReadFlashWord();

	 __bis_SR_register(GIE);       //enable interrupts

	while(1) {
		if (updateDisplay > 0) {
			updateDisplay = 0;
			lifeTimeMinutes += 1;
			WriteFlash(lifeTimeMinutes);
			sprintf(timeBuffer, "%3d minutes elapsed", elapsedTime);
			sprintf(lifeBuffer, "%3lu hours lifetime", lifeTimeMinutes/60);
			WriteString(0,0,timeBuffer);
			WriteString(1,0,lifeBuffer);
  //  		 __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
		}
	}
//	 __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3, enable interrupts
}


// Timer1_A3 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) TIMER1_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(TA1IV, 14)) {
	case 0:
		break;                          // No interrupt
	case 2:
		break;                          // CCR1 not used
	case 4:
		break;                          // CCR2 not used
	case 6:
		break;                          // reserved
	case 8:
		break;                          // reserved
	case 10:
		break;                          // reserved
	case 12:
		break;                          // reserved
	case 14:
		P1OUT ^= 0x01;                  // overflow
		elapsedSeconds++;
		if (elapsedSeconds == 30) {
			elapsedTime++;
			updateDisplay=1;
			elapsedSeconds = 0;
		}
		break;
	default:
		break;
	}
}


// Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCA0IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA0IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    UCA0TXBUF = UCA0RXBUF;                  // TX -> RXed character
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}


//------------------------------------------------------------------------------
// The USCIAB0_ISR is structured such that it can be used to transmit any
// number of bytes by pre-loading TXByteCtr with the byte count.
//------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(UCB0IV, 12)) {
	case 0:
		break;                           // Vector  0: No interrupts
	case 2:
		break;                           // Vector  2: ALIFG
	case 4:
		break;                           // Vector  4: NACKIFG
	case 6:
		break;                           // Vector  6: STTIFG
	case 8:
		break;                           // Vector  8: STPIFG
	case 10:
		break;                           // Vector 10: RXIFG
	case 12:                                  // Vector 12: TXIFG
		if (TXByteCtr)                          // Check TX byte counter
		{
			UCB0TXBUF = TXData;                   // Load TX buffer
			TXByteCtr--;                          // Decrement TX byte counter
		} else {
			UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
			UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
			__bic_SR_register_on_exit(LPM0_bits); // Exit LPM0
		}
		break;
	default:
		break;
	}
}
