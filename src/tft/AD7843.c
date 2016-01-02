//******************************************************************************
/* 
* TFT Display TFT_320_QVT SSD1289 Controller 3-12-2012
* 
*/

#include <msp430g2553.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tftFonts.h"
#include "tft.h"

//Touch pad corrections
#define XSTART 230
#define XEND 3830
#define YSTART 160
#define YEND 3860

void WriteCharTo7843(unsigned char num);
void Write_Command(unsigned int WCommand);
void Write_Data(unsigned int Wdata);
void Write_Command_Data(unsigned int Wcommand, unsigned int Wdata);
void TFT_Set_Address(unsigned int PX1, unsigned int PY1, unsigned int PX2, unsigned int PY2);
void ZeroMemory(char *memory, unsigned int count);

BYTE AD7843_Transfer(BYTE Cmd);
unsigned int ReadCharFrom7843(void);
void GetPadXY(WORD *x, WORD *y, BYTE mode);
WORD AD7843_X(BYTE mode);
WORD AD7843_Y(BYTE mode);

/***********************************************************************/

int TouchPad(void)
{
	volatile WORD xPos, yPos;
	WORD xPad, yPad;
	char mode = 1; 

	GetPadXY(&xPad, &yPad, mode);
	TFT_Dot(xPad, yPad, 0xFFFF);

	/*
	memset(buffer, 0, 12); 
	ltoa(xPad, buffer); 
	TFT_Box(10,170,100,190,WHITE);
	TFT_Text(buffer,10,170,16,WHITE, BLUE);

	memset(buffer, 0, 12); 
	ltoa(yPad, buffer); 
	TFT_Box(10,190,100,210,WHITE); 
	TFT_Text(buffer,10,190,16,WHITE, BLUE);
	*/

	return(0); 
}

/**
* Return x,y location. 
* 
* @param x xPos
* @param y yPos
* @param mode 0 for raw. Non-zero for calibrated. 
* 
*/
void GetPadXY(WORD *x, WORD *y, BYTE mode)
{
	*x = AD7843_X(mode);
	*y = AD7843_Y(mode);
}

WORD AD7843_X(BYTE mode)
{
	WORD xrawCount; 
	WORD xCorrected; 

	P2OUT &= ~D_CS; 		//Set CS Low
	WriteCharTo7843(0xD0);
	xrawCount = ReadCharFrom7843(); 
	P2OUT |= D_CS;			//Set CS High

	if (mode) 
	{
		//Correction
		if ( (xrawCount < XSTART) || (xrawCount > XEND) ) 
			return 0; 

		xCorrected = (xrawCount - XSTART) / ((XEND - XSTART) / 240.0);
	}
	else
	{
		xCorrected = xrawCount;
	}
	return(xCorrected); 
}

WORD AD7843_Y(BYTE mode)
{
	volatile WORD yPos;
	volatile WORD yrawCount;
	volatile WORD yPosReturned; 

	P2OUT &= ~D_CS; 		//Set CS Low
	WriteCharTo7843(0x90);
	yrawCount = ReadCharFrom7843();
	P2OUT |= D_CS;			//Set CS High
	if (mode) 
	{
		if ( ((yrawCount < YSTART) || (yrawCount > YEND)) == 1 )
			return 0;

		yPos = (  ((YEND - YSTART) - (yrawCount - YSTART)) / ( (YEND - YSTART) / 320.0) ); 
		yPosReturned = yPos; 
	}
	else
	{
		yPosReturned = yrawCount; 
	}
	return(yPosReturned); 
}

BYTE AD7843_Transfer(BYTE Cmd)
{
	P2OUT &= ~D_CS; 		//Set CS Low
	WriteCharTo7843(Cmd); 
	P2OUT |= D_CS;

	//Y
	P2OUT &= ~D_CS; 		//Set CS Low
	WriteCharTo7843(0xD0); 
	P2OUT |= D_CS;

	return(0); 
}

//**********************************************************
/** \fn WriteCharTo7843
*  Write data to control register of AD7843
*	\param BYTE num  Command to write
*/

void WriteCharTo7843(BYTE num)        
{
	int count=0;
	BYTE serData; 

	for(count=7; count >=  0; count--)
	{
		serData = (num >> count) & 0x01; 

		if (serData == 1) 
			P2OUT |= D_IN;
		else
			P2OUT &= ~D_IN;

		_NOP();  _NOP();
		P2OUT |= D_CLK; _NOP() ;_NOP();_NOP();
		P2OUT &= ~D_CLK; _NOP();_NOP();_NOP();     //Send
	}
	P2OUT &= ~D_IN;
}

//**********************************************************

unsigned int ReadCharFrom7843(void)         
{
	int count=0;
	volatile unsigned int Num = 0;
	WORD bitIn = 0;

	// P3OUT &= ~D_CS; 		//Set CS Low
	for(count=11; count >= 0; count--)
	{
		_NOP();_NOP();_NOP();
		P2OUT |= D_CLK; _NOP(); _NOP(); _NOP();  

		P2OUT &= ~D_CLK; _NOP(); _NOP(); _NOP();
		if( (P2IN & D_OUT) == D_OUT ) 
		{
			bitIn = (1 << count); 
			Num += bitIn; 
		}
	}

	//Transfer three more null bytes
	for (count = 0; count < 3; count++)
	{
		P2OUT |= D_CLK; _NOP(); _NOP(); _NOP();          
		P2OUT &= ~D_CLK; _NOP(); _NOP(); _NOP();
	}
	P2OUT |= D_CS;	//CS to high
	return(Num);
}

