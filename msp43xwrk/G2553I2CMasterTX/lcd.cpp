#include "msp430g2553.h"
#include <string.h>
#include "lcdchar.h"

extern void I2CWriteBlock(unsigned char , unsigned char *, int);
extern void I2CReadBlock(unsigned char tgtAddress, unsigned char *dataRead, int numBytestoRead);


//Write a string to display
void Lcd::WriteString(int row, int ypos, char message[]) {
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
	WriteLCDByte(address, 0);
	for (i = 0; i < stLength; i++) {
		if (message[i] > 0x1f) {
			WriteLCDByte(message[i], 1);
		}
	}
}


/**
 * Delay 1 msec with a 4MHz clock
 */
void Lcd::DelayMsec(int delay) {
	volatile int dummy;
	int i, outcount;
	for (outcount = 0; outcount < delay; outcount++) {
		for (i = 0; i < 471/4; i++) {
			dummy++;
		}
	}
}

/**
 * Write one byte to I2C. Target address is globally set.
 *
 */
void Lcd::WriteI2CByte(unsigned char data) {

	unsigned char lcldata[2];

	lcldata[0] = data;

	I2CWriteBlock(lcdI2CAddress, lcldata, 2);

}

/**
 * Write I2C Nibble
 */
void Lcd::WriteI2CNibble(unsigned char msbtoWrite, int cmd) {

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
void Lcd::WriteLCDByte(unsigned char bytetoWrite, int cmd) {
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
void Lcd::Setup4bit() {
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

