#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "tftFonts.h"
#include "AD7843.h"
#include "tft.h"


void CreateButtons ();
char sBuffer[39] = { 0 };
char tBuffer[30] = { 0 };

unsigned int xPos;
void *gpio_addr = NULL;
int mmapFD;
char *phrase = "0123";

void ssd1963Init()
{
  int i;
  int x = 450; 
  int y = 200; 

  Init_ssd1963(); 

  Address_set(0, 0, 479, 271); 
  Write_Data(BLACK); 
  for(i=0; i< 272*480; i++)
   SendCommand(WRITEDATA); 

  Address_set(0,0,x,y);   
  Write_Data(RED | GREEN );  
  for(i=0; i< x * y; i++)
    SendCommand(WRITEDATA);  
} 

int
main ()
{
  int retval;
  unsigned long int iCount = 40;
  unsigned int yLine = 9;
  int xPos = 0;

  retval = MapGPIO ();
  if (retval < 0)
    {
      printf("Error attempting to map GPIO1.\n");
    }

 ssd1963Init(); 
 return 0; 

//  SendDisplayReset();

//  TFT_Init();

  TFT_Fill (YELLOW);
  TFT_Set_Address (0, 0, 239, 319);
  usleep (1000 * 100);

  TFT_Fill (GREEN);
  usleep (1000 * 100);

  TFT_Set_Address (0, 0, 239, 319);
  TFT_Fill (RED);
  usleep (1000 * 100);

  Write_Data (BLACK);
  TFT_Set_Address (0, 0, 239, 319);

  Write_Data (WHITE);
  iCount = 32;
  for (yLine = 0; yLine < 320; yLine += 16)
    {
      for (xPos = 0; xPos < 240; xPos += 16)
	{
	  TFT_Char ((char) iCount, xPos, yLine, 16, BLACK, WHITE);
	  iCount++;
	  if (iCount > 127)
	    iCount = 32;
	}
    }
  TFT_Fill (BLACK);
  TFT_Text ("This is a line.", 0, 250, 16, BLUE, BLACK);


  TFT_Rectangle (0, 100, 100 , 300, RED);

  TFT_Text32 (phrase, 0, 50, WHITE, BLACK);

  close (mmapFD);
  return 0;
}


void
CreateButtons ()
{
  WORD x1, y1, x2, y2;
  x1 = 0;
  y1 = 0;
  x2 = 78;
  y2 = 60;

  CreateButton (x1, y1, x2, y2, RED, GREEN, "Hour");
  CreateButton (x1 + 79, y1, x2 * 2, y2, RED, WHITE, "Min");
  CreateButton (x1 + 158, y1, x2 * 3, y2, RED, CYAN, "Test");
}


/** 
* Create a button. Define top left and bottom right. 
* Border and Fill Color
* @param x1 
*/
void
CreateButton (WORD x1, WORD y1, WORD x2, WORD y2, WORD border, WORD backcolor,
	      char *btnText)
{
  BYTE count = 0;
  volatile WORD textX = 0;
  volatile WORD textY = 0;

  //Don't let x2 > 237
  if (x2 > 237)
    x2 = 237;
  TFT_Rectangle (x1, y1, x2, y2, border);
  count++;
  TFT_Rectangle (x1 + count, y1 + count, x2 + count, y2 - count, border);
  count++;
  TFT_Rectangle (x1 + count, y1 + count, x2 + count, y2 - count, border);
  TFT_Box (x1 + count, y1 + count, x2 - count, y2 - count, backcolor);
  count++;
  textX = x1 + ((x2 - x1) / 2) - (strlen (btnText) * 8);
  textY = y1 - 8 + (y2 - y1) / 2;
  TFT_Text (btnText, textX + count, textY, 16, BLACK, backcolor);
}


void
WriteCommandData (unsigned int Wcommand, unsigned int Wdata)
{
  SendWord(Wcommand);
  SendCommand(WRITECMD);
  SendWord(Wdata);
  SendCommand(WRITEDATA);
}


