/*
 * tft1963.h
 *
 *  Created on: 5 Mar 2017
 *      Author: root
 */

#ifndef TFT1963_H_
#define TFT1963_H_


#define WRITEGPIO 21
#define DATACMDGPIO 28
#define RESETGPIO 25

//RGB 565
#define BLACK  0x0000
#define RED    0xF800
#define GREEN  0x07E0
#define BLUE   0x001F
#define WHITE  0xFFFF
#define PURPLE 0xF11F
#define YELLOW 0xFFE0
#define CYAN   0x07FF
#define MAGENTA 0xF81F

//480 x 272
#define XMAXPIXEL  479
#define YMAXPIXEL  271
#define SSD1963		 1

unsigned int  HDP=479;
unsigned int  HT=531;
unsigned int  HPS=43;
unsigned int  LPS=8;
unsigned char HPW=10;

unsigned int  VDP=271;
unsigned int  VT=288;
unsigned int  VPS=12;
unsigned int  FPS=4;
unsigned char VPW=10;

typedef enum
{
   WRITECMD,
   WRITEDATA
} cmdType;


class RpiHardware{
public:
	RpiHardware();
	~RpiHardware();

	int spiDescriptor;

	void initRpiHardware(void);
	void Write_Command (unsigned int data);
	void Write_Data (unsigned int data);
	void Init_ssd1963(void);
	void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
	void TFT_FillDisp(unsigned int color);
	void TFT_Set_Address (unsigned int px1, unsigned int py1, unsigned int px2,
			 unsigned int py2);
	void SendCommand(cmdType cmd);


private:

};



#endif /* TFT1963_H_ */
