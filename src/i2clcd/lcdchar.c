#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "lcdchar.h"


// Constants from the data sheet Command Table Instruction codes and timings
#define LCD_CLEAR_DISPLAY 	0b00000001
#define LCD_RETURN_HOME 	0b00000010
#define LCD_ENTRY_MODE_SET 	0b00000100
#define ENTRY_MODE_LEFT		0b00000010
#define ENTRY_MODE_S		0b00000001
#define LCD_DISPLAY_ON_OFF 	0b00001000
#define DISPLAY_ENTIRE		0b00000100
#define DISPLAY_CURSOR		0b00000010
#define	DISPLAY_CURSOR_POS	0b00000001
#define LCD_CURSOR_DISPLAY   	0b00010000
#define CURSOR_DISPLAY_SC	0b00001000
#define CURSOR_DISPLAY_RL	0b00000100
#define LCD_FUNCTION_SET     	0b00100000
#define LCD_CGRAM_ADDR		0b01000000
#define LCD_DDRAM_ADDR		0b10000000

#define LCD_LONG_DELAY 			1520
#define LCD_SHORT_DELAY			37
#define LCD_ROW_OFFSET_ADDR		0x40

// Wiring Configuration for the 74HC595
// D7 D6 D5 D4 0  0  E  RS  (on the display module)
// H  G  F  E  D  C  B  A   (on the 74HC595)
// RS 1=data 0=command
// write set
// E operation enable signal (falling edge triggered)

/***
 * Send a character command to the display module. These commands are generally set with
 * constants that are defined in the LCDCharacterDisplay.cpp file. The command is an 8-bit
 * value, but it is sent to the module in two parts - the lower 4-bits followed by the
 * upper 4-bits. This data is sent using the code as two 8-bit values, constructed as:
 *
 * 	D7 D6 D5 D4 0  0  E  RS
 *
 * where D7-D4 are the bits of the 8-bit values. 0, 0 are unused. The E bit needs to be
 * toggled high->low to write the value to the display and the RS states if the value being
 * sent is a command or data. For this method, which sends commands, RS will always be 0.
 *
 * @param i The character to send as a command
 */
void
WriteCmd (char i)
{
  char upper = (i << 4) & 0b11110000;
  char lower = i & 0b11110000;

  // need to write the lower data and toggle the E bit
  WriteSPIByte (lower | 0b00000010);	//lower 4 bits
  usleep (1);			//sleep for at least 300ns
  WriteSPIByte (lower | 0b00000000);	//lower 4 bits
  // need to write the upper data and toggle the E bit
  WriteSPIByte (upper | 0b00000010);	//lower 4 bits
  usleep (1);			//sleep for at least 300ns
  WriteSPIByte (upper | 0b00000000);	//lower 4 bits
}

/***
 * Initializes and sets up the display as described in the display data sheets.
 * This setup routine sets up a 4-bit display (sets 4-bit mode), sets the cursor,
 * turns on the display and sets entry mode.
 */
void
Setup4bit ()
{
  WriteSPIByte (0x32);		//Manual write of Wake up!(first)
  usleep (LCD_LONG_DELAY);	//Sleep for at least 5ms
  WriteSPIByte (0x30);		//Toggle the E bit, sends on falling edge
  usleep (LCD_SHORT_DELAY);	//Sleep for at least 160us
  WriteSPIByte (0x32);		//Manual write of Wake up! (second)
  usleep (LCD_SHORT_DELAY);
  WriteSPIByte (0x30);
  usleep (LCD_SHORT_DELAY);
  WriteSPIByte (0x32);		//Manual write of Wake up! (third)
  usleep (LCD_SHORT_DELAY);
  WriteSPIByte (0x30);
  usleep (LCD_SHORT_DELAY);
  WriteSPIByte (0x22);		//Set 4-bit mode
  usleep (LCD_SHORT_DELAY);
  WriteSPIByte (0x20);
  usleep (LCD_SHORT_DELAY);
  WriteCmd (0x28);		//Set 4-bit/2-line
  // Default Cursor, Display and Entry states set in the constructor
  usleep (100);

  WriteCmd (LCD_CLEAR_DISPLAY);
  usleep (2000);
  WriteCmd (LCD_RETURN_HOME);
  usleep (2000);
  WriteCmd (LCD_CURSOR_DISPLAY);
  usleep (100);
  WriteCmd (LCD_DISPLAY_ON_OFF | DISPLAY_ENTIRE );  

  usleep (100);
  WriteCmd (LCD_ENTRY_MODE_SET | ENTRY_MODE_LEFT);

  WriteCmd (LCD_RETURN_HOME);
  usleep (2000);

}

