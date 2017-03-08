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

int main(int argc, char * argv[]) {

	DispDraw rpiObj;
	unsigned char buffer[100];

	puts("Hello World\n");
	fflush(stdout);
    rpiObj.setFont(&FreeMono24pt7b);
	rpiObj.initRpiHardware();
	rpiObj.Init_ssd1963();
	rpiObj.TFT_FillDisp(BLUE);
	rpiObj.TFT_FillDisp(WHITE);
	rpiObj.TFT_FillDisp(RED);
//	rpiObj.TFT_FillDisp(GREEN);

//	rpiObj.drawDot(100,100,WHITE);
	rpiObj.drawDot(1,1,WHITE);
//	rpiObj.drawBox(10,10,200,200,BLUE);
//	rpiObj.drawBox(50,150,400,269,GREEN);
	rpiObj.drawBox(0,0,50,100,WHITE);
	rpiObj.drawHorzLine(0,400,171, 0xFFFE);
	rpiObj.drawHorzLine(0,400,121, PURPLE);
	rpiObj.drawVertLine(10,260, 100, WHITE);
	rpiObj.drawCircle(200,100, 40, BLUE);
	rpiObj.drawChar(140,100,'F', WHITE, BLACK, 3);

	puts("End");
	return 0;
}
