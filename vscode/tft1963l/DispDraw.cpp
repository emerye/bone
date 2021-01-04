/*
 * DispDraw.cpp
 *
 *  Created on: 5 Mar 2017
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "DispDraw.h"
#include "tft1963.h"


DispDraw::DispDraw(int width, int height): RpiHardware(width, height) {
	this->width = width;
	this->height = height;
}


DispDraw::~DispDraw() {

}

/**
 * Write one dot to frame buffer
 */
void DispDraw::drawPixel(int16_t x, int16_t y, uint16_t color) {
	if (y > height + 1) {
		printf("Y coordinate is out of range in drawDot\n");
		return;
	}
	fBuffer[y * width + x] = color;
}


/**
 * Update entire display from buffer
 */
void DispDraw::bufftoDisplay() {

	int i, j;
	unsigned short curValue = 0x0002;
	unsigned short nextValue = 0xFFFF;

	setAddress(0, 0, width, height);
	for (i = 0; i < height + 1; i++) {
		for (j = 0; j < width + 1; j++) {
			nextValue = fBuffer[i * width + j];
			if (curValue == nextValue) {
				SendCommand(WRITEDATA);
				curValue = fBuffer[i * width + j];
			} else {
				Write_Command(SSD1963_WRITE_MEMORY_CONTINUE);
				Write_Data(fBuffer[i * width + j]);
				curValue = fBuffer[i * width + j];
			}
		}
	}
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
		drawPixel(k, y_pos, color);
	}
}


/**
 * Read rgb565 picture.
 */
int DispDraw::rgb565(const char *file, int width, int height, unsigned short *buffer) {
	FILE *fp;
	short int data;
	int i, j;
	int count = 0;
	short cData;

	if ((fp = fopen(file, "r")) != NULL) {
		while (!feof(fp)) {
			fread(&data, 2, 1, fp);
			cData = (((data >> 8) & 0xFF) | ((data & 0xFF) << 8));
			*buffer = cData;
			buffer++;
		}
	} else {
		printf("File %s could not be opened\n", file);
		return 1;
	}
	fclose(fp);
  return 0;
}


