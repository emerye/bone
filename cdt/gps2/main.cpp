
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringPiI2C.h>
#include <time.h>
#include "nmea/nmea.h"
#include <wiringSerial.h>

#include "Adafruit_GFX.h"
#include "tft1963.h"
#include "DispDraw.h"
#include "gfxfont.h"
#include "glcdfont.c"
#include "Fonts/FreeMono9pt7b.h"
#include "Fonts/FreeMono12pt7b.h"
#include "Fonts/FreeMono18pt7b.h"
#include "Fonts/FreeMono24pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans18pt7b.h"
#include "Fonts/FreeSans24pt7b.h"

#define LM75ADDRESS 0x48

/* number of times the handle will run: */
volatile int elapsedSeconds = 0;
volatile int newDataAvail = 0;
volatile int elapsedMinutes = 0;
volatile int pixelColor = 0;

DispDraw *displayRef;

char *inBuffer = (char *) malloc(2048);
int gpsfd;
nmeaINFO info;
nmeaPARSER parser;
nmeaTIME nmeaTime;
bool debug = false;
unsigned int count;
double speedAverage = 0;
double tripMiles;

const char *compassDirection[] = { "North", "NEast", "East", "SEast", "South",
		"SWest", "West", "NWest" };

float getTemperature(int fd) {
	float degC;

	int raw = wiringPiI2CReadReg16(fd, 0x00);
	raw = ((raw << 8) & 0xFF00) + (raw >> 8);
	degC = (float) ((raw / 32.0) / 8.0);
	return (float) (degC * 9.0 / 5.0 + 32);
}

int averageSpeed(int currentSpeed) {
	speedAverage = speedAverage + (currentSpeed - speedAverage) / count;
	return (int) speedAverage;
}

const char* getDirection() {

	double direction = info.direction;

	if (direction > 337.5 || direction < 22.5) {
		return compassDirection[0];
	} else if (direction >= 22.5 && direction < 67.5) {
		return compassDirection[1];
	} else if (direction >= 67.5 && direction < 112.5) {
		return compassDirection[2];
	} else if (direction >= 112.5 && direction < 157.5) {
		return compassDirection[3];
	} else if (direction >= 157.5 && direction < 202.5) {
		return compassDirection[4];
	} else if (direction >= 202.5 && direction < 247.5) {
		return compassDirection[5];
	} else if (direction >= 247.5 && direction < 292.5) {
		return compassDirection[6];
	} else if (direction >= 292.5 && direction < 337.5) {
		return compassDirection[7];
	}
	return compassDirection[0];
}

int parseNmea(const char *cmd, int count) {

	int status = nmea_parse(&parser, cmd, count, &info);
	return status;
}

void printNmea(void) {

	if (debug == false)
		return;

	printf("GGA Lat %f Lon %f\n", info.lat, info.lon);
	printf("Altitude %f ft\n", info.elv * 3.28084);
	nmea_time_now(&nmeaTime);
	printf("Hour %d min %d sec %d\n", nmeaTime.hour, nmeaTime.min,
			nmeaTime.sec);
	printf("Direction %f\n", info.direction);
	printf("Speed %.2f\n", info.speed);
	fflush(stdout);
}

void readGPS() {

	int ch;
	int count;
	nmea_parser_init(&parser);
	int status;
	bool exit = false;

	while (exit == false) {

		count = 0;
		while (((ch != 0x0a) && (count < 100)) == 1) {
			ch = (serialGetchar(gpsfd) & 0xFF);
			inBuffer[count] = ch;
			count += 1;
		}
		inBuffer[count] = '\0';
		if (count == 100) {
			return;
		}
		ch = 0;

		if ((strstr(inBuffer, "$GPGGA") != NULL)) {
			status = parseNmea(inBuffer, count);
			if (status != 1)
				printf("Status returned error %d\n", status);
		} else if ((strstr(inBuffer, "$GPGSV") != NULL)) {
			status = parseNmea(inBuffer, count);
			if (status != 1)
				printf("Status returned error %d\n", status);
			//Not available in GS229 use RMC to terminate.
		} else if ((strstr(inBuffer, "$GPGLL") != NULL)) {
			parseNmea(inBuffer, count);
		} else if ((strstr(inBuffer, "$GPRMC") != NULL)) {
			parseNmea(inBuffer, count);
		//	exit = true;
		} else if ((strstr(inBuffer, "$GPVTG") != NULL)) {
			parseNmea(inBuffer, count);
			exit = true;
		}
		memset(inBuffer, 0, 512);
	}
}

void handle(int sig) {
	elapsedSeconds += 1;
	newDataAvail = 1;
	alarm(1);
}

void fillWithColor(unsigned short color) {
	int i;

	for (i = 0; i < 480 * 272; i++) {
		displayRef->fBuffer[i] = color;
	}
	displayRef->bufftoDisplay();
	sleep(1);
}

