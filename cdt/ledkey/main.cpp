/*
 * main.cpp
 *
 *  Created on: 26 Oct 2017
 *      Author: root
 */
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include "TM1638.h"

//Header pin 36
#define DATAPIN 27
//Header pin 38
#define CLKPIN 28
//Header pin 40
#define STROBEPIN 29



void sendByte(unsigned char dta) {

	int i, state;

	for (i=0; i<8; i++) {
		state = ((dta) & 1);
		dta = dta >> 1;
		digitalWrite(DATAPIN, state);
		digitalWrite(CLKPIN, HIGH);
		digitalWrite(CLKPIN, HIGH);
		digitalWrite(CLKPIN, LOW);
		digitalWrite(CLKPIN, LOW);
	}
}


void sendCommand(unsigned char dta) {
	digitalWrite(STROBEPIN, LOW);
	  sendByte(dta);
	  digitalWrite(STROBEPIN, HIGH);
}


int main(int argc, char *argv[]) {
	int position = -1, i;
	unsigned char buttons;
	unsigned char dString[] = { 0xFF, 0x1, 0x2, 0x7 };

	wiringPiSetup();
	pinMode(DATAPIN, OUTPUT);
	pinMode(CLKPIN, OUTPUT);
	pinMode(STROBEPIN, OUTPUT);
	TM1638 dispObj(DATAPIN, CLKPIN, STROBEPIN);
	dispObj.setupDisplay(true, 2);

	for (i = 0; i < 10; i++) {
		dispObj.clearDisplay();
		dispObj.setDisplayDigit('9', position += 1, true);
		dispObj.setDisplayDigit('1', position += 1, true);
		dispObj.setDisplayDigit('2', position += 1, false);
		dispObj.setDisplayDigit('3', position += 1, true);
		dispObj.setDisplayDigit('4', position += 1, false);
		dispObj.setDisplayDigit('5', position += 1, false);
		dispObj.setDisplayDigit('6', position += 1, false);
		dispObj.setDisplayDigit('7', position += 1, false);

		dispObj.setLEDs(0xAA);
		usleep(100000);
		dispObj.setLEDs(0xFF);
        usleep(100000);
		dispObj.setLEDs(0x0);
		usleep(10000);

		buttons = dispObj.getButtons();
		printf("Buttons %d\n", buttons);
		fflush(stdout);

	}
	dispObj.clearDisplay();
	dispObj.setupDisplay(true, 2);
	dispObj.setDisplayToHexNumber((unsigned long) 0xABCDEF, '1', true);
	dispObj.setDisplay(dString, sizeof(dString));
	dispObj.clearDisplayDigit(6,false);

	puts("Program Complete.\n");
	return 0;
}

