/*
 ============================================================================
 Name        : CellVoltMonitor.c
 Author      : Andy Hughes
 Version     :
 Copyright   : 
 Description : CellVoltMonitor with bq76952
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>
#include <termios.h>
#include <time.h>
#include "ads1115.h"
#include "wiringPi.h"
#include "wiringPiI2C.h"

#define MAC 0x3e
#define MAXCELLS 14
#define STACKVOLTCMD 0x34

int handle;
int adcHandle;
int cellV[16];
/* Cell Gain Correction A1 Board BMS029A
 * 12226,
 * 12221,
 * 12223,
 * 12216,
 * 12153
 Address and Cell Gain 1-5    Not used*/
unsigned char cellGain[] = { 0x80, 0x91, 0xC2, 0x2F, 0xBD, 0x2F, 0xBF, 0x2F,
		0xB8, 0x2F, 0x79, 0x2F };
FILE *logFile;
char *logFileName = "/home/andy/bone/plot/celllog.txt";
int logTime=0;
int stackVolts;
int gloopDelay = 1;      //Time in seconds


double readADS1115() {

	int adcReading;
	unsigned char buffer[10];
	double voltage;

	wiringPiI2CWrite(adcHandle, ADS1015_REG_POINTER_CONVERT);
	usleep(2000);
	read(adcHandle, buffer, 2);
	adcReading = ((buffer[0] << 8) | buffer[1]);
	//printf("ADC reading 0x%04x\n",adcReading);
	voltage = (double)adcReading * 0.000007812;
	usleep(1000);
	return voltage;
}


