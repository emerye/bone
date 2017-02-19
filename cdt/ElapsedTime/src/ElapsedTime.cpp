//============================================================================
// Name        : ElapsedTime.cpp
// Author      : Andy Hughes
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>
#include "oled1309.h"

/* number of times the handle will run: */
volatile int elapsedSeconds = 0;
volatile int newDataAvail = 0;
volatile int elapsedMinutes = 0;


void handle(int sig) {
	elapsedSeconds += 1;
	newDataAvail = 1;
    alarm(1);
}


int main() {
	time_t startTime, currentTime;
	unsigned int elapsedMinutes = 0;
	unsigned int newMinutes;

	oled1309 display;

	time(&startTime);
	signal(SIGALRM, handle);
	alarm(1);
	printf("%3d m\n", elapsedMinutes);
	fflush(stdout);
	while (1) {
		if (newDataAvail > 0) {
			newDataAvail = 0;
			time(&currentTime);
			newMinutes = (currentTime - startTime) / 60;
			if (newMinutes != elapsedMinutes) {
				elapsedMinutes = newMinutes;
				printf("%3u m\n", elapsedMinutes);
				fflush(stdout);
			}
		}
		sleep(1);
	}
	printf("done\n");
	return 0;
}
