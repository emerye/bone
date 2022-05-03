#include <msp430.h>
#include "steinhart_hart.h"

volatile unsigned int tickCountOffset = 0;       //Holds fractional 1 msec clock ticks.
volatile unsigned int tickmsec = 0;             // 1 msec clock tick counter
volatile unsigned int ticksec = 0;              // Seconds tick
volatile unsigned char twoSecUpdate = 0;        // Flag for 2 second ADC update
volatile unsigned int ADC_Result;               //ADC Result
volatile unsigned int cds_begin_tick = 0;
volatile unsigned int cds_end_tick = 0;
volatile unsigned int cds_tick = 0;             //msec tick count used for cadence
volatile unsigned int cadence_value = 0;        //Cadence value to display
volatile unsigned char update_cds;               // Flag to tell to update cadence
volatile unsigned int cds_timeout;
volatile float bat_voltage;               //Battery voltage when S2 is pushed.


#define pos1 4                                                 // Digit A1 - L4
#define pos2 6                                                 // Digit A2 - L6
#define pos3 8                                                 // Digit A3 - L8
#define pos4 10                                                // Digit A4 - L10
#define pos5 2                                                 // Digit A5 - L2
#define pos6 18                                                // Digit A6 - L18

const char digit[10] =
{
    0xFC,                                                      // "0"
    0x60,                                                      // "1"
    0xDB,                                                      // "2"
    0xF3,                                                      // "3"
    0x67,                                                      // "4"
    0xB7,                                                      // "5"
    0xBF,                                                      // "6"
    0xE4,                                                      // "7"
    0xFF,                                                      // "8"
    0xF7                                                       // "9"
};

// LCD memory map for uppercase letters
const char alphabetBig[26][2] =
{
    {0xEF, 0x00},  /* "A" LCD segments a+b+c+e+f+g+m */
    {0xF1, 0x50},  /* "B" */
    {0x9C, 0x00},  /* "C" */
    {0xF0, 0x50},  /* "D" */
    {0x9F, 0x00},  /* "E" */
    {0x8F, 0x00},  /* "F" */
    {0xBD, 0x00},  /* "G" */
    {0x6F, 0x00},  /* "H" */
    {0x90, 0x50},  /* "I" */
    {0x78, 0x00},  /* "J" */
    {0x0E, 0x22},  /* "K" */
    {0x1C, 0x00},  /* "L" */
    {0x6C, 0xA0},  /* "M" */
    {0x6C, 0x82},  /* "N" */
    {0xFC, 0x00},  /* "O" */
    {0xCF, 0x00},  /* "P" */
    {0xFC, 0x02},  /* "Q" */
    {0xCF, 0x02},  /* "R" */
    {0xB7, 0x00},  /* "S" */
    {0x80, 0x50},  /* "T" */
    {0x7C, 0x00},  /* "U" */
    {0x0C, 0x28},  /* "V" */
    {0x6C, 0x0A},  /* "W" */
    {0x00, 0xAA},  /* "X" */
    {0x00, 0xB0},  /* "Y" */
    {0x90, 0x28}   /* "Z" */
};

void initLCD()

{
    // Configure LCD pins
       SYSCFG2 |= LCDPCTL;                                        // R13/R23/R33/LCDCAP0/LCDCAP1 pins selected

       LCDPCTL0 = 0xFFFF;
       LCDPCTL1 = 0x07FF;
       LCDPCTL2 = 0x00F0;                                         // L0~L26 & L36~L39 pins selected

   //    LCDCTL0 = LCDSSEL_0 | LCDDIV_7;                            // flcd ref freq is xtclk  Original value with flicker in sunlight
       LCDCTL0 = LCDSSEL_0 | LCDDIV_4;                            // flcd ref freq is xtclk. Changed divider and battery to fix.

       // LCD Operation - Mode 3, internal 3.08v, charge pump 256Hz
       LCDVCTL = LCDCPEN | LCDREFEN | VLCD_6 | (LCDCPFSEL0 | LCDCPFSEL1 | LCDCPFSEL2 | LCDCPFSEL3);

       LCDMEMCTL |= LCDCLRM;                                      // Clear LCD memory

       LCDCSSEL0 = 0x000F;                                        // Configure COMs and SEGs
       LCDCSSEL1 = 0x0000;                                        // L0, L1, L2, L3: COM pins
       LCDCSSEL2 = 0x0000;

       LCDM0 = 0x21;                                              // L0 = COM0, L1 = COM1
       LCDM1 = 0x84;                                              // L2 = COM2, L3 = COM3

       // Display "123456"
       LCDMEM[pos1] = digit[1];
       LCDMEM[pos2] = digit[2];
       LCDMEM[pos3] = digit[3];
       LCDMEM[pos4] = digit[4];
       LCDMEM[pos5] = digit[5];
       LCDMEM[pos6] = digit[6];
       LCDCTL0 |= LCD4MUX | LCDON;                                // Turn on LCD, 4-mux selected
}


