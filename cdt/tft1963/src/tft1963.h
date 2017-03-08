/*
 * tft1963.h
 *
 *  Created on: 5 Mar 2017
 *      Author: root
 */

#ifndef TFT1963_H_
#define TFT1963_H_

#include "Adafruit_GFX.h"

#define WRITEGPIO 21
#define DATACMDGPIO 28
#define RESETGPIO 25

//RGB 565
#define BLACK  0x0000
#define RED    0xF800
#define GREEN  0x07E0
#define BLUE   0x001F
#define WHITE  0xFFFE
#define PURPLE 0xF11F
#define YELLOW 0xFFE0
#define CYAN   0x07FF
#define MAGENTA 0xF81F

//480 x 272
#define XMAXPIXEL  479
#define YMAXPIXEL  271
#define SSD1963		 1

enum cmdType
{
   WRITECMD,
   WRITEDATA
};


class RpiHardware : public Adafruit_GFX
{

public:
	RpiHardware();
	~RpiHardware();

	int spiDescriptor;

	void initRpiHardware(void);
	void Write_Command (unsigned int data);
	void Write_Data (unsigned int data);
	void Init_ssd1963(void);
	void TFT_FillDisp(unsigned int color);
	void setAddress (unsigned int px1, unsigned int py1, unsigned int px2,
			 unsigned int py2);
	void SendCommand(cmdType);

private:
	unsigned int  HDP;
		unsigned int  HT;
		unsigned int  HPS;
		unsigned int  LPS;
		unsigned char HPW;

		unsigned int  VDP;
		unsigned int  VT;
		unsigned int  VPS;
		unsigned int  FPS;
		unsigned char VPW;

};


#endif /* TFT1963_H_ */
