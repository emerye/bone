#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "spi.h"
#include "gpio.h"

#define RESETLINE 51
#define DCLINE 50


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Global Variables
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#define XLevelL         0x00
#define XLevelH         0x10
#define XLevel          ((XLevelH&0x0F)*16+XLevelL)
#define Max_Column      128
#define Max_Row         64
#define Brightness      0x8F



enum cmd { COMMAND, DATA };
int spiFD;

unsigned char pic[]=
{/*--ER-OLED015-2.bmp  --*/
/*--  128x64  --*/
0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x21,0xFD,0x21,0xE1,0x01,
0xF9,0x49,0x49,0x49,0xF9,0x01,0x01,0x01,0x01,0xFD,0x45,0x45,0x45,0x45,0x45,0x45,
0xFD,0x01,0x01,0x09,0x89,0xC9,0xA9,0x99,0xED,0x89,0x89,0x89,0x89,0x09,0x01,0x11,
0x11,0x11,0x11,0xF1,0x55,0x59,0x51,0xD1,0x19,0x11,0x01,0x29,0xA9,0xF9,0xA5,0x25,
0x01,0x49,0x91,0x01,0xFD,0x01,0x01,0x11,0x11,0xFD,0x91,0x01,0x91,0x91,0xFD,0x91,
0x91,0x11,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x07,0x00,0x1F,0x10,
0x17,0x12,0x12,0x12,0x17,0x18,0x00,0x00,0x00,0x1F,0x08,0x08,0x08,0x08,0x08,0x08,
0x1F,0x00,0x00,0x08,0x08,0x04,0x02,0x10,0x1F,0x00,0x01,0x02,0x04,0x08,0x00,0x10,
0x10,0x08,0x06,0x01,0x10,0x10,0x10,0x0F,0x00,0x00,0x00,0x06,0x01,0x1F,0x00,0x01,
0x02,0x02,0x02,0x02,0x1F,0x01,0x00,0x11,0x11,0x1F,0x00,0x10,0x11,0x0A,0x04,0x0A,
0x11,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x80,0x70,0x2C,0x22,0x2C,0x70,0x80,
0x4C,0x92,0x92,0x64,0x00,0x02,0x02,0xFE,0x02,0x02,0x00,0xFE,0x12,0x12,0x32,0xCC,
0x00,0x00,0xFE,0x00,0x00,0x4C,0x92,0x92,0x64,0x00,0x00,0xFE,0x00,0x00,0xFE,0x0C,
0x10,0x60,0xFE,0x00,0x00,0x7C,0x82,0x82,0x92,0x74,0x00,0x00,0x00,0x00,0x02,0x02,
0xFE,0x02,0x02,0x00,0xFE,0x92,0x92,0x92,0x92,0x00,0x7C,0x82,0x82,0x82,0x44,0x00,
0x00,0xFE,0x10,0x10,0x10,0xFE,0x00,0x00,0x80,0x00,0x00,0x7C,0x82,0x82,0x82,0x44,
0x00,0x00,0x7C,0x82,0x82,0x82,0x7C,0x00,0x00,0x80,0x00,0x00,0x80,0x00,0x00,0xFE,
0x80,0x80,0x80,0x02,0x02,0xFE,0x02,0x02,0x00,0xFE,0x82,0x82,0x82,0x7C,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xC0,0x20,0x00,0x10,0x08,0xC8,0xC8,0x48,0x48,0x48,0x08,0xC8,
0xC8,0x48,0x48,0xC8,0x88,0x08,0x08,0x08,0x08,0x08,0x08,0x88,0xC8,0x48,0xC8,0x88,
0x08,0x08,0xC8,0xC8,0x08,0x08,0x08,0x08,0x08,0xC8,0xC8,0x48,0x48,0x48,0x08,0xC8,
0xC8,0x48,0x48,0xC8,0x88,0x08,0x88,0xC8,0x48,0xC8,0x88,0x08,0x08,0x88,0xC8,0xC8,
0x08,0x08,0x08,0xC8,0xC8,0x48,0x48,0x08,0x08,0x08,0x08,0x08,0x88,0xC8,0x48,0xC8,
0x88,0x08,0x10,0x20,0x20,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x0F,0x30,0x40,0x00,0x80,0x00,0x3F,0x3F,0x22,0x22,0x22,0x00,0x3F,
0x3F,0x04,0x0C,0x1F,0x33,0x20,0x08,0x08,0x08,0x00,0x0F,0x1F,0x30,0x20,0x30,0x1F,
0x0F,0x00,0x3F,0x3F,0x20,0x20,0x20,0x20,0x00,0x3F,0x3F,0x22,0x22,0x22,0x00,0x3F,
0x3F,0x20,0x20,0x3F,0x1F,0x00,0x1F,0x3F,0x20,0x3F,0x1F,0x00,0x03,0x01,0x3F,0x3F,
0x00,0x00,0x17,0x33,0x22,0x3E,0x1C,0x00,0x08,0x08,0x08,0x00,0x30,0x38,0x2C,0x27,
0x23,0x00,0x80,0x40,0x40,0x30,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x7F,0x00,
0x00,0x00,0x40,0x00,0x2C,0x47,0x45,0x39,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x40,0x20,0xF0,0x00,0x00,0x20,0x10,0x90,0x60,0x00,0x60,0x90,0x90,
0x60,0x00,0x00,0x30,0x40,0xC0,0x30,0x00,0xE1,0x91,0x91,0x21,0x01,0x01,0xC1,0x21,
0xF1,0x01,0x01,0x01,0x81,0x41,0x41,0x41,0xF1,0x01,0x81,0x41,0x41,0x41,0x81,0x41,
0xF1,0x41,0x01,0x81,0x41,0x41,0x41,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x01,0x01,0x01,0x01,0xF1,0x91,0x91,0xF1,0x01,0x01,0xC1,0x31,0x01,0x61,0x91,0x91,
0x21,0x01,0x01,0x01,0x01,0x01,0xF1,0x01,0xC1,0x41,0x41,0x81,0x41,0xE1,0x41,0x01,
0x81,0x41,0x40,0x80,0x00,0xC0,0x40,0x40,0x40,0xE0,0x50,0x00,0x40,0x40,0x40,0xC0,
0x00,0x80,0x40,0x40,0x80,0x00,0x80,0x40,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0xFF,
0xFF,0x00,0x00,0x80,0x80,0x07,0x00,0x80,0x06,0x05,0x84,0x84,0x00,0x83,0x84,0x04,
0x03,0x80,0x00,0x06,0x81,0x81,0x06,0x80,0x83,0x04,0x04,0x83,0x00,0x03,0x82,0x82,
0x07,0x02,0x00,0x00,0xE3,0xE4,0x84,0x84,0x87,0x00,0x03,0x84,0x84,0x04,0x03,0x80,
0x87,0x84,0x80,0x04,0x05,0x05,0x82,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,
0xE0,0xE0,0x00,0xA0,0xA7,0x00,0x00,0x80,0x80,0x86,0x81,0x00,0x00,0x82,0x84,0x84,
0x83,0x80,0x00,0x00,0xE0,0xE0,0x07,0x00,0x87,0x80,0x80,0x07,0x80,0x87,0x04,0x00,
0x03,0x85,0x85,0x05,0x00,0x07,0x00,0x80,0x80,0x87,0x00,0x00,0x06,0x85,0x85,0x87,
0x80,0x03,0x04,0x84,0x82,0x80,0x83,0x85,0x85,0x85,0x80,0x80,0x00,0x00,0x00,0xFF,
0xFF,0x80,0x80,0x83,0x9F,0x9C,0x8F,0x83,0x8F,0x9C,0x9F,0x83,0x80,0x83,0x9F,0x9C,
0x8F,0x83,0x8F,0x9C,0x9F,0x83,0x80,0x83,0x9F,0x9C,0x8F,0x83,0x8F,0x9C,0x9F,0x83,
0x80,0x98,0x98,0x80,0x9F,0x9F,0x90,0x90,0x9F,0x8F,0x80,0x8F,0x9F,0x90,0x90,0x9F,
0x9F,0xC0,0xC7,0xFF,0xB8,0x9F,0x87,0x80,0x84,0x84,0x84,0x80,0x8F,0x9F,0x90,0x90,
0x9F,0x9F,0x80,0x9F,0x9F,0x80,0x8B,0x9B,0x96,0x96,0x9D,0x8D,0x80,0xFF,0xFF,0x90,
0x90,0x9F,0x8F,0x80,0x9F,0x9F,0x80,0x8D,0x9E,0x92,0x9F,0x9F,0xC0,0xC7,0xFF,0xB8,
0x9F,0x87,0x80,0x98,0x98,0x80,0x8F,0x9F,0x90,0x99,0x89,0x80,0x8F,0x9F,0x90,0x90,
0x9F,0x8F,0x80,0x9F,0x9F,0x80,0x80,0x9F,0x9F,0x80,0x80,0x9F,0x9F,0x80,0x80,0xFF,
};