// Prints a string starting at position. 
void WriteString(int row, int ypos, char message[]) 
{
   int stLength = strlen(message);  
   int i, address; 

   switch(row) {
     case 0: 
	address = ypos; 
	break; 
     case 1:
	address = 0x40 + ypos; 
	break; 
     case 2:
	address = 20 + ypos; 
	break;
     case 3:
	address = 0x54 + ypos; 
	break; 
      }
    
     address += 0x80; 
     WriteCmd(address); 
     for(i=0; i<stLength; i++) {
     if (message[i] > 0x1f) {
	WriteData(message[i]);
        }
     }     
} 
        

/***
 * Send data to the display module. The data is an 8-bit value, but it is sent to the module
 * in two parts - the lower 4-bits followed by the upper 4-bits. This data is sent using the
 * code as two 8-bit values, constructed as:
 *
 * 	D7 D6 D5 D4 0  0  E  RS
 *
 * where D7-D4 are the bits of the 8-bit values. 0, 0 are unused. The E bit needs to be
 * toggled high->low to write the data to the display and the RS states if the value being
 * sent is a command or data. For this method, which sends data, RS will always be 1.
 *
 * @param i The character to send as data to the display module
 */
void
WriteData (char c)
{
  // 4-bit mode. Send lower 4 bits followed by higher 4 bits
  char upper = (c << 4) & 0b11110000;
  char lower = c & 0b11110000;
  // need to write the lower data and toggle the E bit
  WriteSPIByte (lower | 0b00000011);	//lower 4 bits
  usleep (1);			//sleep for at least 300ns
  WriteSPIByte (lower | 0b00000001);	//lower 4 bits
  // need to write the upper data and toggle the E bit
  WriteSPIByte (upper | 0b00000011);	//lower 4 bits
  usleep (1);			//sleep for at least 300ns
  WriteSPIByte (upper | 0b00000001);	//lower 4 bits
  usleep (1);
}

/***
 * Clears the display by passing the LCD_CLEAR_DISPLAY command
 */
void
DisplayClear ()
{
  WriteCmd (LCD_CLEAR_DISPLAY);
  usleep (LCD_LONG_DELAY);	//data sheets states that a delay of 1.52ms is required
}

/***
 * Returns the cursor to the 00 address by passing the LCD_RETURN_HOME command
 */
void
DisplayHome ()
{
  WriteCmd (LCD_RETURN_HOME);
  usleep (LCD_LONG_DELAY);	//data sheets states that a delay of 1.52ms is required
}

/***
 * Sets the Cursor position on the display module. 0,0 is the top-left corner. Address
 * 0x00 is row 0, 0x40 is row 1. So, (1,5) would be the second row and the 6th column
 * over. Display addresses are in the order:
 *   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
 *   40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F
 * For a 2x16 character display
 * @param row The number of rows in the display (e.g., 2)
 * @param column The number of characters wide the display is (e.g., 16)
 * @return -1 if the row or column is greater than the number of rows or columns
 */
/*
int LCDCharacterDisplay::setCursorPosition(int row, int column){

	if ((column>=this->width)||(row>=this->height)) return -1;
    row = row * LCD_ROW_OFFSET_ADDR;
    int value = (row + column) | LCD_DDRAM_ADDR;
	this->command(value);
	//printf("[%02x]", value);
    return 0;
}
*/
/**
 * Turn the display on or off.
 * @param displayOff pass true to turn the display off, false to turn it back on
 */
/*
void LCDCharacterDisplay::setDisplayOff(bool displayOff){
	if (displayOff){
		this->displayState = displayState & (~DISPLAY_ENTIRE); //bit inversion of DISPLAY_ENTIRE
		this->writeDisplayState();
	}
	else{
		this->displayState = displayState | DISPLAY_ENTIRE;
		this->writeDisplayState();
	}
}
*/