void
TFT_Init (void)
{
  WriteCommandData(0x0000, 0x0001);
  WriteCommandData(0x0003, 0xA8A4);
  WriteCommandData(0x000C, 0x0000);
  WriteCommandData(0x000D, 0x800C);
  WriteCommandData(0x000E, 0x2B00);
  WriteCommandData(0x001E, 0x00B7);
  WriteCommandData(0x0001, 0x2B3F);
  WriteCommandData(0x0002, 0x0600);
  WriteCommandData(0x0010, 0x0000);
  //Vertical
  WriteCommandData(0x0011, 0x6070);
  //Horz
//  WriteCommandData (0x0011, 0x6078);
  WriteCommandData(0x0005, 0x0000);
  WriteCommandData(0x0006, 0x0000);
  WriteCommandData(0x0016, 0xEF1C);
  WriteCommandData(0x0017, 0x0003);
  WriteCommandData(0x0007, 0x0233);
  WriteCommandData(0x000B, 0x0000);
  WriteCommandData(0x000F, 0x0000);
  WriteCommandData(0x0041, 0x0000);
  WriteCommandData(0x0042, 0x0000);
  WriteCommandData(0x0048, 0x0000);
  WriteCommandData(0x0049, 0x013F);
  WriteCommandData(0x004A, 0x0000);
  WriteCommandData(0x004B, 0x0000);
  WriteCommandData(0x0044, 0xEF95);
  WriteCommandData(0x0045, 0x0000);
  WriteCommandData(0x0046, 0x013F);
  WriteCommandData(0x0030, 0x0707);
  WriteCommandData(0x0031, 0x0204);
  WriteCommandData(0x0032, 0x0204);
  WriteCommandData(0x0033, 0x0502);
  WriteCommandData(0x0034, 0x0507);
  WriteCommandData(0x0035, 0x0204);
  WriteCommandData(0x0036, 0x0204);
  WriteCommandData(0x0037, 0x0502);
  WriteCommandData(0x003A, 0x0302);
  WriteCommandData(0x003B, 0x0302);
  WriteCommandData(0x0023, 0x0000);
  WriteCommandData(0x0024, 0x0000);
  WriteCommandData(0x0025, 0x8000);
  WriteCommandData(0x004f, 0x0000);
  WriteCommandData(0x004e, 0x0000);
  Write_Command (0x0022);
}


void
Write_Command (unsigned int Wcommand)
{
  SendWord (Wcommand);
  SendCommand (WRITECMD);
}


void
Write_Data (unsigned int Wdata)
{
  SendWord(Wdata);
  SendCommand(WRITEDATA);
}


void
DelayMsec (unsigned int delay)
{
  usleep (1000 * delay);
}


void
TFT_Set_Address (unsigned int PX1, unsigned int PY1, unsigned int PX2,
		 unsigned int PY2)
{
  WriteCommandData (0x0044, (PX2 << 8) + PX1);	//Horizonal RAM Address Position
  WriteCommandData (0x0045, PY1);	// Vertical RAM address start
  WriteCommandData (0x0046, PY2);	// Vertical RAM address end
  WriteCommandData (0x004E, PX1);	// Set GDDRAM X address
  WriteCommandData (0x004F, PY1);	// Set GDDRAM Y address
  Write_Command (0x0022);
}


void
TFT_Fill (unsigned int color)
{
  unsigned int i, j;

  TFT_Set_Address (0, 0, 239, 319);
  Write_Data (color);
  for (i = 0; i <= 319; i++)
    {
      for (j = 0; j <= 239; j++)
	SendCommand (WRITEDATA);
    }
}


unsigned int
Set_color (unsigned char r, unsigned char g, unsigned char b)
{
  unsigned int temp = 0;

  temp = (r & 0xF8) || (g >> 5) << 8;
  g = (g & 0x1C);
  temp |= (g << 3) || (b >> 3);
  return (temp);
}


void
TFT_Text (char *S, WORD x, WORD y, BYTE DimFont, WORD Fcolor, WORD Bcolor)
{

  BYTE length, k;
  WORD buffer[24] = { 0 };
  BYTE charcount = 0;
  length = strlen (S);

  while (*S != 0)
    {
      buffer[charcount] = *S;
      S++;
      charcount++;
    }

  switch (DimFont)
    {
    case 8:

      for (k = 0; k < length; k++)
	{
	  TFT_Char (buffer[k], x, y, DimFont, Fcolor, Bcolor);
	  x = x + 8;
	}
      break;

    case 16:
      for (k = 0; k < length; k++)
	{
	  TFT_Char (buffer[k], x, y, DimFont, Fcolor, Bcolor);
	  x = x + 16;
	}
    }
}


void
TFT_Text32 (char *S, WORD x, WORD y, WORD Fcolor, WORD Bcolor)
{

  BYTE length, k;
  WORD buffer[10] = { 0 };
  BYTE charcount = 0;
  BYTE iCount;

  length = strlen (S);

  while (*S != 0)
    {
      buffer[charcount] = *S;
      S++;
      charcount++;
    }


  for (k = 0; k < length; k++)
    {
      //Erase area in between characters
      TFT_Set_Address ((x + 32), y, (x + 35), y + 32);
      Write_Data (Bcolor);
      for (iCount = 0; iCount < 4 * 32; iCount++)
	{
	  Write_Command (WRITEDATA);
	}
      TFT_32Char (buffer[k], x, y, Fcolor, Bcolor);
      x = x + 36;
    }
}


