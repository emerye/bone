//============================================================================
// Name        : tft1963.cpp
// Author      : Andy Hughes
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <errno.h>
#include "tft1963.h"
#include "Adafruit_GFX.h"

//Constructor
RpiHardware::RpiHardware() : Adafruit_GFX(XMAXPIXEL, YMAXPIXEL) {

	HDP=479;
	HT=531;
	HPS=43;
	LPS=8;
	HPW=10;

	VDP=271;
	VT=288;
	VPS=12;
	FPS=4;
	VPW=10;


	wiringPiSetup();
	spiDescriptor = wiringPiSPISetup(0, 8000000);
	if (spiDescriptor < 0) {
		puts("SPI Initialisation Failed");
	}
}

RpiHardware::~RpiHardware() {
}

void RpiHardware::initRpiHardware() {
	pinMode(RESETGPIO, OUTPUT);
	pinMode(DATACMDGPIO, OUTPUT);
	pinMode(WRITEGPIO, OUTPUT);
	digitalWrite(RESETGPIO, HIGH);
	pinMode(WRITEGPIO, HIGH);
}


void RpiHardware::SendCommand(cmdType cmd) {
	if (cmd == WRITECMD) {
		digitalWrite(DATACMDGPIO, LOW);
		digitalWrite(WRITEGPIO, LOW);
	} else if (cmd == WRITEDATA) {
		digitalWrite(DATACMDGPIO, HIGH);
		digitalWrite(WRITEGPIO, LOW);
	}
	digitalWrite(WRITEGPIO, HIGH);
	digitalWrite(DATACMDGPIO, HIGH);
}


/*
//Overriding base class
void RpiHardware::drawFastVLine(short x1, short y1,  short y2,
		unsigned short color) {
	int i, j;

	setAddress(x1, y1, x1, y2+y1);
	Write_Data(color);
	for (i = y1; i <= y2+y1; i++) {
			SendCommand(WRITEDATA);
	}
}

void RpiHardware::drawFastHLine(int16_t x, int16_t y,
 int16_t w, uint16_t color) {

	setAddress(x, y, x+w, y);
		Write_Data(color);
		for (int k = x; k <= x+w; k++) {
			SendCommand(WRITEDATA);
		}
}


//This does not write to frame buffer
void RpiHardware::TFT_FillDisp(unsigned int color) {
	unsigned int i, j;

	setAddress(0, 0, XMAXPIXEL, YMAXPIXEL);
	Write_Data(color);
	for (i = 0; i < XMAXPIXEL; i++) {
		for (j = 0; j < YMAXPIXEL; j++)
			SendCommand(WRITEDATA);;
	}
}
*/

void RpiHardware::Write_Command(unsigned int data) {

	unsigned char buffer[2];
	data = data & 0xFFFF;
	buffer[0] = (unsigned char) ((data >> 8) & 0xFF);
	buffer[1] = (unsigned char) (data & 0xFF);
    int status = wiringPiSPIDataRW(spiDescriptor, buffer, 2);
    if (status < 0) {
    	int error = errno;
    	printf("Write SPI command failed in Write_Command. Error: %d\n", error);
    }
	SendCommand(WRITECMD);
}

void RpiHardware::Write_Data(unsigned short data) {

	unsigned char buffer[2];
	data = data & 0xFFFF;
	buffer[0] = (unsigned char) ((data >> 8) & 0xFF);
	buffer[1] = (unsigned char) (data & 0xFF);
	int status = wiringPiSPIDataRW(spiDescriptor, buffer, 2);
	if (status < 1) {
		int error = errno;
		printf("Write SPI data failed in Write_Command. Error: %d\n", error);
	}
	SendCommand(WRITEDATA);
}

