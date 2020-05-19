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
#include <stdio.h>
#include <string.h>
#include "driverlib.h"

#define COMMAND 0
#define DATA 1

#define   Num_of_Results   16

volatile uint16_t A0results[Num_of_Results];
volatile uint16_t A1results[Num_of_Results];
volatile uint16_t A2results[Num_of_Results];
volatile uint16_t A3results[Num_of_Results];

volatile uint16_t A0ADCcnt, A1ADCcnt, A2ADCcnt, A3ADCcnt;

volatile bool adcReadingReady = false;

void toggleEnable() {
	GPIO_setOutputHighOnPin(GPIO_PORT_P7, GPIO_PIN0);
	__delay_cycles(1000);
	GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN0);
	__delay_cycles(500);
}

void writeByte(int cmdType, uint8_t data) {
	uint8_t highNibble = (data >> 4) & 0x0F;
	uint8_t lowNibble = data & 0x0F;
	uint8_t p1State, outBits;

	if (cmdType) {
		GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5);
	} else {
		GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  //Command
	}

	//High nibble
	p1State = P1IN;
	outBits = (p1State & 0xC3) | (highNibble << 2);
	P1OUT = outBits;
	toggleEnable();
	//Low nibble
	p1State = outBits;
	outBits = (p1State & 0xC3) | (lowNibble << 2);
	P1OUT = outBits;
	toggleEnable();
}


void writeString(uint8_t address, char *message) {
	writeByte(COMMAND, 0x80 | address);

	__delay_cycles(100);
	while (*message != 0) {
		writeByte(DATA, *message++);
	}
}


void initADCMulti(void) {

	//Enable A/D channel inputs
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
	GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
	GPIO_PIN3 + GPIO_PIN4 + GPIO_PIN5 +
	GPIO_PIN6 + GPIO_PIN7);

	//Initialize the ADC12_A Module
	/*
	 * Base address of ADC12_A Module
	 * Use internal ADC12_A bit as sample/hold signal to start conversion
	 * USE MODOSC 5MHZ Digital Oscillator as clock source
	 * Use default clock divider of 1
	 */
	ADC12_A_init(ADC12_A_BASE,
	ADC12_A_SAMPLEHOLDSOURCE_SC,
	ADC12_A_CLOCKSOURCE_ADC12OSC,
    ADC12_A_CLOCKDIVIDER_1);

	ADC12_A_enable(ADC12_A_BASE);

	/*
	 * Base address of ADC12_A Module
	 * For memory buffers 0-7 sample/hold for 256 clock cycles
	 * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
	 * Enable Multiple Sampling
	 */
	ADC12_A_setupSamplingTimer(ADC12_A_BASE,
	ADC12_A_CYCLEHOLD_256_CYCLES,
	ADC12_A_CYCLEHOLD_4_CYCLES,
	ADC12_A_MULTIPLESAMPLESENABLE);

	//Configure Memory Buffers
	/*
	 * Base address of the ADC12_A Module
	 * Configure memory buffer 0
	 * Map input A0 to memory buffer 0
	 * Vref+ = AVcc
	 * Vref- = AVss
	 * Memory buffer 0 is not the end of a sequence
	 */
	ADC12_A_configureMemoryParam param0 = { 0 };
	param0.memoryBufferControlIndex = ADC12_A_MEMORY_0;
	param0.inputSourceSelect = ADC12_A_INPUT_A0;
	param0.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_INT;
	param0.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
	param0.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
	ADC12_A_configureMemory(ADC12_A_BASE, &param0);

	/*
	 * Base address of the ADC12_A Module
	 * Configure memory buffer 1
	 * Map input A1 to memory buffer 1
	 * Vref+ = AVcc
	 * Vref- = AVss
	 * Memory buffer 1 is not the end of a sequence
	 *
	 */
	ADC12_A_configureMemoryParam param1 = { 0 };
	param1.memoryBufferControlIndex = ADC12_A_MEMORY_1;
	param1.inputSourceSelect = ADC12_A_INPUT_A1;
	param1.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_INT;
	param1.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
	param1.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
	ADC12_A_configureMemory(ADC12_A_BASE, &param1);
	/*
	 * Base address of the ADC12_A Module
	 * Configure memory buffer 2
	 * Map input A2 to memory buffer 2
	 * Vref+ = AVcc
	 * Vref- = AVss
	 * Memory buffer 2 is not the end of a sequence
	 */
	ADC12_A_configureMemoryParam param2 = { 0 };
	param2.memoryBufferControlIndex = ADC12_A_MEMORY_2;
	param2.inputSourceSelect = ADC12_A_INPUT_A2;
	param2.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_INT;
	param2.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
	param2.endOfSequence = ADC12_A_NOTENDOFSEQUENCE;
	ADC12_A_configureMemory(ADC12_A_BASE, &param2);
	/*
	 * Base address of the ADC12_A Module
	 * Configure memory buffer 3
	 * Map input A3 to memory buffer 3
	 * Vr+ = AVcc
	 * Vr- = AVss
	 * Memory buffer 3 IS the end of a sequence
	 */
	ADC12_A_configureMemoryParam param3 = { 0 };
	param3.memoryBufferControlIndex = ADC12_A_MEMORY_3;
	param3.inputSourceSelect = ADC12_A_INPUT_A3;
	param3.positiveRefVoltageSourceSelect = ADC12_A_VREFPOS_INT;
	param3.negativeRefVoltageSourceSelect = ADC12_A_VREFNEG_AVSS;
	param3.endOfSequence = ADC12_A_ENDOFSEQUENCE;
	ADC12_A_configureMemory(ADC12_A_BASE, &param3);

	//Configure internal reference
	//If ref generator busy, WAIT
	while ( REF_ACTIVE == Ref_isRefGenBusy(REF_BASE))
		;
	//Select internal ref = 1.5V
	Ref_setReferenceVoltage(REF_BASE,
	REF_VREF1_5V);
	//Internal Reference ON
	Ref_enableReferenceVoltage(REF_BASE);

	//Delay (~75us) for Ref to settle
	__delay_cycles(75);
}

