#!/usr/bin/env python3
from smbus2 import SMBus
from time import sleep

BACKLIGHT = 1
ENABLE = 4
address = 0x27

def initLCD():
    LCD_CLEAR_DISPLAY = 1
    LCD_RETURNHOME = 2
    LCD_CURSOR_DISPLAY = 0x10
    LCD_DISPLAY_OFF = 0x08
    DISPLAY_SHIFT = 0x04
    LCD_FUNCTION_SET = 0x20

    sleep(0.015)
    writeLCDNibble(0x30,0)
    sleep(0.010)
    writeLCDNibble(0x30,0)
    sleep(00.001)
    writeLCDNibble(0x30,0)
    sleep(0.010)
    writeLCDNibble(0x20,0)   # Set 4 bit mode
    sleep(00.010)

    writeLCDByte(LCD_FUNCTION_SET + 0xC, 0)   # Set lines 1, 5x11 dot matrix
    sleep(0.001)
    writeLCDByte(LCD_CLEAR_DISPLAY, 0)   # Clear screen
    sleep(0.003)
    writeLCDByte(LCD_DISPLAY_OFF, 0)    # Display Cursor Blink Off
    sleep(0.003)
    writeLCDByte(0x06, 0)   # Cursor right don't shift screen
    sleep(0.001)
    writeLCDByte(0x0C, 0)   # Display on
    sleep(0.001)


# Write a byte of data to LCD display
def writeLCDByte(bytetoWrite, commandType):
    bus = SMBus(1)

    highByte = (bytetoWrite & 0x00F0) + ENABLE + (BACKLIGHT * 8) + commandType
    bus.write_byte(address, highByte)
    highByte = highByte & ~ENABLE
    bus.write_byte(address, highByte)

    lowByte = ((bytetoWrite << 4) & 0x00F0) +ENABLE + (BACKLIGHT * 8) + commandType
    bus.write_byte(address, lowByte)
    lowByte = lowByte & ~ENABLE
    bus.write_byte(address, lowByte)

def writeLCDNibble(bytetoWrite, commandType):
    bus = SMBus(1)

    highByte = (bytetoWrite & 0x00F0) + ENABLE + (BACKLIGHT * 8) + commandType
    bus.write_byte(address, highByte)
    highByte = highByte & ~ENABLE
    bus.write_byte(address, highByte)

    # row 0 or 1
def WriteString(row, pos, displayString):
    address = row + pos + 0x80
    if(row==1): address = address + 0x40 - 1
    writeLCDByte(address,0)
    for x in displayString:
        writeLCDByte(ord(x),1)

bus = SMBus(1)
initLCD()

WriteString(0, 0, "0,0 Line1")
WriteString(1, 20, "1,20 Line4")
WriteString(0, 20, "0,20 Line3")
WriteString(1, 0, "1,0 Line2")

bus.close()