void TextDemo() {
	int x = 1;
	int y = 20;

	fillWithColor(RED);
	fillWithColor(GREEN);
	fillWithColor(BLUE);
	fillWithColor(WHITE);
	memset(displayRef->fBuffer, 0, sizeof(displayRef->fBuffer));
	displayRef->setFont(&FreeMono9pt7b);
	displayRef->writeString(x, y, 1,
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnop", RED);
	displayRef->drawPixel(0, 0, WHITE);
	displayRef->setFont(&FreeMono12pt7b);
	displayRef->writeString(x, y += 20, 1, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdef",
			PURPLE);
	displayRef->setFont(&FreeMono18pt7b);
	displayRef->writeString(20, y += 30, 1, "ABCDEFGHIJKLMNOPQ", SILVER);
	displayRef->drawChar(1, y += 30, 'A', WHITE, BLACK, (uint8_t) 1);
	displayRef->setFont(&FreeMono24pt7b);
	displayRef->writeString(x, y += 35, 1, "ABCDEFGHIJKLMNOP", BLUE);
	displayRef->setFont(&FreeMono18pt7b);
	displayRef->writeString(x, y += 50, 2, "ABCDEFGHIJK", OLIVE);
	displayRef->setFont(&FreeMono24pt7b);
	displayRef->writeString(x, y += 65, 2, "12345678", GREEN);
	displayRef->drawPixel(0, 271, WHITE);
	displayRef->drawPixel(479, 271, WHITE);
	displayRef->drawPixel(479, 0, WHITE);
	displayRef->bufftoDisplay();
}

int main() {
	int i2cHandle;
	time_t startTime, currentTime, gpsStartTime, gpsEndTime;
	unsigned int newMinutes = 0;
	unsigned short int xstart = 0;
	unsigned short int ystart = 30;
	int tick = 0;
	char buffer[50];
	int divider = 60;   //Change to 60 for release
	int testOffset = 0;
	bool debug = false;
	float temperatureF;

	DispDraw display(480, 272);
	displayRef = &display;

	i2cHandle = wiringPiI2CSetup(LM75ADDRESS);
	if (i2cHandle < 0) {
		printf("Error opening I2C device device.\n");
	}

	display.setFont(&FreeSans24pt7b);
	display.initRpiHardware();
	display.Init_ssd1963();
	fillWithColor(BLUE);
	sleep(1);

	gpsfd = serialOpen("/dev/serial0", 9600);
	if (gpsfd < 0) {
		puts("Unable to open serial port.");
	}
	nmea_parser_init(&parser);
	time(&startTime);

	memset(display.fBuffer, 0x00, sizeof(display.fBuffer));
	display.bufftoDisplay();

	time(&startTime);

	for (int i = 0; i < 20; i++) {
//	while (1) {
		time(&gpsStartTime);
		readGPS();
		time(&gpsEndTime);
		while (gpsStartTime == gpsEndTime) {
			sleep(0.05);
			time(&gpsEndTime);
		}
		temperatureF = getTemperature(i2cHandle);

		tick ^= 1;
		count++;

		if (debug)
			printf("Altitude %f ft\n", info.elv * 3.28084);

		/*
		display.setFont(&FreeSans18pt7b);
		sprintf(buffer, "Lat:  %.6f", info.lat);
		display.writeString(xstart, ystart, 1, buffer, GREEN);

		display.setFont(&FreeSans18pt7b);
		sprintf(buffer, "Long: %.6f", info.lon);
		display.writeString(xstart, ystart + 30, 1, buffer, GREEN);
		*/

	//	display.writeString(xstart, ystart + 65, 1, buffer, WHITE);

		display.setFont(&FreeSans24pt7b);
		double curSpeed = info.speed * 0.621371;
		sprintf(buffer, "%d mph    Ave %d mph", (int) curSpeed,
				averageSpeed(curSpeed));
		display.writeString(xstart, ystart + 105, 1, buffer, WHITE);

		time(&currentTime);
		newMinutes = ((currentTime - startTime) / divider) + testOffset;
		sprintf(buffer, "ET %3d min", newMinutes);
		display.writeString(xstart, ystart + 155, 1, buffer, BLUE);
		sprintf(buffer, "%.1f Deg", temperatureF);
		display.writeString(xstart + 255, ystart + 155, 1, buffer, YELLOW);

		sprintf(buffer, "%d Deg  %s", (int) info.direction, getDirection());
		display.writeString(xstart, ystart + 205, 1, buffer, PURPLE);

		tripMiles = (currentTime - startTime)/3600 * speedAverage;
		display.setFont(&FreeSans18pt7b);

		sprintf(buffer, "Trip  %.1f       Alt %dft", tripMiles, (int)(info.elv * 3.28084));
		display.writeString(xstart, ystart, 1, buffer, GREEN);

		if (tick == 1)
			display.drawPixel(479, 271, WHITE);
		display.bufftoDisplay();
		memset(display.fBuffer, 0x00, sizeof(display.fBuffer));

		puts("\n");
		fflush(stdout);
	}
	free(inBuffer);
	serialClose(gpsfd);
	printf("Done\n");
	return 0;
}