int SendByte(enum cmd cmdType, int data)
{
    int error;

    if (cmdType == COMMAND) {
	gpio_set_value(DCLINE, 0);
        puts("Command"); 
    } else {
	gpio_set_value(DCLINE, 1);
        puts("Data Command"); 
    }
    error = SPISendByte(spiFD, data);
    if (error == -1) {
	printf("Error %d\n", error);
    }
    return error;
}


void initDisplay()
{
    gpio_set_value(RESETLINE, 1);
    gpio_set_value(RESETLINE, 0);
    usleep(1000);
    gpio_set_value(RESETLINE, 1);

    sleep(1);

    SendByte(COMMAND, 0xFD);	// Set Command Lock
    SendByte(COMMAND, 0x12);	//   Default => 0x12
    //     0x12 => Driver IC interface is unlocked from entering command.
    //     0x16 => All Commands are locked except 0xFD.

    SendByte(COMMAND, 0xae);	//--turn off oled panel

    SendByte(COMMAND, 0xD5);	// Set Display Clock Divide Ratio / Oscillator Frequency
    SendByte(COMMAND, 0xa0);	//   Default => 0x70
    //     D[3:0] => Display Clock Divider
    //     D[7:4] => Oscillator Frequency

    SendByte(COMMAND, 0xa8);	//--set multiplex ratio(1 to 64)
    SendByte(COMMAND, 0x3f);	//--1/64 duty

    SendByte(COMMAND, 0xd3);	//Set Display Offset
    SendByte(COMMAND, 0x00);

    SendByte(COMMAND, 0x40);	// Set Display Start Line

    SendByte(COMMAND, 0x20);	// Set Memory Addressing Mode
    SendByte(COMMAND, 0x02);	//   Default => 0x02
    //     0x00 => Horizontal Addressing Mode
    //     0x01 => Vertical Addressing Mode
    //     0x02 => Page Addressing Mode


    SendByte(COMMAND, 0xa1);	//--set segment re-map 128 to 0

    SendByte(COMMAND, 0xC8);	//--Set COM Output Scan Direction 64 to 1


    SendByte(COMMAND, 0xda);	//--Set COM Pins Hardware Configuration
    SendByte(COMMAND, 0x12);	//     Disable COM Left/Right Re-Map   Alternative COM Pin Configuration


    SendByte(COMMAND, 0x81);	//--set contrast control register
    SendByte(COMMAND, 0xFF);


    SendByte(COMMAND, 0xD9);	// Set Pre-Charge Period
    SendByte(COMMAND, 0x25);	//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
    //     D[3:0] => Phase 1 Period in 1~15 Display Clocks
    //     D[7:4] => Phase 2 Period in 1~15 Display Clocks

    SendByte(COMMAND, 0xDB);	// Set VCOMH Deselect Level
    SendByte(COMMAND, 0x34);	//   Default => 0x34 (0.78*VCC)


    SendByte(COMMAND, 0xad);	//--Set Master Configuration
    SendByte(COMMAND, 0x8e);	//--

    SendByte(COMMAND, 0xd8);	//--Set Area Color Mode On/Off & Low Power Display Mode
    SendByte(COMMAND, 0x05);	//


    SendByte(COMMAND, 0xa6);	//--set normal display

    SendByte(COMMAND, 0xa4);	//Disable Entire Display On     

    SendByte(COMMAND, 0xaf);	//--turn on oled panel
}



