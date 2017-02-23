/*
 * Hardware.cpp
 *
 *  Created on: 21 Feb 2017
 *      Author: root
 */

#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "Hardware.h"


Hardware::Hardware() {
	// TODO Auto-generated constructor stub

}

Hardware::~Hardware() {
	// TODO Auto-generated destructor stub
}


int Hardware::SendSPIBlock(enum cmd cmdType, unsigned char *spiData,
		int numBytes) {
	int error;
	unsigned char spiBuffer[20];

	memcpy(spiBuffer, spiData, numBytes);

	if (cmdType == COMMAND) {
		digitalWrite(DCLINE, LOW);
	} else {
		digitalWrite(DCLINE, HIGH);
	}

	error = wiringPiSPIDataRW(0, spiBuffer, numBytes);
	if (error == -1) {
		printf("Error writing SPI %d\n", error);
		perror("SPI Write");
	}
	return error;
}

int Hardware::sendByte(enum cmd cmdType, int data) {
	unsigned char spiBuffer[20];

	spiBuffer[0] = (unsigned char) (data & 0xFF);
	return (SendSPIBlock(cmdType, spiBuffer, 1));
}
