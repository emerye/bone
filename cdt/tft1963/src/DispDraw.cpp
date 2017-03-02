/*
 * DispDraw.cpp
 *
 *  Created on: 5 Mar 2017
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "DispDraw.h"

//namespace oledTest {

DispDraw::DispDraw() {

}

DispDraw::~DispDraw() {
	// TODO Auto-generated destructor stub
}


void DispDraw::drawPixel(int16_t x, int16_t y, uint16_t color) {
	drawDot(x,y,color);

}

void DispDraw::drawDot(unsigned int x, unsigned int y, unsigned int color) {

	setAddress(x, y, x , y);
	Write_Data(color);
    SendCommand(WRITEDATA);
}


void DispDraw::drawVertLine(unsigned int y1, unsigned int y2,
		unsigned int x_pos, unsigned int color) {

	unsigned int k;

	setAddress(x_pos, y1, x_pos, y2);
	Write_Data(color);
	for (k = y1; k <= y2; k++) {
		SendCommand(WRITEDATA);
	}
}

void DispDraw::drawHorzLine(int x1, int x2, int y_pos, unsigned int color) {
	int k;

	setAddress(x1, y_pos, x2, y_pos);
	Write_Data(color);

	for (k = x1; k <= x2; k++) {
		drawDot(k, y_pos, color);
	}
}