void InitDisplay(void) {

	__delay_cycles(32 * 1000);
	__delay_cycles(32 * 1000);
	__delay_cycles(32 * 1000);

	GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3);
	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4 | GPIO_PIN5);
	GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  //Command
	toggleEnable();
	__delay_cycles(15 * 1000);
	toggleEnable();
	__delay_cycles(5 * 1000);
	toggleEnable();
	__delay_cycles(5 * 1000);

	P1OUT = 0x08;
	toggleEnable();
	__delay_cycles(5 * 1000);

	writeByte(COMMAND, 0x20 | 0x08);  // Number of lines and font

	writeByte(COMMAND, 0x01);   // Clear Screen Cursor Home
	__delay_cycles(2000);

	writeByte(COMMAND, 0x08);   	// Display Off  Cursor OFF  Blink OFF
	__delay_cycles(100);

	//writeByte(COMMAND, 0x01);   // Clear Screen Cursor Home

	__delay_cycles(5000);

	writeByte(COMMAND, 0x06);   //Inc cursor to right. Don't shift screen
	__delay_cycles(100);

	writeByte(COMMAND, 0x0C);   //Display on
	__delay_cycles(1000);


	writeByte(DATA, 'A');
	writeByte(DATA, 'B');
	writeByte(DATA, 'C');
	writeByte(DATA, 0x45);
	writeByte(DATA, 0x45);
	writeByte(DATA, 0x45);
	writeByte(DATA, 'D');
	writeByte(DATA, 'E');
	writeByte(DATA, 'F');
	writeByte(DATA, 'G');
	writeByte(DATA, 'H');
	writeByte(DATA, 'I');
	writeByte(DATA, 'J');

	writeByte(COMMAND, 0xC0);

	writeByte(DATA, 'K');
	writeByte(DATA, 'L');
	writeString(15, "1234567890ABCDEFGHIJKLMNOPQRSTUVXYZ");
	writeString(64, "1234567890abcdefghijklmnop");
}


void main(void) {


	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	//Initialize WDT module in timer interval mode,
	//with ACLK as source at an interval of 1 second.
	WDT_A_initIntervalTimer(WDT_A_BASE,
	WDT_A_CLOCKSOURCE_ACLK,
	WDT_A_CLOCKDIVIDER_32K);   //1 second

	WDT_A_start(WDT_A_BASE);

	//Enable Watchdog Interupt
	SFR_clearInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);
	SFR_enableInterrupt(SFR_WATCHDOG_INTERVAL_TIMER_INTERRUPT);

	//Set P1.0 to output direction
	GPIO_setAsOutputPin(
	GPIO_PORT_P1,
	GPIO_PIN0);

	GPIO_setAsOutputPin(GPIO_PORT_P7, GPIO_PIN0);   //LCD Enable  Pin 6
	GPIO_setOutputLowOnPin(GPIO_PORT_P7, GPIO_PIN0);

	GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);   //R/S Register Select  Pin 4
	GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);

	GPIO_setAsOutputPin(GPIO_PORT_P1,
			GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5);	//D4  LCD Pin 11
	GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3); //D4 and D5 High
	GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4 | GPIO_PIN5); //D6 and D7 Low

	InitDisplay();
	initADCMulti();

	//Enable memory buffer 3 interrupt
	ADC12_A_clearInterrupt(ADC12_A_BASE,
	ADC12IFG3);
	ADC12_A_enableInterrupt(ADC12_A_BASE,
	ADC12IE3);

	//Enable/Start first sampling and conversion cycle
	/*
	 * Base address of ADC12_A Module
	 * Start the conversion into memory buffer 0
	 * Use the repeated sequence of channels
	 */
	ADC12_A_startConversion(ADC12_A_BASE,
	ADC12_A_MEMORY_0,
	ADC12_A_REPEATED_SEQOFCHANNELS);

	//Clear Screen
	writeByte(COMMAND, 0x01);

	//Enter LPM0, Enable interrupts
	__bis_SR_register(LPM0_bits + GIE);
	//     __bis_SR_register(GIE);

}

