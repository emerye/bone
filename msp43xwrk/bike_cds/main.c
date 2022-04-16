#include <msp430.h>

volatile unsigned int tickCountOffset = 0;       //Holds fractional 1 msec clock ticks.
volatile unsigned int tickmsec = 0;               // 1 msec clock tick counter
volatile unsigned int ticksec = 0;

void initGpio(void)
{
    // Configure all GPIO to Output Low
    P1OUT = 0x00;P2OUT = 0x00;P3OUT = 0x00;P4OUT = 0x00;
    P5OUT = 0x00;P6OUT = 0x00;P7OUT = 0x00;P8OUT = 0x00;

    P1DIR = 0xFF;P2DIR = 0xFF;P3DIR = 0xFF;P4DIR = 0xFF;
    P5DIR = 0xFF;P6DIR = 0xFF;P7DIR = 0xFF;P8DIR = 0xFF;
}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    initGpio();                             // Set all gpio as outputs with low state to prevent floating nodes

    P1DIR |= BIT0;                                // P1.0 output On board LED

    // Configure XT1 oscillator
    P4SEL0 |= BIT1 | BIT2;                              // P4.2~P4.1: crystal pins
    do
        {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);                  // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
        } while (SFRIFG1 & OFIFG);                           // Test oscillator fault flag

    CSCTL6 = (CSCTL6 & ~(XT1DRIVE_3)) | XT1DRIVE_2;     // Higher drive strength and current consumption for XT1 oscillator

    CSCTL4 = SELMS__DCOCLKDIV | SELA__XT1CLK;   // set XT1 32768Hz, DCOCLK as MCLK and SMCLK source

    // Configure GPIO
    P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT |= BIT3;                          // Configure P1.3 as pulled-up
    P1REN |= BIT3;                          // P1.3 pull-up register enable
    P1IES |= BIT3;                          // P1.3 Hi/Low edge
    P1IE |= BIT3;                           // P1.3 interrupt enabled


    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    //Timer 1
    TA0CCR0 = 0;                          //Stop the Timer
    TA0CCTL0 |= CCIE;                             // TACCR0 interrupt enabled
    TA0CTL = TASSEL__ACLK | MC__UP | MC_1;             // SMCLK, UP mode
    TA0CCR0 = 31;

    P1IFG &= ~BIT3;                         // P1.3 IFG cleared

    while(1)
    {
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
    P1IFG &= ~BIT3;                         // Clear P1.3 IFG
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
    if (tickmsec > 999) {
          P1OUT ^= BIT0;                      // P1.0 = toggle
          tickmsec = 0;
          ticksec += 1;
        }

    tickCountOffset += 768;
    if (tickCountOffset > 1000) {
        tickCountOffset -= 1000;
        TA0CCR0 = 32;
    } else {
        TA0CCR0 = 31;
    }

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

