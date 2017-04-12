/*
 * DispDraw.h
 *
 *  Created on: 5 Mar 2017
 *      Author: root
 */

#ifndef DISPDRAW_H_
#define DISPDRAW_H_

#include "tft1963.h"
#include "Adafruit_GFX.h"


class DispDraw: public RpiHardware {

public:
	int width, height;
	unsigned short fBuffer[((800 + 1) * (480 + 1))];

	DispDraw();
	DispDraw(int width, int height);

	virtual ~DispDraw();

    void bufftoDisplay(void);
	void drawBox (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,
		 unsigned int color);
	void drawHorzLine (int x1, int x2, int y_pos,
		    unsigned int color);
	void drawFastVLinelcl (short y1, short y2, short x_pos,
		    unsigned short color);
	void drawPixel(int16_t x, int16_t y, uint16_t color);
	void drawVertLine(unsigned int y1, unsigned int y2,
			unsigned int x_pos, unsigned int color);

private:

};


#endif /* DISPDRAW_H_ */
