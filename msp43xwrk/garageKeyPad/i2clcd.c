#include <msp430.h>
#include <stdint.h>
#include "i2clcd.h"

//Send 4 high bits over I2C
void SendNibble(uint8_t byte, uint8_t cmd) {

    TXData = byte;
    TXByteCtr = 1;                          // Load TX byte counter
    while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
                                            // Remain in LPM0 until all data
                                            // is TX'd
}


void WriteI2CNibble (uint8_t msbtoWrite, uint8_t cmd)
{
  unsigned char bytetoWrite = BACKLED;

  bytetoWrite = bytetoWrite | (msbtoWrite & 0xF0) | ENABLE | cmd;
  SendNibble(bytetoWrite, cmd);

  bytetoWrite &= ~ENABLE;
  SendNibble(bytetoWrite, cmd);

  bytetoWrite |= ENABLE;
  SendNibble(bytetoWrite, cmd);

}

/**
 * Write a byte using 2 4 bit transactions
 */
void WriteI2CByte (uint8_t bytetoWrite, uint8_t cmd)
{
  unsigned char lower = (bytetoWrite << 4) & 0b11110000;
  unsigned char upper = bytetoWrite & 0b11110000;

  WriteI2CNibble (upper, cmd);
  WriteI2CNibble (lower, cmd);
}



/**
 * Initialize character lcd in 4 bit mode.
 */
void LCDinit(void) {

    __delay_cycles(20000);      //Wait 15 msec after power up
    WriteI2CNibble(0x30, 0);
    __delay_cycles(10000);       //Wait 5 msec
    WriteI2CNibble(0x30, 0);
    __delay_cycles(250);       //Wait 160usec
    WriteI2CNibble(0x30, 0);
    __delay_cycles(10000);       //Wait 4.1 msec

    WriteI2CNibble (0x20, 0);     //Set 4-bit/2-line
    __delay_cycles(2000);       //Wait 160usec

    // Default Cursor, Display and Entry states set in the constructor
       __delay_cycles(1000);

       WriteI2CByte (LCD_CLEAR_DISPLAY, 0);
       __delay_cycles (2000);
       WriteI2CByte (LCD_RETURN_HOME, 0);
       __delay_cycles (2000);
       WriteI2CByte (LCD_CURSOR_DISPLAY, 0);
       __delay_cycles (100);
       WriteI2CByte (LCD_DISPLAY_ON_OFF | DISPLAY_ENTIRE, 0);

       __delay_cycles (2000);
       WriteI2CByte (LCD_ENTRY_MODE_SET | ENTRY_MODE_LEFT, 0);
       __delay_cycles (2000);
       WriteI2CByte (LCD_RETURN_HOME, 0);
       __delay_cycles (20000);

}