void
TFT_32Char (char C1, unsigned int x, unsigned int y, unsigned int Fcolor,
	    unsigned int Bcolor)
{

  unsigned char *ptrFont;
  unsigned int Cptrfont;
  unsigned int k, i, lineCount;
  volatile unsigned char cbit;

  ptrFont = (unsigned char *) FONT_32x32;
  Cptrfont = (C1 - 0x30) * 128;
  ptrFont = ptrFont + Cptrfont;
  Set_GDRAM_Address (x, y);

  for (i = 0; i < 32; i++)
    {
      Set_GDRAM_Address (x, y + i);
      for (k = 0; k < 4; k++)
	{
	  for (lineCount = 0; lineCount < 8; lineCount++)
	    {
	      cbit = (*ptrFont << lineCount) & 0x80;
	      if (cbit == 0x80)
		Write_Data (Fcolor);
	      else
		{
		  Write_Data (Bcolor);
		}
	    }
	  ptrFont++;
	}
    }
}

void
TFT_Char (char C1, unsigned int x, unsigned int y, unsigned char DimFont,
	  unsigned int Fcolor, unsigned int Bcolor)
{

  unsigned char *ptrFont;
  unsigned int *ptrintFont;
  unsigned int Cptrfont, y_new;
  unsigned char font8x8[8];
  unsigned int font16x16[16];
  unsigned int k, i, x_new, print1, print2;
  unsigned int print3;


  switch (DimFont)
    {
    case 8:
      {
	ptrFont = (unsigned char *) FONT_8x8;
	Cptrfont = (C1 - 32) * 8;
	ptrFont = ptrFont + Cptrfont;
	TFT_Set_Address (x, y, x, y);
	x_new = x;
	y_new = y;
	for (k = 0; k < 8; k++)
	  {
	    font8x8[k] = *ptrFont;
	    ptrFont++;
	  }

	for (i = 0; i < 8; i++)
	  {
	    for (k = 0; k < 8; k++)
	      {
		print1 = (font8x8[i] & 0x80);
		print2 = print1 >> 7;
		if (print2 == 1)
		  {
		    TFT_Dot (x_new, y_new, Fcolor);
		    x_new++;
		  }
		else
		  {
		    TFT_Dot (x_new, y_new, Bcolor);
		    x_new++;
		    TFT_Set_Address (x_new, y, x_new, y);
		  }
		font8x8[i] = font8x8[i] << 1;
	      }
	    y_new++;
	    x_new = x;
	    TFT_Set_Address (x, y_new, x, y_new);
	  }

      }
      break;


    case 16:
      {
	ptrintFont = (unsigned int *) FONT_16x16;
	Cptrfont = (C1 - 32) * 16;
	ptrintFont = ptrintFont + Cptrfont;

	TFT_Set_Address (x, y, x + 15, y + 15);
	for (k = 0; k < 16; k++)
	  {
	    font16x16[k] = *ptrintFont;
	    ptrintFont++;
	  }

	for (i = 0; i < 16; i++)
	  {
	    for (k = 0; k < 16; k++)
	      {
		print3 = (font16x16[i] & 0x8000);
		if ((print3 >> 15) == 1)
		  {
		    //TFT_Dot(x_new,y_new,Fcolor);
		    Write_Data (Fcolor);
		    x_new++;
		  }
		else
		  {
		    //TFT_Dot(x_new,y_new,Bcolor);
		    Write_Data (Bcolor);
		    x_new++;
		  }
		font16x16[i] = font16x16[i] << 1;
	      }
	  }
      }
    }
}


void
TFT_Box (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,
	 unsigned int color)
{
  unsigned int i, j;

  TFT_Set_Address (x1, y1, x2, y2);
  Write_Data (color);
  for (i = y1; i <= y2; i++)
    {
      for (j = x1; j <= x2; j++)
	{
	  SendCommand (WRITEDATA);
	}
  }
}


void
TFT_Dot (unsigned int x, unsigned int y, unsigned int color)
{
  TFT_Set_Address (x, y, x, y);
  Write_Data (color);
}


void
TFT_Line (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,
	  unsigned int color)
{
  int x, y, addx, addy, dx, dy;
  long int P;
  unsigned int i;

  dx = abs (x2 - x1);
  dy = abs (y2 - y1);
  x = x1;
  y = y1;

  if (x1 > x2)
    {
      addx = -1;
    }
  else
    {
      addx = 1;
      if (y1 > y2)
	{

	  addy = -1;

	}

      else

	{

	  addy = 1;

	}



      if (dx >= dy)

	{

	  P = (2 * dy) - dx;

	  for (i = 1; i < dx + 2; i++)

	    {

	      TFT_Dot (x, y, color);

	      if (P < 0)

		{

		  P = P + (2 * dy);

		  x = (x + addx);

		}

	      else

		{

		  P = P + (2 * dy) - (2 * dx);

		  x = x + addx;

		  y = y + addy;

		}

	    }

	}

      else

	{

	  P = (2 * dx) - dy;

	  for (i = 1; i < dy + 2; i++)

	    TFT_Dot (x, y, color);

	}

      if (P < 0)

	{

	  P = P + (2 * dx);

	  y = y + addy;

	}

      else
	{
	  P = P + (2 * dx) - (2 * dy);

	  x = x + addx;

	  y = y + addy;

	}
    }
}


