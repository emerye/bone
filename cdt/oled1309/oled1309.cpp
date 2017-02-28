/*
 * oled1309.cpp
 *
 *  Created on: 11 Feb 2017
 *      Author: root
 */

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <inttypes.h>
#include "oled1309.h"
#include "SSD1309.h"
#include "glcdfont.h"
#include "gfxfont.h"
#include "FreeMono24pt7b.h"
#include "FreeMono12pt7b.h"
#include "Fonts/FreeSerif12pt7b.h"
#include "Fonts/FreeSans12pt7b.h"


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define XLevelL         0x00
#define XLevelH         0x10
#define XLevel          ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column      128
#define Max_Row         64
#define Brightness      0x8F
#define ssd1306_swap(a, b) { int16_t t = a; a = b; b = t; }


//unsigned char spiBuffer[20];	//Buffer to hold SPI data

int spiFD;
int rotation = 0;

GFXfont gfxFont;

unsigned char picBuf[1024];

#define ssd1306_swap(a, b) { int16_t t = a; a = b; b = t; }
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }


const unsigned char pic[]=
{/*--ER-OLED015-2.bmp  --*//*--  128x64  --*/
0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x21, 0xFD, 0x21, 0xE1, 0x01, 0xF9, 0x49, 0x49, 0x49, 0xF9,
		0x01, 0x01, 0x01, 0x01, 0xFD, 0x45, 0x45, 0x45, 0x45, 0x45, 0x45, 0xFD,
		0x01, 0x01, 0x09, 0x89, 0xC9, 0xA9, 0x99, 0xED, 0x89, 0x89, 0x89, 0x89,
		0x09, 0x01, 0x11, 0x11, 0x11, 0x11, 0xF1, 0x55, 0x59, 0x51, 0xD1, 0x19,
		0x11, 0x01, 0x29, 0xA9, 0xF9, 0xA5, 0x25, 0x01, 0x49, 0x91, 0x01, 0xFD,
		0x01, 0x01, 0x11, 0x11, 0xFD, 0x91, 0x01, 0x91, 0x91, 0xFD, 0x91, 0x91,
		0x11, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x07,
		0x00, 0x1F, 0x10, 0x17, 0x12, 0x12, 0x12, 0x17, 0x18, 0x00, 0x00, 0x00,
		0x1F, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1F, 0x00, 0x00, 0x08, 0x08,
		0x04, 0x02, 0x10, 0x1F, 0x00, 0x01, 0x02, 0x04, 0x08, 0x00, 0x10, 0x10,
		0x08, 0x06, 0x01, 0x10, 0x10, 0x10, 0x0F, 0x00, 0x00, 0x00, 0x06, 0x01,
		0x1F, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x1F, 0x01, 0x00, 0x11, 0x11,
		0x1F, 0x00, 0x10, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x10, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFE, 0x92, 0x92, 0x92, 0x92,
		0x80, 0x70, 0x2C, 0x22, 0x2C, 0x70, 0x80, 0x4C, 0x92, 0x92, 0x64, 0x00,
		0x02, 0x02, 0xFE, 0x02, 0x02, 0x00, 0xFE, 0x12, 0x12, 0x32, 0xCC, 0x00,
		0x00, 0xFE, 0x00, 0x00, 0x4C, 0x92, 0x92, 0x64, 0x00, 0x00, 0xFE, 0x00,
		0x00, 0xFE, 0x0C, 0x10, 0x60, 0xFE, 0x00, 0x00, 0x7C, 0x82, 0x82, 0x92,
		0x74, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0xFE, 0x02, 0x02, 0x00, 0xFE,
		0x92, 0x92, 0x92, 0x92, 0x00, 0x7C, 0x82, 0x82, 0x82, 0x44, 0x00, 0x00,
		0xFE, 0x10, 0x10, 0x10, 0xFE, 0x00, 0x00, 0x80, 0x00, 0x00, 0x7C, 0x82,
		0x82, 0x82, 0x44, 0x00, 0x00, 0x7C, 0x82, 0x82, 0x82, 0x7C, 0x00, 0x00,
		0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0xFE, 0x80, 0x80, 0x80, 0x02, 0x02,
		0xFE, 0x02, 0x02, 0x00, 0xFE, 0x82, 0x82, 0x82, 0x7C, 0x00, 0xFF, 0xFF,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x20, 0x00, 0x10, 0x08,
		0xC8, 0xC8, 0x48, 0x48, 0x48, 0x08, 0xC8, 0xC8, 0x48, 0x48, 0xC8, 0x88,
		0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x88, 0xC8, 0x48, 0xC8, 0x88, 0x08,
		0x08, 0xC8, 0xC8, 0x08, 0x08, 0x08, 0x08, 0x08, 0xC8, 0xC8, 0x48, 0x48,
		0x48, 0x08, 0xC8, 0xC8, 0x48, 0x48, 0xC8, 0x88, 0x08, 0x88, 0xC8, 0x48,
		0xC8, 0x88, 0x08, 0x08, 0x88, 0xC8, 0xC8, 0x08, 0x08, 0x08, 0xC8, 0xC8,
		0x48, 0x48, 0x08, 0x08, 0x08, 0x08, 0x08, 0x88, 0xC8, 0x48, 0xC8, 0x88,
		0x08, 0x10, 0x20, 0x20, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x0F, 0x30, 0x40, 0x00, 0x80, 0x00, 0x3F, 0x3F, 0x22, 0x22,
		0x22, 0x00, 0x3F, 0x3F, 0x04, 0x0C, 0x1F, 0x33, 0x20, 0x08, 0x08, 0x08,
		0x00, 0x0F, 0x1F, 0x30, 0x20, 0x30, 0x1F, 0x0F, 0x00, 0x3F, 0x3F, 0x20,
		0x20, 0x20, 0x20, 0x00, 0x3F, 0x3F, 0x22, 0x22, 0x22, 0x00, 0x3F, 0x3F,
		0x20, 0x20, 0x3F, 0x1F, 0x00, 0x1F, 0x3F, 0x20, 0x3F, 0x1F, 0x00, 0x03,
		0x01, 0x3F, 0x3F, 0x00, 0x00, 0x17, 0x33, 0x22, 0x3E, 0x1C, 0x00, 0x08,
		0x08, 0x08, 0x00, 0x30, 0x38, 0x2C, 0x27, 0x23, 0x00, 0x80, 0x40, 0x40,
		0x30, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x7F, 0x00, 0x00,
		0x00, 0x40, 0x00, 0x2C, 0x47, 0x45, 0x39, 0x00, 0x03, 0x00, 0x03, 0x00,
		0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x40, 0x20, 0xF0, 0x00, 0x00, 0x20,
		0x10, 0x90, 0x60, 0x00, 0x60, 0x90, 0x90, 0x60, 0x00, 0x00, 0x30, 0x40,
		0xC0, 0x30, 0x00, 0xE1, 0x91, 0x91, 0x21, 0x01, 0x01, 0xC1, 0x21, 0xF1,
		0x01, 0x01, 0x01, 0x81, 0x41, 0x41, 0x41, 0xF1, 0x01, 0x81, 0x41, 0x41,
		0x41, 0x81, 0x41, 0xF1, 0x41, 0x01, 0x81, 0x41, 0x41, 0x41, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xF1,
		0x91, 0x91, 0xF1, 0x01, 0x01, 0xC1, 0x31, 0x01, 0x61, 0x91, 0x91, 0x21,
		0x01, 0x01, 0x01, 0x01, 0x01, 0xF1, 0x01, 0xC1, 0x41, 0x41, 0x81, 0x41,
		0xE1, 0x41, 0x01, 0x81, 0x41, 0x40, 0x80, 0x00, 0xC0, 0x40, 0x40, 0x40,
		0xE0, 0x50, 0x00, 0x40, 0x40, 0x40, 0xC0, 0x00, 0x80, 0x40, 0x40, 0x80,
		0x00, 0x80, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
		0x00, 0x00, 0x80, 0x80, 0x07, 0x00, 0x80, 0x06, 0x05, 0x84, 0x84, 0x00,
		0x83, 0x84, 0x04, 0x03, 0x80, 0x00, 0x06, 0x81, 0x81, 0x06, 0x80, 0x83,
		0x04, 0x04, 0x83, 0x00, 0x03, 0x82, 0x82, 0x07, 0x02, 0x00, 0x00, 0xE3,
		0xE4, 0x84, 0x84, 0x87, 0x00, 0x03, 0x84, 0x84, 0x04, 0x03, 0x80, 0x87,
		0x84, 0x80, 0x04, 0x05, 0x05, 0x82, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x80, 0x80, 0x80, 0xE0, 0xE0, 0x00, 0xA0, 0xA7, 0x00, 0x00, 0x80, 0x80,
		0x86, 0x81, 0x00, 0x00, 0x82, 0x84, 0x84, 0x83, 0x80, 0x00, 0x00, 0xE0,
		0xE0, 0x07, 0x00, 0x87, 0x80, 0x80, 0x07, 0x80, 0x87, 0x04, 0x00, 0x03,
		0x85, 0x85, 0x05, 0x00, 0x07, 0x00, 0x80, 0x80, 0x87, 0x00, 0x00, 0x06,
		0x85, 0x85, 0x87, 0x80, 0x03, 0x04, 0x84, 0x82, 0x80, 0x83, 0x85, 0x85,
		0x85, 0x80, 0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x80, 0x83, 0x9F,
		0x9C, 0x8F, 0x83, 0x8F, 0x9C, 0x9F, 0x83, 0x80, 0x83, 0x9F, 0x9C, 0x8F,
		0x83, 0x8F, 0x9C, 0x9F, 0x83, 0x80, 0x83, 0x9F, 0x9C, 0x8F, 0x83, 0x8F,
		0x9C, 0x9F, 0x83, 0x80, 0x98, 0x98, 0x80, 0x9F, 0x9F, 0x90, 0x90, 0x9F,
		0x8F, 0x80, 0x8F, 0x9F, 0x90, 0x90, 0x9F, 0x9F, 0xC0, 0xC7, 0xFF, 0xB8,
		0x9F, 0x87, 0x80, 0x84, 0x84, 0x84, 0x80, 0x8F, 0x9F, 0x90, 0x90, 0x9F,
		0x9F, 0x80, 0x9F, 0x9F, 0x80, 0x8B, 0x9B, 0x96, 0x96, 0x9D, 0x8D, 0x80,
		0xFF, 0xFF, 0x90, 0x90, 0x9F, 0x8F, 0x80, 0x9F, 0x9F, 0x80, 0x8D, 0x9E,
		0x92, 0x9F, 0x9F, 0xC0, 0xC7, 0xFF, 0xB8, 0x9F, 0x87, 0x80, 0x98, 0x98,
		0x80, 0x8F, 0x9F, 0x90, 0x99, 0x89, 0x80, 0x8F, 0x9F, 0x90, 0x90, 0x9F,
		0x8F, 0x80, 0x9F, 0x9F, 0x80, 0x80, 0x9F, 0x9F, 0x80, 0x80, 0x9F, 0x9F,
		0x80, 0x80, 0xFF, };

