#ifndef TFT_H_
#define TFT_H_

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

//480 x 272
#define XMAXPIXEL  479 
#define YMAXPIXEL  271 
#define SSD1963		 1

typedef enum
{
   WRITECMD,
   WRITEDATA
} CMDTYPE;


//Global
extern void *gpio_addr; 
extern int mmapFD;
extern volatile unsigned gDispProc; 

extern void TFT_Init(void);
extern void DelayMsec(unsigned int delay);
extern void Write_Command(unsigned int Wcommand);
extern void Write_Data(unsigned int Wdata);
extern void WriteCommandData(unsigned int Wcommand, unsigned int Wdata);
extern void TFT_Set_Address(unsigned int PX1, unsigned int PY1, unsigned int PX2, unsigned int PY2);
extern void TFT_FillDisp(unsigned int color);
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
extern void Set_GDRAM_Address(int xi, int y1);

extern void TFT_Char32(char C1, unsigned int x, unsigned int y, unsigned int Fcolor, unsigned int Bcolor);
extern void TFT_Text32(char * S, WORD x, WORD y, WORD Fcolor, WORD Bcolor);
extern void TestLargeFont(void);

//**************************************************
extern int SendWord(int data); 
extern int SendCommand(CMDTYPE); 
extern int MapGPIO();
extern void SendDisplayReset(); 

extern void Init_ssd1963();
extern void TFT_Write_Data(unsigned char, unsigned char); 
extern void Address_set(unsigned x1, unsigned y1, unsigned x2, unsigned y2); 

#endif /*TFT_H_*/
