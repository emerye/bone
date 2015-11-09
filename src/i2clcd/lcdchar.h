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

extern int i2cfd;	//I2C file 
extern int lcdreg;	//I2C file 

extern void WriteI2CByte(unsigned char bytetoWrite, int cmd); 
extern void Setup4bit(void); 
extern void DisplayClear(void); 
extern void WriteString(int row, int ypos, char message[]); 

#endif /* LCDCHAR_H_ */
