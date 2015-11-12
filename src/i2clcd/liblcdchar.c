#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
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

//
void
WriteI2CNibble (int i2cfd, unsigned char msbtoWrite, int cmd)
{
  int ret;
  unsigned char bytetoWrite = BACKLED;

  if (cmd > 1)
    puts ("Type is greater than 1 in function WriteI2CByte\n");

  bytetoWrite = bytetoWrite | (msbtoWrite & 0xF0) | ENABLE | cmd;
  ret = i2c_smbus_write_byte (i2cfd, bytetoWrite);
  if (ret < 0)
    puts ("Failed to write byte 1 in WriteNibble.\n");
  bytetoWrite &= ~ENABLE;
  ret = i2c_smbus_write_byte (i2cfd, bytetoWrite);
  if (ret < 0)
    puts ("Failed to write byte 2 in WriteNibble\n");

  bytetoWrite |= ENABLE;
  ret = i2c_smbus_write_byte (i2cfd, bytetoWrite);
  if (ret < 0)
    puts ("Failed to write byte 3 in WriteNIbble.\n");
}


//Command = 0  Data = 1
void
WriteI2CByte (int i2cfd, unsigned char bytetoWrite, int cmd)
{
  unsigned char lower = (bytetoWrite << 4) & 0b11110000;
  unsigned char upper = bytetoWrite & 0b11110000;

  if (cmd > 1)
    puts ("Type is greater than 1 in function WriteI2CByte\n");

  WriteI2CNibble (i2cfd, upper, cmd);
  WriteI2CNibble (i2cfd, lower, cmd);
}

/**
Setup 4 bit display
*/
void
Setup4bit (int i2cfd)
{
  usleep (20000);		//Wait 15msec after power on.   
  WriteI2CNibble (i2cfd, 0x30, 0);	//Manual write of Wake up!(first)
  usleep (LCD_LONG_DELAY);	//Sleep for at least 5ms

  WriteI2CNibble (i2cfd, 0x30, 0);	//Toggle the E bit, sends on falling edge
  usleep (LCD_SHORT_DELAY);	//Sleep for at least 160us

  WriteI2CNibble (i2cfd, 0x30, 0);	//Manual write of Wake up! (second)
  usleep (LCD_SHORT_DELAY);

  WriteI2CNibble (i2cfd, 0x20, 0);	//Function set to 4 bit         
  usleep (LCD_SHORT_DELAY);

  WriteI2CByte (i2cfd, 0x28, 0);	//Set 4-bit/2-line
  // Default Cursor, Display and Entry states set in the constructor
  usleep (100);

  WriteI2CByte (i2cfd, LCD_CLEAR_DISPLAY, 0);
  usleep (2000);
  WriteI2CByte (i2cfd, LCD_RETURN_HOME, 0);
  usleep (2000);
  WriteI2CByte (i2cfd, LCD_CURSOR_DISPLAY, 0);
  usleep (100);
  WriteI2CByte (i2cfd, LCD_DISPLAY_ON_OFF | DISPLAY_ENTIRE, 0);

  usleep (100);
  WriteI2CByte (i2cfd, LCD_ENTRY_MODE_SET | ENTRY_MODE_LEFT, 0);

  WriteI2CByte (i2cfd, LCD_RETURN_HOME, 0);
  usleep (2000);

}

//Prints a string starting at position. 
void
WriteString (int i2cfd, int row, int ypos, char message[])
{
  int stLength = strlen (message);
  int i, address;

  switch (row)
    {
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
  WriteI2CByte (i2cfd, (unsigned char) address, 0);
  for (i = 0; i < stLength; i++)
    {
      if (message[i] > 0x1f)
	{
	  WriteI2CByte (i2cfd, message[i], 1);
	}
    }
}


/***
 * Clears the display by passing the LCD_CLEAR_DISPLAY command
 */
void
DisplayClear (int i2cfd)
{
  WriteI2CByte (i2cfd, LCD_CLEAR_DISPLAY, 0);
  usleep (LCD_LONG_DELAY);	//data sheets states that a delay of 1.52ms is required
}

/***
 * Returns the cursor to the 00 address by passing the LCD_RETURN_HOME command
 */
void
DisplayHome (int i2cfd)
{
  WriteI2CByte (i2cfd, LCD_RETURN_HOME, 0);
  usleep (LCD_LONG_DELAY);	//data sheets states that a delay of 1.52ms is required
}
