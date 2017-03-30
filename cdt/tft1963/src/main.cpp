#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
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

DispDraw *clsObj;

int main(int argc, char * argv[]) {

	DispDraw rpiObj;

	int i=0, x = 5;
	int y = 30;
	int j;
	char cBuffer[30];
	char tBuf[20] = { 0 };
	char iBuf[20];

	clsObj = &rpiObj;

	fflush(stdout);
	rpiObj.setFont(&FreeSans24pt7b);
	rpiObj.initRpiHardware();
	rpiObj.Init_ssd1963();

	for (int i = 1; i < 400; i++) {
//	while(1) {
		i+=1;
		y = 30;
		memset(rpiObj.fBuffer, 0x00, sizeof(rpiObj.fBuffer));

        iBuf[0] = '\0';
        cBuffer[0] = '\0';
		sprintf(iBuf, "   %d", i);
		sprintf(tBuf, "%s", iBuf);
        strcat(cBuffer, tBuf);

		try {
			rpiObj.writeString(x, 30, 1, "OK", PURPLE);

			rpiObj.writeString(x, y += 40, 1, "Hello", BLUE);

			rpiObj.writeString(x, y += 40, 1, "HIGHHFDHHD", RED);

			rpiObj.writeString(x, y += 40, 1, "012.34567", YELLOW);

			rpiObj.writeString(x, y += 80, 2, tBuf, GREEN);

			rpiObj.bufftoDisplay();
			sleep(0.1);
		} catch (int ex) {
			printf("Exception number %d\n", ex);

		}

	}

	puts("End");
	return 0;
}