//Constructor
oled1309::oled1309() {
	setFont(FreeMono24pt7b);
	wiringPiSetup();
	init_Hardware();
	initDisplay();
	setContrast(0xFF);
	clearDisplay();

}

oled1309::~oled1309() {
}

void oled1309::setFont(const GFXfont font) {
	gfxFont = font;
}

int oled1309::width() {
	return SSD1309_LCDWIDTH;
}

int oled1309::height() {
	return (SSD1309_LCDHEIGHT);
}

void oled1309::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
		uint16_t color) {
	// Update in subclasses if desired!
	for (int16_t i = x; i < x + w; i++) {
		drawFastVLine(i, y, h, color);
	}
}

extern "C"
// Draw a circle outline
void oled1309::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0, y0 + r, color);
	drawPixel(x0, y0 - r, color);
	drawPixel(x0 + r, y0, color);
	drawPixel(x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}

void oled1309::drawCircleHelper(int16_t x0, int16_t y0, int16_t r,
		uint8_t cornername, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x, color);
			drawPixel(x0 - x, y0 - y, color);
		}
	}
}

void oled1309::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	drawFastVLine(x0, y0 - r, 2 * r + 1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void oled1309::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
		uint8_t cornername, int16_t delta, uint16_t color) {

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
		}
	}
}

