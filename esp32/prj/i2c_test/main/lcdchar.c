#include <stdio.h>
#include <string.h>
#include "lcdchar.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "i2c.h"

//Set this to I2C port number
#define I2CPORTNUM 1

#define I2CBPADDR 0x27

// Constants from the data sheet Command Table Instruction codes and timings
#define LCD_CLEAR_DISPLAY 0b00000001
#define LCD_RETURN_HOME 0b00000010
#define LCD_ENTRY_MODE_SET 0b00000100
#define ENTRY_MODE_LEFT 0b00000010
#define ENTRY_MODE_S 0b00000001
#define LCD_DISPLAY_ON_OFF 0b00001000
#define DISPLAY_ENTIRE 0b00000100
#define DISPLAY_CURSOR 0b00000010
#define DISPLAY_CURSOR_POS 0b00000001
#define LCD_CURSOR_DISPLAY 0b00010000
#define CURSOR_DISPLAY_SC 0b00001000
#define CURSOR_DISPLAY_RL 0b00000100
#define LCD_FUNCTION_SET 0b00100000
#define LCD_CGRAM_ADDR 0b01000000
#define LCD_DDRAM_ADDR 0b10000000

#define LCD_LONG_DELAY 1520
#define LCD_SHORT_DELAY 37
#define LCD_ROW_OFFSET_ADDR 0x40

void WriteI2CNibble(unsigned char msbtoWrite, int cmd)
{
  int ret = 0;
  unsigned char bytetoWrite = BACKLED;

  if (cmd > 1)
    puts("Type is greater than 1 in function WriteI2CByte\n");

  bytetoWrite = bytetoWrite | (msbtoWrite & 0xF0) | ENABLE | cmd;
  ic2_master_write_byte(I2CPORTNUM, I2CBPADDR, bytetoWrite);
  if (ret < 0)
  {
    puts("Failed to write byte 1 in WriteNibble.\n");
  }
  bytetoWrite &= ~ENABLE;
  ic2_master_write_byte(I2CPORTNUM, I2CBPADDR, bytetoWrite);
  if (ret < 0)
    puts("Failed to write byte 2 in WriteNibble\n");

  bytetoWrite |= ENABLE;
  ic2_master_write_byte(I2CPORTNUM, I2CBPADDR, bytetoWrite);
  if (ret < 0)
    puts("Failed to write byte 3 in WriteNIbble.\n");
}

//Command = 0  Data = 1
void WriteI2CByte(unsigned char bytetoWrite, int cmd)
{
  unsigned char lower = (bytetoWrite << 4) & 0b11110000;
  unsigned char upper = bytetoWrite & 0b11110000;

  if (cmd > 1)
    puts("Type is greater than 1 in function WriteI2CByte\n");

  WriteI2CNibble(upper, cmd);
  WriteI2CNibble(lower, cmd);
}

/**
Setup 4 bit display
*/
void Setup4bit()
{
  vTaskDelay(15 / portTICK_RATE_MS); //Wait 15msec after power on.
  WriteI2CNibble(0x30, 0);       //Manual write of Wake up!(first)
  vTaskDelay(10 / portTICK_RATE_MS);  //Sleep for at least 5ms

  WriteI2CNibble(0x30, 0);   //Toggle the E bit, sends on falling edge
  vTaskDelay(10 / portTICK_RATE_MS); //Sleep for at least 160us

  WriteI2CNibble(0x30, 0); //Manual write of Wake up! (second)
  vTaskDelay(10 / portTICK_RATE_MS);

  WriteI2CNibble(0x20, 0); //Function set to 4 bit
  vTaskDelay(50 / portTICK_RATE_MS);

  WriteI2CByte(0x28, 0); //Set 4-bit/2-line
  // Default Cursor, Display and Entry states set in the constructor
  vTaskDelay(20 / portTICK_RATE_MS);

  WriteI2CByte(LCD_CLEAR_DISPLAY, 0);
  vTaskDelay(20 / portTICK_RATE_MS);
  WriteI2CByte(LCD_RETURN_HOME, 0);
  vTaskDelay(20 / portTICK_RATE_MS);
  WriteI2CByte(LCD_CURSOR_DISPLAY, 0);
  vTaskDelay(20 / portTICK_RATE_MS);
  WriteI2CByte(LCD_DISPLAY_ON_OFF | DISPLAY_ENTIRE, 0);
  vTaskDelay(20 / portTICK_RATE_MS);
  WriteI2CByte(LCD_ENTRY_MODE_SET | ENTRY_MODE_LEFT, 0);
  vTaskDelay(20 / portTICK_RATE_MS);
  WriteI2CByte(LCD_RETURN_HOME, 0);
  vTaskDelay(20 / portTICK_RATE_MS);
}

 /** Prints a string starting at row number and first character position. 
 * These are zero based.
 * int row   Row number
 * int ypos  Starting character position. 
 */
void WriteString(int row, int ypos, char *message)
{
  int stLength = strlen((char*) message);
  int i, address = 0;

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
  WriteI2CByte((unsigned char)address, 0);
  for (i = 0; i < stLength; i++)
  {
    if (message[i] > 0x1f)
    {
      WriteI2CByte(message[i], 1);
    }
  }
}

/***
 * Clears the display by passing the LCD_CLEAR_DISPLAY command
 */
void DisplayClear()
{
  WriteI2CByte(LCD_CLEAR_DISPLAY, 0);
  vTaskDelay(40 / portTICK_RATE_MS); //data sheets states that a delay of 1.52ms is required
}

/***
 * Returns the cursor to the 00 address by passing the LCD_RETURN_HOME command
 */
void DisplayHome()
{
  WriteI2CByte(LCD_RETURN_HOME, 0);
  vTaskDelay(40 / portTICK_RATE_MS); //data sheets states that a delay of 1.52ms is required
}
