#ifndef PILCDCHAR_H_
#define PILCDCHAR_H_

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

/**
 * Write a byte to the display. 
 *
 * @param i2cfd open i2c driver reference
 * @param bytetoWrite Byte to write
 * @param cmd 0=Command 1=Data
 */ 
extern void WriteI2CByte(int i2cfd, unsigned char bytetoWrite, int cmd); 

/**
 * Set up 4 bit interface
 *
 * @param i2cfd File descriptor 
 */
extern void Setup4bit(int i2cfd); 

/**
 * Clear Display
 *
 * @param i2cfd File descriptor 
 */
extern void DisplayClear(int i2cfd); 

/**
 * Write a string to the display at starting position
 *
 * @param i2cfd File descriptor 
 * @param row Starting row number
 * @param ypos Starting y position
 * @param message[] String to send. 
 */
extern void WriteString(int i2cfd, int row, int ypos, char message[]); 

#endif /* PILCDCHAR_H_ */
