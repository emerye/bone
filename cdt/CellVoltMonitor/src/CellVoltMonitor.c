/*
 ============================================================================
 Name        : CellVoltMonitor.c
 Author      : Andy Hughes
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "wiringPi.h"
#include "wiringPiI2C.h"

#define MAC 0x3e
#define CALBASE = 0x9180;

int cellV[16];
/* Cell Gain Correction A1 Board BMS029A
 * 12226,
 * 12221,
 * 12223,
 * 12216,
 * 12153
 Address and Cell Gain 1-5    */
unsigned char cellGain[] = { 0x80, 0x91, 0xC2, 0x2F, 0xBD, 0x2F, 0xBF, 0x2F,
		0xB8, 0x2F, 0x79, 0x2F };

//Calculate a Checksum for a packet. Checksum is returned as an unsigned char.
unsigned char pktChecksum(unsigned char *data, short dataLength) {
	short iCount;
	unsigned char checksum = 0;

	for (iCount = 0; iCount < dataLength; iCount++)
		checksum = checksum + data[iCount];

	return ~checksum;
}

int maxCellV(void) {
	int i;
	int maxCellV = 0;
	int maxCellNum = 0;

	for (i = 0; i < 14; i++) {
		if (cellV[i] > maxCellV) {
			maxCellV = cellV[i];
			maxCellNum = i;
		}
	}
	return maxCellNum;
}

int minCellV(void) {
	int i;
	int minCellV = 5000;
	int minCellNum = 0;

	for (i = 0; i < 14; i++) {
		if (cellV[i] < minCellV) {
			minCellV = cellV[i];
			minCellNum = i;
		}
	}
	return minCellNum;
}

void printBuffer(unsigned char *buffer, int totalBytes) {
	int i;

	for (i = 0; i < totalBytes; i++) {
		printf("%02x ", buffer[i] & 0xFF);
	}
	puts("");
}

int writeI2CBlock(int handle, int startAddress, int numBytestoWrite,
		unsigned char *data) {
	int status, i;

	for (i = 0; i < numBytestoWrite; i++) {
		status = wiringPiI2CWriteReg8(handle, startAddress + i, data[i]);
		if (status == -1) {
			return status;
		}
	}
	return 0;
}

int ReadI2CBlock(int handle, int startAddress, int numBytestoRead,
		unsigned char *result) {

	int i;
	int data;

	for (i = 0; i < numBytestoRead; i++) {

		data = wiringPiI2CReadReg8(handle, startAddress + i);
		if (data == -1) {
			return data;
		} else {
			result[i] = data;
		}
	}
	return data;
}

void testFunctions(int handle) {

	unsigned char data[] = { 1, 2 };
	int status, i;
	unsigned char buffer[64];

	status = writeI2CBlock(handle, 0x66, sizeof(data), data);
	if (status == -1) {
		printf("Error %d\n", status);
	}

	memset(buffer, 0, sizeof(buffer));
	status = ReadI2CBlock(handle, 0x66, sizeof(data), buffer);
	printf("Test Buffer:");
	printBuffer(buffer, sizeof(data));

	//Version Read
	for (i = 0; i < 1; i++) {
		data[0] = 2;
		data[1] = 0;
		status = writeI2CBlock(handle, MAC, 2, data);
		if (status == -1) {
			printf("Error %d\n", status);
		}
		status = ReadI2CBlock(handle, 0x40, 11, buffer);
		printBuffer(buffer, 11);
	}
}

void enterCfgUpdate(int handle) {

	int status;
	unsigned char buffer[] = { 0x90, 0x00 };

	status = writeI2CBlock(handle, MAC, 2, buffer);
	if (status == -1) {
		printf("Error entering config update: %d\n", status);
	}
	delay(200);
}

int initDM(int handle) {

	unsigned char chkSum = 0;
	int status;
	unsigned char buffer[256];

	//Disable sleep
	buffer[0] = 0x18;
	buffer[1] = 0x92;
	buffer[2] = 0x02;
	buffer[3] = 0x00;
	chkSum = pktChecksum(buffer, 4);
	status = writeI2CBlock(handle, MAC, 4, buffer);
	if (status == -1) {
		printf("Error writing block disabling sleep in initDM\n");
	}
	buffer[0] = chkSum;
	buffer[1] = 6;
	status = writeI2CBlock(handle, 0x60, 2, buffer);
	if (status == -1) {
		printf("Error writing block sending checksum in initDM\n");
	}
	sleep(2);

	//Cell Gain
	chkSum = pktChecksum(cellGain, 12);
	status = writeI2CBlock(handle, MAC, 12, cellGain);
	if (status == -1) {
		printf("Error writing block in initDM\n");
	}
	buffer[0] = chkSum;
	buffer[1] = 14;
	status = writeI2CBlock(handle, 0x60, 2, buffer);
	if (status == -1) {
		printf("Error writing block in initDM\n");
	}
	return status;
}

int main(void) {
	int handle, i, maxCell, minCell, status;
	int data, address = 0x14;

	handle = wiringPiI2CSetup(0x8);
	if (handle < 0) {
		printf("Error number %d opening device.\n", handle);
	}

	status = initDM(handle);
	if (status == -1) {
		puts("Error returned from initDM");
	}

	for (i = 0; i < 14; i++) {
		data = wiringPiI2CReadReg16(handle, address + (i * 2));
		cellV[i] = data;
		printf("Cell %d = %04x = %d\n", i + 1, cellV[i], cellV[i]);
	}

//	testFunctions(handle);
	maxCell = maxCellV();
	minCell = minCellV();
	printf("Max Cell:%d Voltage: %d\n", maxCell, cellV[maxCell]);
	printf("Min Cell:%d Voltage: %d\n", minCell, cellV[minCell]);

	return EXIT_SUCCESS;
}