void
TFT_H_Line (unsigned char x1, unsigned char x2, unsigned char y_pos,
	    unsigned int color)
{
  unsigned int k;
  for (k = x1; k <= x2; k++)
    {
      TFT_Dot (k, y_pos, color);
    }
}


void
TFT_V_Line (unsigned int y1, unsigned int y2, unsigned char x_pos,
	    unsigned int color)
{

  unsigned int k;

  for (k = y1; k <= y2; k++)

    {
      TFT_Dot (x_pos, k, color);
    }

}


/* Writes an unfilled rectangle */
void
TFT_Rectangle (unsigned int x1, unsigned int y1, unsigned int x2,
	       unsigned int y2, unsigned int color)
{

  TFT_H_Line (x1, x2, y1, color);
  TFT_H_Line (x1, x2, y2, color);
  TFT_V_Line (y1, y2, x1, color);
  TFT_V_Line (y1, y2, x2, color);
}


void
TFT_Circle (WORD x, WORD y, BYTE radius, BYTE fill, WORD color)
{

  int a_, b_, P;

  a_ = 0;

  b_ = radius;

  P = 1 - radius;

  while (a_ <= b_)

    {

      if (fill == 1)

	{

	  TFT_Line (x - a_, y + b_, x + a_, y + b_, color);

	  TFT_Line (x - a_, y - b_, x + a_, y - b_, color);

	  TFT_Line (x - b_, y + a_, x + b_, y + a_, color);

	  TFT_Line (x - b_, y - a_, x + b_, y - a_, color);

	}

      else

	{

	  TFT_Dot (a_ + x, b_ + y, color);

	  TFT_Dot (b_ + x, a_ + y, color);

	  TFT_Dot (x - a_, b_ + y, color);

	  TFT_Dot (x - b_, a_ + y, color);

	  TFT_Dot (b_ + x, y - a_, color);

	  TFT_Dot (a_ + x, y - b_, color);

	  TFT_Dot (x - a_, y - b_, color);

	  TFT_Dot (x - b_, y - a_, color);

	}


      if (P < 0)

	{

	  P = (P + 3) + (2 * a_);

	  a_ += 1;

	}

      else

	{

	  P = (P + 5) + (2 * (a_ - b_));

	  a_ += 1;

	  b_ -= 1;

	}

    }

}





//Set Absolute GDRAM address. x Range is 0-319. Y Range is 0-239; 
//This sets the absolute address for writes. 
void
Set_GDRAM_Address (int x1, int y1)
{

  WriteCommandData (0x004E, x1);	// Set GDDRAM X address
  WriteCommandData (0x004F, y1);	// Set GDDRAM Y address
  Write_Command (0x22);

}

void
TestLargeFont (void)
{

  int yStart, iCount;

  char mydigit = '0';


  TFT_Fill (BLACK);

  yStart = 230;

  for (iCount = 0; iCount < 2; iCount++)

    {

      TFT_32Char (mydigit, 0, yStart, WHITE, BLUE);

      mydigit = mydigit + 1;

      TFT_32Char (mydigit, 36, yStart, WHITE, BLUE);

      mydigit = mydigit + 1;

      TFT_32Char (mydigit, 72, yStart, WHITE, BLACK);

      mydigit = mydigit + 1;

      TFT_32Char (mydigit, 146, yStart, WHITE, BLACK);

      mydigit = mydigit + 1;

      TFT_32Char (mydigit, 182, yStart, WHITE, BLACK);

      mydigit = mydigit + 1;

      yStart += 36;

    }

  TFT_Text32 ("012345", 0, 230, GREEN, RED);

}




/*
sub procedure TFT_Image(dim pos_x,pos_y,dim_x,dim_y as word,dim const picture as ^byte)
dim k,i,x,y,temp as word
TFT_CS  = 0
x =pos_x
y =pos_y

temp = picture^
picture = picture +1
temp = (temp << 8)
temp = temp + picture^
picture = picture +1

TFT_Set_Address(pos_x,pos_y,dim_x,dim_y)

for k = 0 to dim_y - 1
for i = 0 to dim_x - 1

TFT_Set_Address(x,y,x,y)
Write_Data(temp)
temp = picture^
picture = picture +1
temp = (temp << 8)
temp = temp + picture^
picture = picture +1
inc(x)
next i
x = 0
inc(y)
next k
TFT_CS  = 1
end sub
*/