// Bresenham's algorithm - thx wikpedia
void oled1309::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
		uint16_t color) {
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		_swap_int16_t(x0, y0);
		_swap_int16_t(x1, y1);
	}

	if (x0 > x1) {
		_swap_int16_t(x0, x1);
		_swap_int16_t(y0, y1);
	}
	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, color);
		} else {
			drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

/** Clear display
 *
 */
void oled1309::clearDisplay(void) {

    memset(buffer, 0, 1024);
	displayPicture();
}

// Draw a rectangle
void oled1309::drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
		uint16_t color) {
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y + h - 1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x + w - 1, y, h, color);
}

// the most basic function, set a single pixel
void oled1309::drawPixel(int16_t x, int16_t y, uint16_t color) {
	if ((x < 0) || (x >= width()) || (y < 0) || (y >= height()))
		return;

	// check rotation, move pixel around if necessary
	switch (rotation) {
	case 1:
		ssd1306_swap(x, y)
		;
		x = WIDTH - x - 1;
		break;
	case 2:
		x = WIDTH - x - 1;
		y = HEIGHT - y - 1;
		break;
	case 3:
		ssd1306_swap(x, y)
		;
		y = HEIGHT - y - 1;
		break;
	}

	// x is which column
	switch (color) {
	case WHITE:
		buffer[x + (y / 8) * SSD1309_LCDWIDTH] |= (1 << (y & 7));
		break;
	case BLACK:
		buffer[x + (y / 8) * SSD1309_LCDWIDTH] &= ~(1 << (y & 7));
		break;
	case INVERSE:
		buffer[x + (y / 8) * SSD1309_LCDWIDTH] ^= (1 << (y & 7));
		break;
	}
}