// Enable 1.2 Volt Reference output on P1.4
void enableExtReference()
{
    // Configure reference output on external pin.
    PMMCTL0_H = PMMPW_H;                             // Unlock the PMM registers
    PMMCTL2 |= EXTREFEN;                   // Enable external 1.2 volt reference
}

void initGpio(void)
{
    // Configure all GPIO to Output Low
    P1OUT = 0x00;P2OUT = 0x00;P3OUT = 0x00;P4OUT = 0x00;
    P5OUT = 0x00;P6OUT = 0x00;P7OUT = 0x00;P8OUT = 0x00;

    P1DIR = 0xFF;P2DIR = 0xFF;P3DIR = 0xFF;P4DIR = 0xFF;
    P5DIR = 0xFF;P6DIR = 0xFF;P7DIR = 0xFF;P8DIR = 0xFF;
}

unsigned char digitDecode(unsigned char number)
{
    unsigned char segment;

    switch(number) {
        case 0:
            segment = digit[0];
            break;
        case 1:
            segment = digit[1];
            break;
        case 2:
            segment = digit[2];
            break;
        case 3:
            segment = digit[3];
            break;
        case 4:
            segment = digit[4];
            break;
        case 5:
            segment = digit[5];
            break;
        case 6:
            segment = digit[6];
            break;
        case 7:
            segment = digit[7];
            break;
        case 8:
            segment = digit[8];
            break;
        case 9:
            segment = digit[9];
            break;
        default:
            segment = digit[0];
    }
    return segment;
}


void displayValue(unsigned int dispValue)
{
    // Handles displaying up to 99999
    if (dispValue>= 10000)
        LCDMEM[pos2] = digitDecode(((dispValue/10000) % 10));
    if (dispValue>= 1000)
        LCDMEM[pos3] = digitDecode(((dispValue/1000) % 10));
    if (dispValue>= 100)
        LCDMEM[pos4] = digitDecode(((dispValue/100) % 10));
    if (dispValue>= 10)
        LCDMEM[pos5] = digitDecode(((dispValue/10) % 10));
    if (dispValue>= 1)
        LCDMEM[pos6] = digitDecode(((dispValue/1) % 10));
}


void displayCadence(unsigned int dispValue)
{
    // Handles displaying up to 99999
    if (dispValue>= 100)
        LCDMEM[pos1] = digitDecode(((dispValue/100) % 10));
    else
        LCDMEM[pos1] = 0x00;
    if (dispValue>= 10)
        LCDMEM[pos2] = digitDecode(((dispValue/10) % 10));
    else
        LCDMEM[pos2] = 0x00;
    if (dispValue>= 1)
        LCDMEM[pos3] = digitDecode(((dispValue/1) % 10));
    else {
        LCDMEM[pos3] = digit[0];
    }
}


void displayCadenceInactive()
{
    unsigned char dispChar = 0x03;

        LCDMEM[pos1] = dispChar;
        LCDMEM[pos2] = dispChar;
        LCDMEM[pos3] = dispChar;
}


void displayTemperature(unsigned int dispValue)
{
    if (dispValue>= 100)
           LCDMEM[pos4] = digitDecode(((dispValue/10) % 10));
       else
           LCDMEM[pos4] = 0x00;
    if (dispValue>= 10)
        LCDMEM[pos5] = digitDecode(((dispValue/10) % 10));
    else
        LCDMEM[pos5] = 0x00;
    if (dispValue>= 1)
        LCDMEM[pos6] = digitDecode(((dispValue/1) % 10));
    else
        LCDMEM[pos6] = digit[0];
}