//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Instruction Setting
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Start_Column(unsigned char d)
{
        SendByte(COMMAND, 0x00+d%16);               // Set Lower Column Start Address for Page Addressing Mode
                                                //   Default => 0x00
        SendByte(COMMAND,0x10+d/16);               // Set Higher Column Start Address for Page Addressing Mode
                                                //   Default => 0x10
}


void Set_Addressing_Mode(unsigned char d)
{
        SendByte(COMMAND, 0x20);                    // Set Memory Addressing Mode
        SendByte(COMMAND, d);                       //   Default => 0x02
                                                //     0x00 => Horizontal Addressing Mode
                                                //     0x01 => Vertical Addressing Mode
                                                //     0x02 => Page Addressing Mode
}


void Set_Column_Address(unsigned char a, unsigned char b)
{
        SendByte(COMMAND, 0x21);                    // Set Column Address
        SendByte(COMMAND, a);                       //   Default => 0x00 (Column Start Address)
        SendByte(COMMAND, b);                       //   Default => 0x7F (Column End Address)
}


void Set_Page_Address(unsigned char a, unsigned char b)
{
        SendByte(COMMAND,0x22);                    // Set Page Address
        SendByte(COMMAND, a);                       //   Default => 0x00 (Page Start Address)
        SendByte(COMMAND, b);                       //   Default => 0x07 (Page End Address)
}



void Display_Picture(unsigned char *p) 
{unsigned char *picture;
    unsigned char i,j,num=0;
                picture=p;
        for(i=0;i<0x08;i++)
        {
 //       Set_Start_Page(i);
        SendByte(COMMAND, 0xB0 | i); 
        Set_Start_Column(XLevel);
        for(j=0;j<0x80;j++)
                {
                    SendByte(DATA, *picture);
                        picture++;
                }
        }
}


void init_Hardware(void)
{
    gpio_export(RESETLINE);
    gpio_export(DCLINE);
    gpio_set_dir(DCLINE, 1);
    gpio_set_dir(RESETLINE, 1);
}


int main(int argc, char **argv)
{
    init_Hardware();
    Init_SPI();
    initDisplay();
    
    sleep(1);
    Display_Picture(pic); 
    return 0;
}