void oled1309::drawFastVLine(int16_t x, int16_t __y, int16_t __h,
		uint16_t color) {

	// do nothing if we're off the left or right side of the screen
	if (x < 0 || x >= WIDTH) {
		return;
	}
	// make sure we don't try to draw below 0
	if (__y < 0) {
		// __y is negative, this will subtract enough from __h to account for __y being 0
		__h += __y;
		__y = 0;

	}
	// make sure we don't go past the height of the display
	if ((__y + __h) > HEIGHT) {
		__h = (HEIGHT - __y);
	}
	// if our height is now negative, punt
	if (__h <= 0) {
		return;
	}
	// this display doesn't need ints for coordinates, use local byte registers for faster juggling
	register uint8_t y = __y;
	register uint8_t h = __h;

	// set up the pointer for fast movement through the buffer
	register uint8_t *pBuf = buffer;
	// adjust the buffer pointer for the current row
	pBuf += ((y / 8) * SSD1309_LCDWIDTH);
	// and offset x columns in
	pBuf += x;

	// do the first partial byte, if necessary - this requires some masking
	register uint8_t mod = (y & 7);
	if (mod) {
		// mask off the high n bits we want to set
		mod = 8 - mod;

		// note - lookup table results in a nearly 10% performance improvement in fill* functions
		// register uint8_t mask = ~(0xFF >> (mod));
		static uint8_t premask[8] = { 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC,
				0xFE };
		register uint8_t mask = premask[mod];

		// adjust the mask if we're not going to reach the end of this byte
		if (h < mod) {
			mask &= (0XFF >> (mod - h));
		}

		switch (color) {
		case WHITE:
			*pBuf |= mask;
			break;
		case BLACK:
			*pBuf &= ~mask;
			break;
		case INVERSE:
			*pBuf ^= mask;
			break;
		}

		// fast exit if we're done here!
		if (h < mod) {
			return;
		}

		h -= mod;

		pBuf += SSD1309_LCDWIDTH;
	}
	// write solid bytes while we can - effectively doing 8 rows at a time
	if (h >= 8) {
		if (color == INVERSE) { // separate copy of the code so we don't impact performance of the black/white write version with an extra comparison per loop
			do {
				*pBuf = ~(*pBuf);

				// adjust the buffer forward 8 rows worth of data
				pBuf += SSD1309_LCDWIDTH;

				// adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
				h -= 8;
			} while (h >= 8);
		} else {
			// store a local value to work with
			register uint8_t val = (color == WHITE) ? 255 : 0;

			do {
				// write our value in
				*pBuf = val;

				// adjust the buffer forward 8 rows worth of data
				pBuf += SSD1309_LCDWIDTH;

				// adjust h & y (there's got to be a faster way for me to do this, but this should still help a fair bit for now)
				h -= 8;
			} while (h >= 8);
		}
	}

	// now do the final partial byte, if necessary
	if (h) {
		mod = h & 7;
		// this time we want to mask the low bits of the byte, vs the high bits we did above
		// register uint8_t mask = (1 << mod) - 1;
		// note - lookup table results in a nearly 10% performance improvement in fill* functions
		static uint8_t postmask[8] = { 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F,
				0x7F };
		register uint8_t mask = postmask[mod];
		switch (color) {
		case WHITE:
			*pBuf |= mask;
			break;
		case BLACK:
			*pBuf &= ~mask;
			break;
		case INVERSE:
			*pBuf ^= mask;
			break;
		}
	}
}

