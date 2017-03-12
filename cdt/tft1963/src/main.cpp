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

	int i, x = 0;

	puts("Hello World\n");
	fflush(stdout);
	rpiObj.setFont(&FreeSans24pt7b);

	rpiObj.initRpiHardware();
	rpiObj.Init_ssd1963();
	rpiObj.TFT_FillDisp(BLACK);
	rpiObj.drawDot(1, 1, WHITE);

	for (i = 0; i < 1; i++) {

		rpiObj.drawDot(20,20,BLUE);

		rpiObj.drawDot(22,20,RED);
		rpiObj.drawDot(24,20,GREEN);

		rpiObj.drawDot(26,20,PURPLE);
		rpiObj.drawDot(27,20,PURPLE);

		int xOff = 0;

		rpiObj.fillRect(xOff+=20, 200, 50+xOff, 50, 0xffff);
		rpiObj.fillRect(xOff+=20, 200, 50+xOff, 50, 0x0F00);
		rpiObj.fillRect(xOff+=20, 200, 50+xOff, 50, 0x000F);
		rpiObj.fillRect(xOff+=20, 200, 50+xOff, 50, 0x001F);
		rpiObj.fillRect(xOff+=20, 200, 50+xOff, 50, WHITE);
	//	rpiObj.fillRect(xOff+=20, 200, 50+xOff, 50, 0xffff);


		rpiObj.writeString(x, 40, 1, "Hello", RED | GREEN | BLUE);

	//	rpiObj.writeStringErase(x, 120, 1, "HIGHHFDHHD", RED, BLACK, 0);

		rpiObj.writeString(x, 120, 1, "012.34567", GREEN);

		rpiObj.writeString(x, 170, 1, "ABCD..EFG", BLUE);

		rpiObj.setFont(&FreeSans12pt7b);
		rpiObj.writeString(50, 200, 1, "ABCD..EFG", BLUE);
		rpiObj.setFont(&FreeSans24pt7b);

	}

	puts("End");
	return 0;
}
