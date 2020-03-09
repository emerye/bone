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
#include "wiringPi.h"
#include "wiringPiI2C.h"

#define MAC 0x3e

void printBuffer(unsigned char *buffer, int totalBytes) {
	int i;

	for(i=0; i<totalBytes; i++) {
		printf("%02x ", buffer[i] & 0xFF);
	}
	puts("");
}


int writeI2CBlock(int handle, int startAddress, int numBytestoWrite, unsigned char *data) {
	int status, i;

	for(i=0; i<numBytestoWrite; i++) {
		status = wiringPiI2CWriteReg8 (handle, startAddress+i, data[i]);
		if (status > 0) {
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
		printf("Data Read %02x\n", data & 0xFF);
		result[i] = data;
	}
	return 0;
}


void testFunctions(int handle) {

	unsigned char data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int status;
	unsigned char buffer[64];

	status = writeI2CBlock(handle, 0x40, sizeof(data), data);
	if (status == -1 ) {
		printf("Error %d\n", status);
	}

	memset(buffer, 0, sizeof(buffer));
	status = ReadI2CBlock(handle, 0x16, 10, buffer);
	printf("Byte 0: %02x Byte 1: %02x\n", buffer[0], buffer[1]);

	//Version Read
	data[0] = 2;
	data[1] = 0;
	status = writeI2CBlock(handle, MAC, 2, data);
		if (status == -1 ) {
			printf("Error %d\n", status);
		}

	status = ReadI2CBlock(handle, 0x40, 11, buffer);

	printBuffer(buffer, 11);
}

int main(void) {
	int handle, i;
	int data, address = 0x14;

	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	handle = wiringPiI2CSetup(0x8);

	if (handle < 0) {
		printf("Error number %d opening device.\n", handle);
	}

	for (i = 0; i < 16; i++) {
		data = wiringPiI2CReadReg16(handle, address + (i * 2));
		printf("Cell %d = %04x = %d\n", i, data, data);
	}

	testFunctions(handle);

	return EXIT_SUCCESS;
}