//Watchdog Timer interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=WDT_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(WDT_VECTOR)))
#endif
void WDT_A_ISR(void) {
	char buffer[10];
	int i;
	float r1 = 2.7E6;
	float r2 = 68.0E3;
	float gain, absVoltage;
	int displayOffset;

	//Toggle P1.0
	GPIO_toggleOutputOnPin(
	GPIO_PORT_P1,
	GPIO_PIN0);
	if (adcReadingReady == true) {
		A0ADCcnt = A1ADCcnt = A2ADCcnt = A3ADCcnt = 0;
		for (i = 0; i < Num_of_Results; i++) {
			A0ADCcnt += A0results[i];
			A1ADCcnt += A1results[i];
			A2ADCcnt += A2results[i];
			A3ADCcnt += A3results[i];
		}
		A0ADCcnt = A0ADCcnt / Num_of_Results;
		A1ADCcnt = A1ADCcnt / Num_of_Results;
		A2ADCcnt = A2ADCcnt / Num_of_Results;
		A3ADCcnt = A3ADCcnt / Num_of_Results;

		gain = r2 / (r1 + r2);
		gain = 1.00 / gain;

		//Cell 9
		r2 = 68.5E3;
		r1 = 2.721E6;
		gain = r2 / (r1 + r2);
		gain = 1.00 / gain;
		absVoltage = ((float) A1ADCcnt / (float) 4096) * 1.50;
		sprintf(buffer, "%.3f", absVoltage);
		displayOffset = 0;
		writeString(displayOffset, "C9 ");
		writeString(displayOffset + 4, buffer);
		writeString(displayOffset + 10, "  ");
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%.3f", absVoltage * gain);
		writeString(displayOffset + 11, buffer);

		//Cell 10
		r2 = 69.4E3;
		r1 = 2.696E6;
		gain = r2 / (r1 + r2);
		gain = 1.00 / gain;
		absVoltage = ((float) A2ADCcnt / (float) 4096) * 1.50;
		sprintf(buffer, "%.3f", absVoltage);
		displayOffset = 20;
		writeString(displayOffset, "C10");
		writeString(displayOffset + 4, buffer);
		writeString(displayOffset + 10, "  ");
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%.3f", absVoltage * gain);
		writeString(displayOffset + 11, buffer);

		//Cell 8
		r2 = 70.6E3;
		r1 = 2.772E6;
		gain = r2 / (r1 + r2);
		gain = 1.00 / gain;
		absVoltage = ((float) A3ADCcnt / (float) 4096) * 1.50;
		sprintf(buffer, "%.3f", absVoltage);
		displayOffset = 0x40;
		writeString(displayOffset, "C8");
		writeString(displayOffset + 4, buffer);
		writeString(displayOffset + 10, "  ");
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%.3f", absVoltage * gain);
		writeString(displayOffset + 11, buffer);

		adcReadingReady = false;
	}
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC12_VECTOR)))
#endif
void ADC12ISR(void) {
	static uint16_t index = 0;

	switch (__even_in_range(ADC12IV, 34)) {
	case 0:
		break;   //Vector  0:  No interrupt
	case 2:
		break;   //Vector  2:  ADC overflow
	case 4:
		break;   //Vector  4:  ADC timing overflow
	case 6:
		break;   //Vector  6:  ADC12IFG0
	case 8:
		break;   //Vector  8:  ADC12IFG1
	case 10:
		break;   //Vector 10:  ADC12IFG2
	case 12:          //Vector 12:  ADC12IFG3
		//Move A0 results, IFG is cleared
		A0results[index] = ADC12_A_getResults(ADC12_A_BASE,
		ADC12_A_MEMORY_0);
		//Move A1 results, IFG is cleared
		A1results[index] = ADC12_A_getResults(ADC12_A_BASE,
		ADC12_A_MEMORY_1);
		//Move A2 results, IFG is cleared
		A2results[index] = ADC12_A_getResults(ADC12_A_BASE,
		ADC12_A_MEMORY_2);
		//Move A3 results, IFG is cleared
		A3results[index] = ADC12_A_getResults(ADC12_A_BASE,
		ADC12_A_MEMORY_3);

		//Increment results index, modulo; Set BREAKPOINT here
		index++;

		if (index == Num_of_Results) {
			(index = 0);
			adcReadingReady = true;
		}
	case 14:
		break;   //Vector 14:  ADC12IFG4
	case 16:
		break;   //Vector 16:  ADC12IFG5
	case 18:
		break;   //Vector 18:  ADC12IFG6
	case 20:
		break;   //Vector 20:  ADC12IFG7
	case 22:
		break;   //Vector 22:  ADC12IFG8
	case 24:
		break;   //Vector 24:  ADC12IFG9
	case 26:
		break;   //Vector 26:  ADC12IFG10
	case 28:
		break;   //Vector 28:  ADC12IFG11
	case 30:
		break;   //Vector 30:  ADC12IFG12
	case 32:
		break;   //Vector 32:  ADC12IFG13
	case 34:
		break;   //Vector 34:  ADC12IFG14
	default:
		break;
	}
}

