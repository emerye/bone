/*
 * i2clcd.h
 *
 *  Created on: Oct 23, 2021
 *      Author: andy
 */

#ifndef I2CLCD_H_
#define I2CLCD_H_

#define RS          1
#define READWRITE   2
#define ENABLE      4
#define BACKLED     8
#define DATA        1
#define COMMAND     0

// Constants from the data sheet Command Table Instruction codes and timings
#define LCD_CLEAR_DISPLAY   0b00000001
#define LCD_RETURN_HOME     0b00000010
#define LCD_ENTRY_MODE_SET  0b00000100
#define ENTRY_MODE_LEFT     0b00000010
#define ENTRY_MODE_S        0b00000001
#define LCD_DISPLAY_ON_OFF  0b00001000
#define DISPLAY_ENTIRE      0b00000100
#define DISPLAY_CURSOR      0b00000010
#define DISPLAY_CURSOR_POS  0b00000001
#define LCD_CURSOR_DISPLAY      0b00010000
#define CURSOR_DISPLAY_SC   0b00001000
#define CURSOR_DISPLAY_RL   0b00000100
#define LCD_FUNCTION_SET        0b00100000
#define LCD_CGRAM_ADDR      0b01000000
#define LCD_DDRAM_ADDR      0b10000000

extern volatile unsigned char TXData;
extern volatile unsigned char TXByteCtr;
extern volatile uint8_t ledState;

void SendNibble(uint8_t, uint8_t);
void WriteI2CByte (uint8_t bytetoWrite, uint8_t cmd);
void LCDinit(void);
void WriteString (int row, int ypos, const char message[]);

#endif /* I2CLCD_H_ */
