
#define SEVENINCH

#ifdef SEVENINCH
//7 inch display
#define TFTWIDTH 800
#define TFTHEIGHT 480
#define PICWIDTH 640
#else
//4.3 inch display
#define TFTWIDTH 480
#define TFTHEIGHT 272
#define PICWIDTH 362
#endif


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


void fillWithColor(unsigned short color) {
	int i;

	for(i=0; i<TFTWIDTH*TFTHEIGHT; i++) {
		clsObj->fBuffer[i] = color;
	}
	clsObj->bufftoDisplay();
	sleep(1);
}


void dispPix(int width, int height, const char *fileName) {

	unsigned short buffer[800*480];
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


void show_dir(const char *path, int width, int length) {
	struct dirent *entry;
	DIR *dir = opendir(path);
	char fileName[2048];
	if (dir == NULL) {
		return;
	}

	while ((entry = readdir(dir)) != NULL) {
		strcpy(fileName, path);
		strcat(fileName, entry->d_name);
		if ( (strstr(fileName, "rgb")!= NULL)) {
		dispPix(width, length, fileName);
		printf("%s\n", entry->d_name);
		fflush(stdout);
		sleep(2);
		}
	}

	closedir(dir);
}




int main(int argc, char * argv[]) {


//	DispDraw rpiObj(800,480);
	DispDraw rpiObj(TFTWIDTH,TFTHEIGHT);

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
	memset(rpiObj.fBuffer, 0, sizeof(rpiObj.fBuffer));
	rpiObj.drawPixel(TFTWIDTH-1,TFTHEIGHT-1,WHITE);
	rpiObj.bufftoDisplay();

	fillWithColor(RED);
	fillWithColor(GREEN);
	fillWithColor(BLUE);
	fillWithColor(WHITE);
	sleep(1);

for (int i = 1; i < 3; i++) {
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

	//		rpiObj.writeString(x, y += 200, 4, tBuf, GREEN);

	//		rpiObj.drawPixel(799,479, WHITE);
			rpiObj.drawPixel(TFTWIDTH-1,TFTHEIGHT-1,WHITE);

			rpiObj.bufftoDisplay();

		} catch (int ex) {
			printf("Exception number %d\n", ex);

		}
	}

if (1) {
//	show_dir("/home/andy/pictures/", PICWIDTH, TFTHEIGHT);
	show_dir("/home/andy/images/", PICWIDTH, TFTHEIGHT);

}

	puts("End");
	return 0;
}
