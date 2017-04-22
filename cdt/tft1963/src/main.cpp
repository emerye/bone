#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <dirent.h>
#include <sys/types.h>

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


void list_dir(const char *path) {
	struct dirent *entry;
	DIR *dir = opendir(path);
	if (dir == NULL) {
		return;
	}

	while ((entry = readdir(dir)) != NULL) {
		printf("%s\n", entry->d_name);
	}

	closedir(dir);
}


void dispPix(int width, int height, const char *fileName) {

	unsigned short buffer[width*height];
	int i,j;

	int status = clsObj->rgb565(fileName, width, height, buffer);
	memset(clsObj->fBuffer, 0x00, sizeof(clsObj->fBuffer));
    for (i=0; i<height; i++) {
    	for (j=0; j<width; j++) {
    		clsObj->fBuffer[i*clsObj->width + j] = buffer[i*width+j];
    	}
    }
    clsObj->bufftoDisplay();
}

int main(int argc, char * argv[]) {

	DispDraw rpiObj(800,480);

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
	memset(rpiObj.fBuffer, 0x00, sizeof(rpiObj.fBuffer));
	rpiObj.bufftoDisplay();

	system("ls -al");
	list_dir("/home/andy/bone/images/image///");

for (int i = 1; i < 2; i++) {
 //  while(1) {
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

			rpiObj.writeString(x, y += 200, 4, tBuf, GREEN);

			rpiObj.drawPixel(799,479, WHITE);

			rpiObj.bufftoDisplay();
			sleep(0.1);
		} catch (int ex) {
			printf("Exception number %d\n", ex);

		}
	}

	dispPix(640, 480, "/home/andy/bone/images/image/paris.rgb");
//	sleep(1);
	dispPix(640, 480, "/home/andy/bone/images/image/nwharrahs.rgb");

	dispPix(640, 480, "/home/andy/bone/images/image/paris.rgb");
	sleep(1);

	puts("End");
	return 0;
}
