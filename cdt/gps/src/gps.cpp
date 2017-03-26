//============================================================================
// Name        : gps.cpp
// Author      : Andy Hughes
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <nmea/nmea.h>
#include <wiringSerial.h>
#include <string>

using namespace std;

char *inBuffer = (char *) malloc(2048);
int fd;
nmeaINFO info;
nmeaPARSER parser;
nmeaTIME nmeaTime;


void printNmea(void) {

	printf("GGA Lat %f Lon %f\n", info.lat, info.lon);
	printf("Altitude %f ft\n", info.elv * 3.28084);
	nmea_time_now(&nmeaTime);
	printf("Hour %d min %d\n", nmeaTime.hour, nmeaTime.min);
	printf("Direction %f\n", info.direction);
	printf("Speed %.2f\n", info.speed);
	fflush(stdout);
}


int parseNmea(const char *cmd, int count) {

	int status = nmea_parse(&parser, cmd, count, &info);
//	printf("%s", inBuffer);
	return status;
}


int main() {
	int ch;
	int count, i;
	nmea_parser_init(&parser);

	fd = serialOpen("/dev/serial0", 9600);
	if (fd < 0) {
		puts("Unable to open serial port.");
	}

	for (i = 0; i < 300; i++) {
		count = 0;
		while (((ch != 0x0a) && (count < 100)) == 1) {
			ch = (serialGetchar(fd) & 0xFF);
			inBuffer[count] = ch;
			count += 1;
		}
		inBuffer[count] = '\0';
		ch = 0;
	//	printf("%s\n",inBuffer);

		if ((strstr(inBuffer, "$GPGGA") != NULL)) {
			int status = parseNmea(inBuffer, count);
			if (status != 1)
				printf("Status returned error %d\n", status);
		} else if ((strstr(inBuffer, "$GPGSV") != NULL)) {
			int status = parseNmea(inBuffer, count);
			if (status != 1)
				printf("Status returned error %d\n", status);
		} else if ((strstr(inBuffer, "$GPRMC") != NULL)) {
			int status = parseNmea(inBuffer, count);
			if (status != 1)
				printf("Status returned error %d\n", status);
		} else if ((strstr(inBuffer, "$GPGLL") != NULL)) {
			printNmea();
		}

		memset(inBuffer, 0, 512);
	}


	printNmea();

	free(inBuffer);
	serialClose(fd);

	return 0;
}
