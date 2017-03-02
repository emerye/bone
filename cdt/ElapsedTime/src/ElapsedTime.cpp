//============================================================================
// Name        : ElapsedTime.cpp
// Author      : Andy Hughes
// Version     :
// Copyright   : Your copyright notice
// Description : Elapsed Time Clock
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <time.h>
#include "../oled1309/oled1309.h"
#include "../oled1309/gfxfont.h"
#include "../oled1309/glcdfont.h"
#include "bme280.h"
#include "FreeMono12pt7b.h"
#include "oled1309.h"
#include "FreeMono18pt7b.h"
#include "FreeMono9pt7b.h"

#define SEALEVELPRESSURE_HPA (1013.25)
#define TEMPOFFSET -0.3

/* number of times the handle will run: */
volatile int elapsedSeconds = 0;
volatile int newDataAvail = 0;
volatile int elapsedMinutes = 0;
volatile int pixelColor = 0;

oled1309 *displayRef;

void handle(int sig) {
	elapsedSeconds += 1;
	newDataAvail = 1;
	alarm(1);
}

void displayUnits() {
	displayRef->setFont(FreeMono9pt7b);
	int xstart = 20;
	int ystart = 9;
	int height = 9 + 8;

	displayRef->setFont(FreeMono9pt7b);
	displayRef->writeString(xstart,ystart ,1,"Min");
	displayRef->writeString(xstart + 60, ystart, 1, "Temp");
}

int main() {
	Adafruit_BME280 sensor;
	time_t startTime, currentTime;
	unsigned int elapsedMinutes = 0;
	unsigned int newMinutes = 0;
	int xstart = 0;
	int ystart = 24;
	int height = 24 + 8;
	char buffer[10];
	int divider = 1;   //Change to 60 for release
	int testOffset = 0;
	double temperature, temperatureF, pressure, humidity;

	oled1309 display;
	displayRef = &display;
	displayUnits();

	//Init BME280
	sensor.sensorRef = &sensor;
	sensor.initHardware();

	display.setFont(FreeMono18pt7b);

	printf("%3um\n", elapsedMinutes);
	time(&startTime);
//	sprintf(buffer, "%3d", elapsedMinutes);
//	display.writeString(xstart, ystart, 1, buffer);
    display.clearDisplay();
	display.displayPicture();

	signal(SIGALRM, handle);
	alarm(1);

	for (int i = 0; i < 400; i++) {
	 // for (;;) {
		if (newDataAvail > 0) {
			newDataAvail = 0;
			time(&currentTime);
			newMinutes = ((currentTime - startTime) / divider) + testOffset;
			if (newMinutes != elapsedMinutes) {
				elapsedMinutes = newMinutes;
				printf("%3um\n", elapsedMinutes);
				fflush(stdout);
				sprintf(buffer, "%3dM", elapsedMinutes);
				display.fillRect(xstart, 0, 127, height,
						BLACK);
				memset(display.buffer, 0, 1024);
				display.writeString(xstart, ystart, 1, buffer);
			}
		}
		pixelColor ^= 1;
		display.drawPixel(127, 63, pixelColor);

		temperature = sensor.readTemperature();
		temperatureF = temperature * 9.0 / 5.0 + 32 TEMPOFFSET;
		memset(buffer,0,10);
		printf("Temperature is %.1f\n", temperatureF);

		sprintf(buffer, "%.1fT", temperatureF);
		display.writeString(0, 60, 1, buffer);

		pressure = sensor.readPressure() / 100.0F;
		printf("Pressure in hPa is %f\n", pressure);

		printf("Pressure in inches of mercury is %f\n",
				pressure / 33.8638866667);

		humidity = sensor.readHumidity();
		printf("Humidity is %f \n", humidity);

		display.displayPicture();
		sleep(1);

	}
	printf("Done\n");
	return 0;
}
