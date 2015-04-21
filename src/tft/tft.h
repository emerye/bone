#ifndef TFT_H_
#define TFT_H_


//TFT Interface Ports
#define SHIFTDATA BIT0
#define SHIFTCLOCK BIT1
#define TFT_RESET BIT2 
#define TFT_RS BIT3
#define TFT_RD BIT4
#define TFT_WR BIT5

// Pin definitions for Touch Screen
#define D_CLK  	BIT0	
#define D_CS	BIT1	
#define D_IN	BIT2	
//P2 Inputs
#define D_BUSY		BIT3
#define D_OUT		BIT4
#define D_PENIRQ  	BIT5


#define BYTE unsigned char
#define WORD unsigned int
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

/**
* Write a word to the TFT shift register
* 
* @param data Data to write to shift register
*/
void WriteWordtoShift(unsigned int data);

extern void TFT_Init(void);
extern void DelayMsec(unsigned int delay);
extern void Write_Command(unsigned int Wcommand);
extern void Write_Data(unsigned int Wdata);
extern void Write_Command_Data(unsigned int Wcommand, unsigned int Wdata);
extern void TFT_Set_Address(unsigned int PX1, unsigned int PY1, unsigned int PX2, unsigned int PY2);
extern void TFT_Fill(unsigned int color);
extern unsigned int Set_color(unsigned char r, unsigned char g, unsigned char b);
extern void TFT_Text(char * S, WORD x, WORD y, BYTE DimFont, WORD Fcolor, WORD Bcolor);
extern void TFT_Char(char C1, unsigned int x, unsigned int y, unsigned char DimFont, unsigned int Fcolor, unsigned int Bcolor);
extern void TFT_Circle(WORD x, WORD y, BYTE radius, BYTE fill, WORD color);
extern void TFT_Box(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);
extern void TFT_Dot(unsigned int x, unsigned int y, unsigned int color);
extern void TFT_Line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);
extern void TFT_H_Line(unsigned char x1, unsigned char x2 ,unsigned char y_pos, unsigned int color);
extern void TFT_V_Line(unsigned int y1,unsigned int y2, unsigned char x_pos , unsigned int color);
extern void TFT_Rectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int color);
extern void CreateButton(WORD x1, WORD y1, WORD x2, WORD y2, WORD border, WORD backcolor, char *btnText);
//Set Absolute GDRAM address. x Range is 0-319. Y Range is 0-239; 
//This sets the absolute address for writes. 
extern void Set_GDRAM_Address(int xi, int y1);

extern void TFT_32Char(char C1, unsigned int x, unsigned int y, unsigned int Fcolor, unsigned int Bcolor);
extern void TFT_Text32(char * S, WORD x, WORD y, WORD Fcolor, WORD Bcolor);
extern void TestLargeFont(void);


#endif /*TFT_H_*/