void oled1309::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
	// Do bounds/limit checks
	if (y < 0 || y >= HEIGHT) {
		return;
	}

	// make sure we don't try to draw below 0
	if (x < 0) {
		w += x;
		x = 0;
	}
	// make sure we don't go off the edge of the display
	if ((x + w) > WIDTH) {
		w = (WIDTH - x);
	}
	// if our width is now negative, punt
	if (w <= 0) {
		return;
	}

	// set up the pointer for  movement through the buffer
	register uint8_t *pBuf = buffer;
	// adjust the buffer pointer for the current row
	pBuf += ((y / 8) * SSD1309_LCDWIDTH);
	// and offset x columns in
	pBuf += x;

	register uint8_t mask = 1 << (y & 7);

	switch (color) {
	case WHITE:
		while (w--) {
			*pBuf++ |= mask;
		}
		;
		break;
	case BLACK:
		mask = ~mask;
		while (w--) {
			*pBuf++ &= mask;
		}
		;
		break;
	case INVERSE:
		while (w--) {
			*pBuf++ ^= mask;
		}
		;
		break;
	}
}

void oled1309::invertDisplay(uint8_t i) {
	if (i) {
		sendByte(COMMAND, SSD1309_INVERTDISPLAY);
	} else {
		sendByte(COMMAND, SSD1309_NORMALDISPLAY);
	}
}

int oled1309::SendSPIBlock(enum cmd cmdType, unsigned char *spiData,
		int numBytes) {
	int error;
	unsigned char spiBuffer[20];

	memcpy(spiBuffer, spiData, numBytes);

	if (cmdType == COMMAND) {
		digitalWrite(DCLINE, LOW);
	} else {
		digitalWrite(DCLINE, HIGH);
	}

	error = wiringPiSPIDataRW(0, spiBuffer, numBytes);
	if (error == -1) {
		printf("Error writing SPI %d\n", error);
		perror("SPI Write");
	}
	return error;
}


int oled1309::sendByte(enum cmd cmdType, int data) {
	unsigned char spiBuffer[20];

	spiBuffer[0] = (unsigned char) (data & 0xFF);
	return (SendSPIBlock(cmdType, spiBuffer, 1));
}

void oled1309::drawFastHLineInternal(int16_t x, int16_t y, int16_t w,
		uint16_t color) {
	bool bSwap = false;
	int rotation = 0;

	switch (rotation) {
	case 0:
		// 0 degree rotation, do nothing
		break;
	case 1:
		// 90 degree rotation, swap x & y for rotation, then invert x
		bSwap = true;
		ssd1306_swap(x, y)
		;
		x = WIDTH - x - 1;
		break;
	case 2:
		// 180 degree rotation, invert x and y - then shift y around for height.
		x = WIDTH - x - 1;
		y = HEIGHT - y - 1;
		x -= (w - 1);
		break;
	case 3:
		// 270 degree rotation, swap x & y for rotation, then invert y  and adjust y for w (not to become h)
		bSwap = true;
		ssd1306_swap(x, y)
		;
		y = HEIGHT - y - 1;
		y -= (w - 1);
		break;
	}

	if (bSwap) {
		//   drawFastVLineInternal(x, y, w, color);
	} else {
		//   drawFastHLineInternal(x, y, w, color);
	}
}

