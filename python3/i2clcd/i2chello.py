from smbus2 import SMBus
from time import sleep
import digitalio


address = 0x27
backlight = 1
ENABLE = 4

LCD_CLEAR_DISPLAY = 1
LCD_RETURNHOME = 2
LCD_CURSOR_DISPLAY = 0x10
LCD_DISPLAY_OFF = 0x08
DISPLAY_SHIFT = 0x04
LCD_FUNCTION_SET = 0x20


def initLCD():
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

    highByte = (bytetoWrite & 0x00F0) + ENABLE + (backlight * 8) + commandType
    bus.write_byte(address, highByte)
    highByte = highByte & ~ENABLE
    bus.write_byte(address, highByte)
    bus.write_byte(address, highByte | ENABLE)

    lowByte = ((bytetoWrite << 4) & 0x00F0) +ENABLE + (backlight * 8) + commandType
    bus.write_byte(address, lowByte)
    lowByte = lowByte & ~ENABLE
    bus.write_byte(address, lowByte)
    bus.write_byte(address, lowByte | ENABLE)


def writeLCDNibble(bytetoWrite, commandType):
    highByte = (bytetoWrite & 0x00F0) + ENABLE + (backlight * 8) + commandType
    bus.write_byte(address, highByte)
    highByte = highByte & ~ENABLE
    bus.write_byte(address, highByte)
    bus.write_byte(address, highByte | ENABLE)

    # row 0 or 1
def WriteString(row, pos, displayString):
    address = row + pos + 0x80
    if(row==1): address = address + 0x40 - 1
    writeLCDByte(address,0)
    for x in displayString:
        writeLCDByte(ord(x),1)



bus = SMBus(1)
initLCD()
writeLCDByte(0x80,0)    # Set address
sleep(0.05)
for x in range(0,2):
    writeLCDByte(ord('n'), 1)

writeLCDByte(0xC0,0)    # Set address
sleep(0.05)
for x in range(0,2):
    writeLCDByte(ord('M'), 1)

WriteString(0, 20, "Hello there Sunday")
WriteString(1, 20, "Hello There")
WriteString(0, 0, "Hello There")
WriteString(1, 0, "Hello There")
sleep(4)
backlight = 0
WriteString(1,60,"e")
bus.close()

