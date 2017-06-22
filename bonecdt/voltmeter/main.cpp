/*
 * main.cpp
 *
 *  Created on: Jun 18, 2017
 *      Author: andy
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>

const char * adcFileName = "/sys/bus/iio/devices/iio:device0/in_voltage1_raw";
char dataFileName[512];
FILE *dtaFileHandle;

bool showHelp = false;

void usage(void) {
	puts("\nWrite time and voltage to file every second\n");
	puts("Options:\n");
	puts("    -h  Usage\n");
	puts("    Argument is file to write results. This file is always overwritten.\n");
}

int writeLog(char *dataLine) {

	if( (dtaFileHandle = fopen(dataFileName, "a")) != NULL) {
		fprintf(dtaFileHandle, "%s", dataLine);
		fclose(dtaFileHandle);
		return 1;
	} else {
		printf("Unable to open data file.\n");
		return 1;
	}
}

int readVoltage(void) {
	int fd, status;
	unsigned char buffer[256] = { 0 };

	fd = open(adcFileName, O_RDONLY);
	if (fd < 0) {
		printf("Unable to open file\n");
	}

	off_t fileLength = lseek(fd, 0, SEEK_END);  // goes to end of file
	if (fileLength < 0) {
		printf("File fseek error.\n");
	}
	status = lseek(fd, 0, SEEK_SET);  // goes to end of file
	if (status < 0) {
		printf("File fseek error setting begining of file..\n");
	}

	status = read(fd, buffer, 4);
	if (status < 0) {
		printf("Error reading bytes from file.\n");
	}
	close(fd);
	return (atoi((const char *) buffer));
}


int main(int argc, char * argv[]) {
	time_t startTime, currentTime, now;
	int rawAdc, i, status;
	double ratio, voltage;
	double eTime;
	FILE *pdtaFile;
	char dataBuffer[512];

	char *cvalue = NULL;
	int index;
	int c;

	opterr = 0;

	while ((c = getopt(argc, argv, "hc:")) != -1)
		switch (c) {
		case 'h':
			showHelp = true;
			break;
		case 'c':
			cvalue = optarg;
			break;
		case '?':
			if (optopt == 'c')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
			fflush(stdout);
			fflush(stderr);
			sleep(3);
			return 1;
		default:
			fprintf(stderr, "Unknown option\n\n");
			usage();
			sleep(3);
			abort();
		}

	printf("cvalue = %s\n", cvalue);

	if (optind != 1) {
		printf("Output file name required as an argument.\n");
		usage();
		return 0;
	}

	for (index = optind; index < argc; index++) {
		printf("Non-option argument %s\n", argv[index]);
		strcpy(dataFileName, argv[index]);
	}

	//Actions

	if (showHelp == true) {
		usage();
		fflush(stdout);
		sleep(3);
		return 0;
	}

	if( (dtaFileHandle = fopen(dataFileName, "w")) != NULL) {
		fprintf(dtaFileHandle, "Time,Voltage\n");
		fclose(dtaFileHandle);
	} else {
		printf("Unable to open data file.\n");
		return 1;
	}

	startTime = time(NULL);

//	while(1) {
	for (i = 0; i < 30; i++) {
		currentTime = time(NULL);
		eTime = difftime(currentTime, startTime);
		rawAdc = readVoltage();

		ratio = rawAdc / 4095.0;
		memset(dataBuffer, 0, sizeof(dataBuffer));
		sprintf(dataBuffer, "%.0f,%.2f\n", eTime, (double) ratio * 5.769299);
		printf("%s",dataBuffer);
		status = writeLog(dataBuffer);
		fflush(stdout);
		now = time(NULL);
		while ( (difftime(now,currentTime) == 0) ) {
			sleep(0.05);
			now = time(NULL);
		}
	}
}
