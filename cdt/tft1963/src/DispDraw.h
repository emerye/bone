/*
 * DispDraw.h
 *
 *  Created on: 5 Mar 2017
 *      Author: root
 */

#ifndef DISPDRAW_H_
#define DISPDRAW_H_

#include "tft1963.h"


class DispDraw: public RpiHardware {
public:
	DispDraw();
	virtual ~DispDraw();
	void drawDot (unsigned int x, unsigned int y, unsigned int color);
	void drawBox (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,
		 unsigned int color);
	void drawHorzLine (int x1, int x2, int y_pos,
		    unsigned int color);
	void drawVertLine (unsigned int y1, unsigned int y2, unsigned int x_pos,
		    unsigned int color);


};


#endif /* DISPDRAW_H_ */
