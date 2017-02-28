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
#include "FreeMono12pt7b.h"
#include "oled1309.h"
#include "FreeMono18pt7b.h"

/* number of times the handle will run: */
volatile int elapsedSeconds = 0;
volatile int newDataAvail = 0;
volatile int elapsedMinutes = 0;
volatile int pixelColor = 0;

void handle(int sig) {
	elapsedSeconds += 1;
	newDataAvail = 1;
	alarm(1);
}

int main() {
	time_t startTime, currentTime;
	unsigned int elapsedMinutes = 0;
	unsigned int newMinutes = 0;
	int xstart = 0;
	int ystart = 22;
	int height = 22 + 8;
	char buffer[10];
	int divider = 1;   //Change to 60 for release
	int testOffset = 0;

	oled1309 display;
	display.setFont(FreeMono18pt7b);

	printf("%3um\n", elapsedMinutes);
	time(&startTime);
	sprintf(buffer, "%3dm", elapsedMinutes);
	display.writeString(xstart, ystart, 1, buffer);

	display.displayPicture();

	signal(SIGALRM, handle);
	alarm(1);

	for (int i = 0; i < 300; i++) {
	 // for (;;) {
		if (newDataAvail > 0) {
			newDataAvail = 0;
			time(&currentTime);
			newMinutes = ((currentTime - startTime) / divider) + testOffset;
			if (newMinutes != elapsedMinutes) {
				elapsedMinutes = newMinutes;
				printf("%3um\n", elapsedMinutes);
				fflush(stdout);
				sprintf(buffer, "%3dm", elapsedMinutes);
				display.fillRect(xstart, 0, 127, height,
						BLACK);
				display.writeString(xstart, ystart, 1, buffer);
			}
		}
		pixelColor ^= 1;
		display.drawPixel(127, 63, pixelColor);
		display.displayPicture();
		sleep(1);

	}
	printf("Done\n");
	return 0;
}
