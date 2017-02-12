/*
 * oled1309.h
 *
 *  Created on: 11 Feb 2017
 *      Author: root
 */

#ifndef OLED1309_H_
#define OLED1309_H_


#include "gfxfont.h"
#include "glcdfont.h"

#define RESETLINE 29
#define DCLINE 28
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

class oled1309 {
public:
	int x;
	GFXfont gfxFont;

	oled1309(int x);
	virtual ~oled1309();
	void print(void);
	void setFont(const GFXfont font);
	void drawPixel(int16_t x, int16_t y, uint16_t color);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);



private:
	void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
			uint16_t bg, uint8_t size);
	int width();
	int height();


};

#endif /* OLED1309_H_ */
