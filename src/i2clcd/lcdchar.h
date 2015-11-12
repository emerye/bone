#ifndef LCDCHAR_H_
#define LCDCHAR_H_

#define RS 1
#define READWRITE 2
#define ENABLE 4
#define BACKLED 8

#define BIT0 1
#define BIT1 2
#define BIT2 4
#define BIT3 8
#define BIT4 0x10 
#define BIT5 0x20
#define BIT6 0x40 
#define BIT7 0x80 


extern void WriteI2CByte(int i2cfd, unsigned char bytetoWrite, int cmd); 
extern void Setup4bit(int i2cfd); 
extern void DisplayClear(int i2cfd); 
extern void WriteString(int i2cfd, int row, int ypos, char message[]); 

#endif /* LCDCHAR_H_ */
