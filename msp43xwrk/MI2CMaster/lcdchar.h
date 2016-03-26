/*
 * lcdchar.h
 *
 *  Created on: Mar 25, 2016
 *      Author: andy
 */

#ifndef LCDCHAR_H_
#define LCDCHAR_H_

// Constants from the data sheet Command Table Instruction codes and timings
#define LCD_CLEAR_DISPLAY 		0b00000001
#define LCD_RETURN_HOME 		0b00000010
#define LCD_ENTRY_MODE_SET 		0b00000100
#define ENTRY_MODE_LEFT		0b00000010
#define ENTRY_MODE_S        0b00000001
#define LCD_DISPLAY_ON_OFF 		0b00001000
#define DISPLAY_ON   		0b00000100
#define DISPLAY_CURSOR		0b00000010
#define	DISPLAY_CURSOR_BLINK	0b00000001
#define LCD_CURSOR_DISPLAY   	0b00010000
#define CURSOR_DISPLAY_SC   0b00001000
#define CURSOR_DISPLAY_RL   0b00000100
#define LCD_FUNCTION_SET     	0b00100000
#define LCD_CGRAM_ADDR			0b01000000
#define LCD_DDRAM_ADDR			0b10000000

#define LCD_ROW_OFFSET_ADDR		0x40

enum operation
{
	COMMAND = 0,
	DATA = 1
};




#endif /* LCDCHAR_H_ */
