#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include "tft1963.h"
#include "DispDraw.h"
#include "gfxfont.h"
#include "glcdfont.c"
#include "Adafruit_GFX.h"
#include "Fonts/FreeMono12pt7b.h"
#include "Fonts/FreeMono18pt7b.h"
#include "Fonts/FreeMono24pt7b.h"
#include "Fonts/FreeMonoBoldOblique18pt7b.h"
#include "Fonts/FreeSans24pt7b.h"

DispDraw *clsObj;

void writeStringErase(int16_t x, int16_t y, int size, const char *string, unsigned short color) {

    int16_t x1, y1;
    uint16_t w, h;

	clsObj->getTextBounds((char *)string, x, y,
	       &x1,  &y1,  &w,  &h);

//	printf("x1 %d y1 %d w %d, h %d\n", x1, y1, w, h);
//	clsObj->drawRect((unsigned int)x1, (unsigned int)y1, (unsigned int) w, (unsigned int) h, (unsigned int)color);
	clsObj->fillRect((unsigned int)x, (unsigned int)y1, (unsigned int) w + (x1-x), (unsigned int) h, (unsigned int)BLACK);
	clsObj->writeString(x,y,1,"HIGHHFDHHD", RED);

}

int main(int argc, char * argv[]) {

	DispDraw rpiObj;
	unsigned char buffer[100];
	int i, x = 0;

	clsObj = &rpiObj;

	puts("Hello World\n");
	fflush(stdout);
    rpiObj.setFont(&FreeSans24pt7b);
	rpiObj.initRpiHardware();
	rpiObj.Init_ssd1963();
	rpiObj.TFT_FillDisp(BLACK);
	rpiObj.drawDot(1,1,WHITE);

	for(i=0; i<3; i++) {

	rpiObj.TFT_FillDisp(BLACK);
	writeStringErase (x,100,1,"HIGHHFDHHD", RED);
	rpiObj.writeString(x,50,1,"01234.567", WHITE);

	rpiObj.writeString(x,100,1,"012.34567", GREEN);

	rpiObj.writeString(x,150,1,"ABCD..EFG", BLUE);

	rpiObj.writeString(x,200,1,"012.34567", GREEN);
	rpiObj.writeString(x,200,1,"ABCD..EFG", BLUE);
	rpiObj.writeString(x,200,1,"HIGHHFDHHD", RED);
	writeStringErase (x,200,1,"HIGHHFDHHD", RED);

	}


	puts("End");
	return 0;
}
