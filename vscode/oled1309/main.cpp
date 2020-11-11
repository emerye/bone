#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <time.h>
#include "oled1309.h"
#include "gfxfont.h"
#include "glcdfont.h"
#include "FreeMono24pt7b.h"
#include "FreeMono12pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeMono9pt7b.h"
#include "Fonts/FreeSansBold12pt7b.h"
#include "Fonts/TomThumb.h"
#include "pic.h"


void test(oled1309 display)
{
	int xstart = 0;
	int ystart = 18;
	int width = 40;
	int height = 22;
	int i;

	//	display.drawFastHLine(0, 63, 127, WHITE);
	sleep(2);
	//   drawFastVLine(15, 8, 30, WHITE);
	//  drawCircle(32,32,16,WHITE);
	//   drawLine(0, 0, 60, 100,
	//    WHITE);
	//  drawRect(10,10,50,50,WHITE);
	//  drawPixel(100,50,WHITE);
	//  fillRect(40,10, 50,20,WHITE);
	//	drawChar(80, 40, 'C', WHITE, BLACK, 1);
	//	drawChar(87, 40, 'D', WHITE, BLACK, 1);
	//	drawCharCustom(70, 30, 'H',
	//	WHITE, BLACK, 1);
	//	drawCharCustom(90, 30, 'G',
	//	WHITE, BLACK, 1);
	// writeString(stText);

	memset(display.buffer, 0, sizeof(display.buffer));

	int j;
	unsigned char byte;

	//Start of display loop
	for (i = 0; i < 1; i++)
	{
		memset(display.buffer, 0x55, sizeof(display.buffer));
		display.displayPicture();
		sleep(2);

		for (j = 0; j < sizeof(pic); j++)
		{
			display.buffer[j] = pic[j];
		}
		display.displayPicture();
		sleep(3);

		memset(display.buffer, 0, sizeof(display.buffer));
		display.displayPicture();

		display.setFont(TomThumb);
		display.fillRect(xstart, 37, 130, height, BLACK);
		display.writeString(xstart, 18, 1, "ABCDEFGHI");
		display.setFont(FreeMono9pt7b);
		display.writeString(xstart, 36, 1, "123456789");
		display.setFont(FreeSerif12pt7b);
		display.writeString(xstart, 54, 1, "abcdefgh");
		display.displayPicture();
		sleep(1);
		memset(display.buffer, 0, sizeof(display.buffer));
		display.displayPicture();
		display.setFont(FreeMono9pt7b);
		display.writeString(0, 18, 1, "ABCDEFGHI");
		//display.writeString(0, 36, 1, "123456789");
		display.fillRect(xstart, 37, 130, height, BLACK);
		display.writeString(0, 44, 1, "11/05/2020");
		display.displayPicture();
		sleep(1);
	}
}

int main(int argc, char **argv)
{
	printf("Started\n");
	fflush(stdout);
	oled1309 display;

	display.fillRect(0, 0, 127, 63, WHITE);
	display.displayPicture();
	sleep(2);
	memset(display.buffer, 0, sizeof(display.buffer));
	for (int i = 0; i < sizeof(display.buffer); i++)
	{
		display.buffer[i] = pic[i];
	}
	display.displayPicture();
	sleep(4);
	display.setFont(FreeSans12pt7b);
	test(display);
	puts("End");
	return 0;
}