//Init sequence for SSD1963 and eBay 4.3 inch TFT.
void RpiHardware::Init_ssd1963(void) {
	//Reset Line
	digitalWrite(RESETGPIO, HIGH);
	usleep(15000);
	digitalWrite(RESETGPIO, LOW);
	usleep(25000);
	digitalWrite(RESETGPIO, HIGH);
	usleep(25000);

	Write_Command(0x00E2);	//PLL multiplier, set PLL clock to 120M
	Write_Data(0x002d);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	Write_Data(0x0002);
	Write_Data(0x0054);

	Write_Command(0x00E0);  // PLL enable
	Write_Data(0x0001);
	usleep(1000 * 50);       //Wait 5 usec

	Write_Command(0x00E0);  //Use PLL as system clock. Enable PLL
	Write_Data(0x0003);
	usleep(1000 * 50);

	Write_Command(0x0001);  // software reset
	usleep(5000);
	Write_Command(0x00E6);	//PLL setting fWHITEor PCLK, depends on resolution
	Write_Data(0x0000);
	Write_Data(0x00ff);
	Write_Data(0x00be);

	usleep(50000);

	Write_Command(0x00F0); //pixel data interface
	Write_Data(0x0003);    //16 bit 565= 3
	usleep(1000);

	//Rotation
	Write_Command(SSD1963_SET_ADDRESS_MODE);
	Write_Data(0);

	Write_Command(0x00B0);	//LCD SPECIFICATION
	Write_Data(0x0020);     //24 bit
	Write_Data(0x0000);
	Write_Data((HDP >> 8) & 0X00FF);  //Set HDP
	Write_Data(HDP & 0X00FF);
	Write_Data((VDP >> 8) & 0X00FF);  //Set VDP
	Write_Data(VDP & 0X00FF);
	Write_Data(0x0000);
	usleep(5 * 10000);

	Write_Command(0x00B4);	//HSYNC
	Write_Data((HT >> 8) & 0X00FF);  //Set HT
	Write_Data(HT & 0X00FF);
	Write_Data((HPS >> 8) & 0X00FF);  //Set HPS
	Write_Data(HPS & 0X00FF);
	Write_Data(HPW);			   //Set HPW
	Write_Data((LPS >> 8) & 0X00FF);  //SetLPS
	Write_Data(LPS & 0X00FF);
	Write_Data(0x0000);

	Write_Command(0x00B6);	//VSYNC
	Write_Data((VT >> 8) & 0X00FF);   //Set VT
	Write_Data(VT & 0X00FF);
	Write_Data((VPS >> 8) & 0X00FF);  //Set VPS
	Write_Data(VPS & 0X00FF);
	Write_Data(VPW);			   //Set VPW
	Write_Data((FPS >> 8) & 0X00FF);  //Set FPS
	Write_Data(FPS & 0X00FF);

	Write_Command(0x0029); //display on
	usleep(25000);

	/*
	 LCD_WR_REG(0x00BE); //set PWM for B/L
	 LCD_WR_Data(0x0006);
	 LCD_WR_Data(0x00f0);
	 LCD_WR_Data(0x0001);
	 LCD_WR_Data(0x00f0);
	 LCD_WR_Data(0x0000);
	 LCD_WR_Data(0x0000);
	 */

	/*
	 Write_Command(0x00d0);
	 Write_Data(0x000d);


	 Write_Command(0x00BA);
	 Write_Data(0x0000);
	 */
}

void RpiHardware::setAddress(unsigned int x1, unsigned int y1, unsigned int x2,
		unsigned int y2) {
	Write_Command(0x002A);		//Set Column Address

	Write_Data(x1 >> 8);
	Write_Data(x1 & 0xff);
	Write_Data(x2 >> 8);
	Write_Data(x2 & 0xff);

	Write_Command(0x002b);		//Set page address

	Write_Data(y1 >> 8);
	Write_Data(y1 & 0x00ff);
	Write_Data(y2 >> 8);
	Write_Data(y2 & 0x00ff);

	Write_Command(0x002c);
}




