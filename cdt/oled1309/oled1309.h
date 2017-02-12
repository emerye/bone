/*
 * oled1309.h
 *
 *  Created on: 11 Feb 2017
 *      Author: root
 */

#ifndef OLED1309_H_
#define OLED1309_H_

#include "SSD1309.h"
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
	void print2(void);

	virtual ~oled1309();


    virtual void setFont(GFXfont font);
	virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
	virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	virtual void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	virtual void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
				uint16_t bg, uint8_t size);
	virtual void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
	virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	virtual void drawFastVLine(int16_t x, int16_t __y, int16_t __h, uint16_t color);
	virtual void invertDisplay(uint8_t i);
	virtual int SendSPIBlock(enum cmd cmdType, unsigned char *spiData, int numBytes);
	virtual int SendByte(enum cmd cmdType, int data);
	virtual void initDisplay();
	virtual void Display_Picture(unsigned char *p);
	virtual void init_Hardware(void);
	virtual void setContrast(unsigned char level);
	virtual void drawCharCustom(int16_t x, int16_t y, unsigned char c, uint16_t color,
			uint16_t bg, uint8_t size);
	virtual void writeString(int x, int y, int size, const char *string);
	virtual void print(void);





private:



	void Set_Page_Address(unsigned char a, unsigned char b);
	void Set_Column_Address(unsigned char a, unsigned char b);
	void Set_Addressing_Mode(unsigned char d);
	void Set_Start_Column(unsigned char d);

	void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color);

	void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
			uint16_t color);
	void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
			int16_t delta, uint16_t color);
	void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	int width();
	int height();


};

#endif /* OLED1309_H_ */
