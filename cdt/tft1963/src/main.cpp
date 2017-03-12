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

	for (i = 0; i < 3; i++) {

		rpiObj.Write_Command(SSD1963_GET_ADDRESS_MODE);
		rpiObj.Write_Data(0x0040);

		rpiObj.writeString(x, 70, 2, "HelloThere", GREEN);

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
