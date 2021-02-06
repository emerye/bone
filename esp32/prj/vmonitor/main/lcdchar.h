#ifndef LCDCHAR_H_
#define LCDCHAR_H_

#define RS 1
#define READWRITE 2
#define ENABLE 4
#define BACKLED 8

/**
 * Write a byte to the display. 
 *
 * @param i2cfd open i2c driver reference
 * @param bytetoWrite Byte to write
 * @param cmd 0=Command 1=Data
 */ 
extern void WriteI2CByte(unsigned char bytetoWrite, int cmd); 

/**
 * Set up 4 bit interface
 *
 * @param i2cfd File descriptor 
 */
extern void Setup4bit(); 

/**
 * Clear Display
 *
 * @param i2cfd File descriptor 
 */
extern void DisplayClear(); 

/**
 * Write a string to the display at starting position
 *
 * @param row Starting row number
 * @param ypos Starting y position
 * @param message[] String to send. 
 */
extern void WriteString(int row, int ypos, char message[]); 

#endif /* LCDCHAR_H_ */