int configADS1115() {

	unsigned char buffer[4];
	int status;

	buffer[0] = ADS1015_REG_POINTER_CONFIG;
	buffer[1] = ADS1015_REG_CONFIG_PGA_0_256V;
	buffer[2] = 0x40;  //Mode continuous

	//Write to conversion register. This is a block write
	status = write(adcHandle, buffer, 3);
	if (status == -1) {
		printf("Error writing block during initialization.\n");
		return status;
	}
	return 0;
}


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

	for (i = 0; i < MAXCELLS; i++) {
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

	for (i = 0; i < MAXCELLS; i++) {
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

int writeI2CBlock(int startAddress, int numBytestoWrite,
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

int readI2CBlock(int startAddress, int numBytestoRead,
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

void testFunctions() {

	unsigned char data[] = { 1, 2 };
	int status, i;
	unsigned char buffer[64];

	status = writeI2CBlock(0x66, sizeof(data), data);
	if (status == -1) {
		printf("Error %d\n", status);
	}

	memset(buffer, 0, sizeof(buffer));
	status = readI2CBlock(0x66, sizeof(data), buffer);
	printf("Test Buffer:");
	printBuffer(buffer, sizeof(data));

	//Version Read
	for (i = 0; i < 1; i++) {
		data[0] = 2;
		data[1] = 0;
		status = writeI2CBlock(MAC, 2, data);
		if (status == -1) {
			printf("Error %d\n", status);
		}
		status = readI2CBlock(0x40, 11, buffer);
		printBuffer(buffer, 11);
	}
}

int initDM() {

	unsigned char chkSum = 0;
	int status;
	unsigned char buffer[256];

	//Disable sleep
	buffer[0] = 0x18;
	buffer[1] = 0x92;
	buffer[2] = 0x02;
	buffer[3] = 0x00;
	chkSum = pktChecksum(buffer, 4);
	status = writeI2CBlock(MAC, 4, buffer);
	if (status == -1) {
		printf("Error writing block disabling sleep in initDM\n");
	}
	buffer[0] = chkSum;
	buffer[1] = 6;
	status = writeI2CBlock(0x60, 2, buffer);
	if (status == -1) {
		printf("Error writing block sending checksum in initDM\n");
	}
	sleep(2);

	//Cell Gain
	/*
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
	 */

	//Stack Gain
	buffer[0] = 0xA2;
	buffer[1] = 0x91;
	buffer[2] = 0xC9;  //TOS Gain 36041
	buffer[3] = 0x8C;
	chkSum = pktChecksum(buffer, 4);
	status = writeI2CBlock(MAC, 4, buffer);
	if (status == -1) {
		printf("Error writing block to set TOS Gain in initDM\n");
	}
	buffer[0] = chkSum;
	buffer[1] = 6;
	status = writeI2CBlock(0x60, 2, buffer);
	if (status == -1) {
		printf(
				"Error writing block sending checksum to set TOS Gain in initDM\n");
	}
	sleep(2);
	return status;
}

void readCells() {
	int data, address = 0x14;
	int i, minCell, maxCell;
	double curVoltage, current;

	for (i = 0; i < MAXCELLS; i++) {
		data = wiringPiI2CReadReg16(handle, address + (i * 2));
		cellV[i] = data;
	}
//	cellV[13] = 3672;   Cell 14 is not connected on Schwinn
	for (i = 0; i < MAXCELLS; i += 2) {
		printf("Cell%d=%d Cell%d=%d\n", i + 1, cellV[i], i + 2, cellV[i + 1]);
	}

	stackVolts = (wiringPiI2CReadReg16(handle, STACKVOLTCMD)) * 10;

	curVoltage = readADS1115();
	current = curVoltage * 39.0625;
	printf("Current %f Amps\n", current);

	maxCell = maxCellV();
	minCell = minCellV();
	printf("Min Cell:%d Voltage: %d\n", minCell + 1, cellV[minCell]);
	printf("Max Cell:%d Voltage: %d\n", maxCell + 1, cellV[maxCell]);
	printf("Voltage Delta: %d mV\n", cellV[maxCell] - cellV[minCell]);
	printf("Stack Voltage %d\n", stackVolts);
	puts("");
	fprintf(logFile,
			"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %f\n",
			logTime, cellV[0], cellV[1], cellV[2], cellV[3], cellV[4],
			cellV[5], cellV[6], cellV[7], cellV[8], cellV[9], cellV[10],
			cellV[11], cellV[12], cellV[13], minCell + 1, maxCell + 1,
			cellV[maxCell] - cellV[minCell], stackVolts, current);

	logTime = logTime + gloopDelay;
	fflush(logFile);
	fflush(stdout);
}

/******************************************************************************/

void timeStamp (char *curTime) {

  static char time_buffer[64];
  const struct tm *tm;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  strftime ( time_buffer, 64, "%d %B %Y %I:%M:%S %p", tm );
  sprintf ( curTime, "%s\n", time_buffer );
  return;
}


int main(int argc, char *argv[]) {

	int status;
	logTime = 0;
	int index;
	int c;
	opterr = 0;
	char currentTime[64];

	while ((c = getopt(argc, argv, "hd:")) != -1)
		switch (c) {
		case 'd':
			gloopDelay = atoi(optarg);
			printf("Option delay %d\n", gloopDelay);
			break;
		case 'h':
			puts(
					"Default operation without an argument is to take measurements in a 1 second loop.");
			puts(
					"Option -d [delay time in seconds] sets the delay between readings in seconds.");
			printf("Log file is written to %s.\n", logFileName);
			break;
		case '?':
			if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			return 1;
		default:
			abort();
		}

	for (index = optind; index < argc; index++)
		printf("Non-option argument %s\n", argv[index]);

	printf("Log file name %s\n", logFileName);

	handle = wiringPiI2CSetup(0x8);
	if (handle < 0) {
		printf("Error number %d opening device.\n", handle);
	}

	adcHandle = wiringPiI2CSetup(ADS1015_ADDRESS);
		if (handle < 0) {
			printf("Error number %d opening device.\n", adcHandle);
		}

	status = configADS1115();
		if (status == -1) {
			printf("Error configuring device.\n");
			return -1;
		}

	status = initDM(handle);
	if (status == -1) {
		puts("Error returned from initDM");
	}

	if ((logFile = fopen(logFileName, "w")) == NULL) {
		printf("Unable to open cell logfile %s\n", logFileName);
		return -2;
	}
	timeStamp(currentTime);
	fprintf(logFile, "# Created on %s", currentTime);
	fprintf(logFile, "# Delay is %d seconds.\n", gloopDelay);
	fprintf(logFile,"# Time Cell1 Cell2 Cell3 Cell4 Cell5 Cell6 Cell7 Cell8 Cell9 Cell10 Cell11 Cell12 Cell13 Cell14 MinCell MaxCell DeltaV StackV Current\n");

	//Main loop
	while (1) {
		readCells(handle);
		sleep(gloopDelay);
	}
	fclose(logFile);
	close(handle);
	close(adcHandle);

	return EXIT_SUCCESS;
}