void oled1309::initDisplay() {
	digitalWrite(RESETLINE, HIGH);
	digitalWrite(RESETLINE, LOW);
	usleep(15000);
	digitalWrite(RESETLINE, HIGH);

	usleep(1000);

	sendByte(COMMAND, 0xFD);	// Set Command Lock
	sendByte(COMMAND, 0x12);	//   Default => 0x12
	//     0x12 => Driver IC interface is unlocked from entering command.
	//     0x16 => All Commands are locked except 0xFD.

	sendByte(COMMAND, SSD1309_DISPLAYOFF);	//--turn off oled panel

	sendByte(COMMAND, SSD1309_SETDISPLAYCLOCKDIV);// Set Display Clock Divide Ratio / Oscillator Frequency
	sendByte(COMMAND, 0xa0);	//   Default => 0x70
	//     D[3:0] => Display Clock Divider
	//     D[7:4] => Oscillator Frequency

	sendByte(COMMAND, SSD1309_SETMULTIPLEX);	//--set multiplex ratio(1 to 64)
	sendByte(COMMAND, 0x3f);	//--1/64 duty

	sendByte(COMMAND, SSD1309_SETDISPLAYOFFSET);	//Set Display Offset
	sendByte(COMMAND, 0x00);

	sendByte(COMMAND, 0x40);	// Set Display Start Line

	sendByte(COMMAND, SSD1309_MEMORYMODE);	// Set Memory Addressing Mode
	sendByte(COMMAND, 0x02);	//   Default => 0x02
	//     0x00 => Horizontal Addressing Mode
	//     0x01 => Vertical Addressing Mode
	//     0x02 => Page Addressing Mode

	sendByte(COMMAND, 0xa1);	//--set segment re-map 128 to 0

	sendByte(COMMAND, 0xC8);	//--Set COM Output Scan Direction 64 to 1

	sendByte(COMMAND, 0xda);	//--Set COM Pins Hardware Configuration
	sendByte(COMMAND, 0x12);//     Disable COM Left/Right Re-Map   Alternative COM Pin Configuration

	sendByte(COMMAND, SSD1309_SETCONTRAST);	//--set contrast control register
	sendByte(COMMAND, 0x8F);    // Maximum

	sendByte(COMMAND, 0xD9);	// Set Pre-Charge Period
	sendByte(COMMAND, 0x25);//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
	//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
	//     D[7:4] => Phase 2 Period in 1~15 Display Clocks

	sendByte(COMMAND, 0xDB);	// Set VCOMH Deselect Level
	sendByte(COMMAND, 0x34);	//   Default => 0x34 (0.78*VCC)

	sendByte(COMMAND, 0xad);	//--Set Master Configuration
	sendByte(COMMAND, 0x8e);	//--

	sendByte(COMMAND, 0xd8);//--Set Area Color Mode On/Off & Low Power Display Mode
	sendByte(COMMAND, 0x05);	//

	sendByte(COMMAND, 0xa4);	//Disable Entire Display On

	sendByte(COMMAND, 0xa6);	//--set normal display

	sendByte(COMMAND, 0xaf);	//--turn on oled panel
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void oled1309::Set_Start_Column(unsigned char d) {
	sendByte(COMMAND, 0x00 + d % 16); // Set Lower Column Start Address for Page Addressing Mode
	//   Default => 0x00
	sendByte(COMMAND, 0x10 + d / 16); // Set Higher Column Start Address for Page Addressing Mode
									  //   Default => 0x10
}

void oled1309::Set_Addressing_Mode(unsigned char d) {
	sendByte(COMMAND, 0x20);                    // Set Memory Addressing Mode
	sendByte(COMMAND, d);                       //   Default => 0x02
	//     0x00 => Horizontal Addressing Mode
	//     0x01 => Vertical Addressing Mode
	//     0x02 => Page Addressing Mode
}

void oled1309::Set_Column_Address(unsigned char a, unsigned char b) {
	sendByte(COMMAND, 0x21);                    // Set Column Address
	sendByte(COMMAND, a);            //   Default => 0x00 (Column Start Address)
	sendByte(COMMAND, b);              //   Default => 0x7F (Column End Address)
}

void oled1309::Set_Page_Address(unsigned char a, unsigned char b) {
	sendByte(COMMAND, 0x22);                    // Set Page Address
	sendByte(COMMAND, a);              //   Default => 0x00 (Page Start Address)
	sendByte(COMMAND, b);                //   Default => 0x07 (Page End Address)
}

void oled1309::displayPicture(void) {
	unsigned char *picture;
	unsigned char i, j;
	picture = buffer;

	for (i = 0; i < 0x08; i++) {
		//Set_Start_Page(i);
		sendByte(COMMAND, 0xB0 | i);
		Set_Start_Column(XLevel);
		for (j = 0; j < 0x80; j++) {
			sendByte(DATA, *picture);
			picture++;
		}
	}
}

void oled1309::init_Hardware(void) {
	int status;

	pinMode(RESETLINE, OUTPUT);
	pinMode(DCLINE, OUTPUT);
	status = wiringPiSPISetup(0, 2000000);
	if (status < 1) {
		perror("Error opening SPI");
	}
}

void oled1309::setContrast(unsigned char level) {
	unsigned char buffer[3];
	buffer[0] = 0x81;
	buffer[1] = level;
	SendSPIBlock(COMMAND, buffer, 2);
}

// Draw a custom Font Character
void oled1309::drawCharCustom(int16_t x, int16_t y, unsigned char c,
		uint16_t color, uint16_t bg, uint8_t size) {

	// Character is assumed previously filtered by write() to eliminate
	// newlines, returns, non-printable characters, etc.  Calling drawChar()
	// directly with 'bad' characters of font may cause mayhem!

	//  c -= font(&gfxFont->first);

	c -= gfxFont.first;
	//  GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
	GFXglyph glyph = gfxFont.glyph[c];
	//    uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);
	uint8_t *bitmap = (uint8_t *) (gfxFont.bitmap);

	uint16_t bo = glyph.bitmapOffset;
	uint8_t w = glyph.width;
	uint8_t h = glyph.height;
//	int8_t xa = glyph.xAdvance;
	int8_t xo = glyph.xOffset;
	int8_t yo = glyph.yOffset;
	uint8_t xx, yy, bits, bit = 0;
	int16_t xo16, yo16;

	if (size > 1) {
		xo16 = xo;
		yo16 = yo;
	}

	// Todo: Add character clipping here

	// NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
	// THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
	// has typically been used with the 'classic' font to overwrite old
	// screen contents with new data.  This ONLY works because the
	// characters are a uniform size; it's not a sensible thing to do with
	// proportionally-spaced fonts with glyphs of varying sizes (and that
	// may overlap).  To replace previously-drawn text when using a custom
	// font, use the getTextBounds() function to determine the smallest
	// rectangle encompassing a string, erase the area with fillRect(),
	// then draw new text.  This WILL unfortunately 'blink' the text, but
	// is unavoidable.  Drawing 'background' pixels will NOT fix this,
	// only creates a new set of problems.  Have an idea to work around
	// this (a canvas object type for MCUs that can afford the RAM and
	// displays supporting setAddrWindow() and pushColors()), but haven't
	// implemented this yet.

	for (yy = 0; yy < h; yy++) {
		for (xx = 0; xx < w; xx++) {
			if (!(bit++ & 7)) {
				bits = bitmap[bo++];
			}
			if (bits & 0x80) {
				if (size == 1) {
					drawPixel(x + xo + xx, y + yo + yy, color);
				} else {
					fillRect(x + (xo16 + xx) * size, y + (yo16 + yy) * size,
							size, size, color);
				}
			}
			bits <<= 1;
		}
	}
} // End classic vs custom font

// Draw a character in classic font.
void oled1309::drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
		uint16_t bg, uint8_t size) {

	if ((x >= WIDTH) || // Clip right
			(y >= HEIGHT) || // Clip bottom
			((x + 6 * size - 1) < 0) || // Clip left
			((y + 8 * size - 1) < 0))   // Clip top
		return;

	//   if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior
	int chval = int(c);

	for (int8_t i = 0; i < 6; i++) {
		uint8_t line;
		if (i < 5)
			line = font[((chval * 5) + i)];
		else
			line = 0x0;
		for (int8_t j = 0; j < 8; j++, line >>= 1) {
			if (line & 0x1) {
				if (size == 1)
					drawPixel(x + i, y + j, color);
				else
					fillRect(x + (i * size), y + (j * size), size, size, color);
			} else if (bg != color) {
				if (size == 1)
					drawPixel(x + i, y + j, bg);
				else
					fillRect(x + i * size, y + j * size, size, size, bg);
			}
		}
	}
}


void oled1309::writeString(int x, int y, int size, const char *string) {

	unsigned char ch, c;

	while (*string != 0) {
		ch = *string;
		string++;
		c = ch;
		c -= gfxFont.first;
		GFXglyph glyph = gfxFont.glyph[c];
		int8_t xa = glyph.xAdvance - 4;
		drawCharCustom(x, y, ch,
		WHITE, BLACK, size);
		x += xa * size;
	}
}