void clearDisplay(void)
{
    LCDMEM[pos1] = 0x00;
    LCDMEM[pos2] = 0x00;
    LCDMEM[pos3] = 0x00;
    LCDMEM[pos4] = 0x00;
    LCDMEM[pos5] = 0x00;
    LCDMEM[pos6] = 0x00;
}

double readADCVoltage()
{
    double VREF = 1.500;
    return ((double)(ADCMEM0 / 1024.0) * VREF);
}

/*  Cadence Input P1.3
 *  ADC6 Temperature Input P1.6
 *  Bat input ADC5 P1.5
 */
int main(void)
{
    volatile double tempC;
    volatile unsigned int tempF;

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    initGpio();                             // Set all gpio as outputs with low state to prevent floating nodes

    // Configure XT1 oscillator
    P4SEL0 |= BIT1 | BIT2;                              // P4.2~P4.1: crystal pins
    do
        {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);                  // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
        } while (SFRIFG1 & OFIFG);                           // Test oscillator fault flag

    CSCTL6 = (CSCTL6 & ~(XT1DRIVE_3)) | XT1DRIVE_2;     // Higher drive strength and current consumption for XT1 oscillator

    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;   // set XT1 32768Hz, DCOCLK as MCLK and SMCLK source

    // Configure GPIOs
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction

    //Cadence switch input
    P1OUT |= BIT3;                          // Configure P1.3 as pulled-up
    P1REN |= BIT3;                          // P1.3 pull-up register enable
    P1IES |= BIT3;                          // P1.3 Hi/Low edge
    P1IE |= BIT3;                           // P1.3 interrupt enabled

    //S2 Button P2.6
    P2DIR &= ~BIT6;                          // Set P2.6 as input
    P2OUT |= BIT6;                           // P2.6 high
    P2REN |= BIT6;                          // P2.3 pull-up register enable
    P2IES |= BIT6;                          // P2.3 Hi/Low edge
    //P2IE |= BIT6;                           // P2.3 interrupt enabled

    P1OUT &= ~BIT1;                          // Set P1.1 low

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    initLCD();
    enableExtReference();
    // Configure reference
    PMMCTL0_H = PMMPW_H;                                      // Unlock the PMM registers
    PMMCTL2 |= INTREFEN;                                      // Enable internal reference

    // Configure ADC A6 pin for ADC and Ext 1.2 volt Ref in system cfg register in SYS Module
    SYSCFG2 |= ADCPCTL6;
    SYSCFG2 |= ADCPCTL4;
    SYSCFG2 |= ADCPCTL5;        //Input for measuring Bat Voltage P1.5

    // Configure ADC10
    ADCCTL0 |= ADCSHT_4 | ADCON;                             // ADCON, S&H=64 ADC clks
    ADCCTL1 |= ADCSHP;                                       // ADCCLK = MODOSC; sampling timer
    ADCCTL2 |= ADCRES;                                       // 10-bit conversion results
    ADCMCTL0 |= ADCINCH_6 | ADCSREF_1;;                      // A6 ADC input select
    ADCIE |= ADCIE0;                                         // Enable ADC conv complete interrupt

    //Timer 1
    TA0CCR0 = 0;                          //Stop the Timer
    TA0CCTL0 |= CCIE;                             // TACCR0 interrupt enabled
    TA0CTL = TASSEL__ACLK | MC__UP | MC_1;             // SMCLK, UP mode
    TA0CCR0 = 31;

    P1IFG &= ~BIT3;                         // P1.3 IFG cleared

    clearDisplay();
    while(1)
    {
        if (twoSecUpdate == 2) {
            P1OUT ^= BIT0;
            twoSecUpdate = 0;
            // ADC_Sample
            ADCCTL0 |= ADCENC | ADCSC;                           // Sampling and conversion start
            __bis_SR_register(LPM0_bits | GIE);                  // LPM0, ADC_ISR will force exit
            __no_operation();                                    // For debug only
            tempF = calccurrentTemperature(readADCVoltage()) + 1;
            displayTemperature(tempF);
        }

        //Bat Voltage A5 P1.5
        if( (P2IN & 0x40) == 0x40) {
                 LCDM12W_L &= ~BIT1;
               } else {
                 LCDM12W_L |= BIT1;

                 ADCMCTL0 &= ~ADCINCH_6;                        //Disable channel 6
                 ADCMCTL0 |= ADCINCH_5 | ADCSREF_1;;            // A5 ADC input select
                 ADCIE |= ADCIE0;

                 ADCCTL0 |= ADCENC | ADCSC;                           // Sampling and conversion start
                 __bis_SR_register(LPM0_bits | GIE);                  // LPM0, ADC_ISR will force exit
                 __no_operation();                                    // For debug only
                 bat_voltage = ((ADCMEM0/1024.0) * 1.50) *1000;
                 __no_operation();
                 __disable_interrupt();
                 ADCMCTL0 |= ADCINCH_6 | ADCSREF_1;                      // Set back to A6 ADC input select
                 clearDisplay();
                 displayValue((unsigned int)bat_voltage);
                 __no_operation();
                 __delay_cycles(100000);
                 __enable_interrupt();
               }

        if (cds_timeout > 5)
        {
            cds_timeout = 0;
            cds_tick = 0;
            displayCadenceInactive();
            cds_begin_tick = 0;
        }

        __bis_SR_register(LPM3_bits | GIE); // Enter LPM3 w/interrupt
        __no_operation();                   // For debugger
    }
}


// Port 1 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT1_VECTOR))) Port_1 (void)
#else
#error Compiler not supported!
#endif
{
    P1OUT ^= BIT0;                      // P1.0 = toggle
    cds_end_tick = cds_tick - cds_begin_tick;
    if((cds_end_tick > 100) && (cds_end_tick < 3500) ) {
        displayCadence((1000.0 / cds_end_tick) * 60);
    }
    cds_begin_tick = 0;
    cds_timeout = 0;
    cds_tick = 0;
    P1IFG &= ~BIT3;   // Clear P1.3 IFG

    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3
}


// Timer A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer_A (void)
#else
#error Compiler not supported!
#endif
{
    tickmsec += 1;
    cds_tick += 1;
    if (tickmsec > 999) {
          tickmsec = 0;
          ticksec += 1;
          cds_timeout += 1;
          twoSecUpdate += 1;
          LCDM12W_L ^= BIT2;
          if( (P2IN & 0x40) == 0x40) {
            LCDM12W_L &= ~BIT1;
          } else {
            LCDM12W_L |= BIT1;
          }
        }
    // Adjust for excess counts
    tickCountOffset += 768;
    if (tickCountOffset > 1000) {
        tickCountOffset -= 1000;
        TA0CCR0 = 32;
    } else {
        TA0CCR0 = 31;
    }
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3
}


// Not used
// Timer0_A3 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TA0IV,TA0IV_TAIFG))
    {
        case TA0IV_NONE:
            break;                               // No interrupt
        case TA0IV_TACCR1:
            break;                               // CCR1 not used
        case TA0IV_TACCR2:
            break;                               // CCR2 not used
        case TA0IV_3:
            break;                               // reserved
        case TA0IV_4:
            break;                               // reserved
        case TA0IV_5:
            break;                               // reserved
        case TA0IV_6:
            break;                               // reserved
        case TA0IV_TAIFG:
            P1OUT ^= BIT0;                       // overflow
            break;
        default:
            break;
    }
}

// ADC interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC_VECTOR))) ADC_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG))
    {
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG:
            break;
        case ADCIV_ADCTOVIFG:
            break;
        case ADCIV_ADCHIIFG:
            break;
        case ADCIV_ADCLOIFG:
            break;
        case ADCIV_ADCINIFG:
            break;
        case ADCIV_ADCIFG:
            ADC_Result = ADCMEM0;
            __bic_SR_register_on_exit(LPM0_bits);            // Clear CPUOFF bit from LPM0
            break;
        default:
            break;
    }
}
