from machine import Pin, I2C 
from time import sleep
import machine

ENABLE = 4

class CharLCD:
    BACKLIGHT = 1
    LCD_CLEAR_DISPLAY = 1
    LCD_RETURNHOME = 2
    LCD_CURSOR_DISPLAY = 0x10
    LCD_DISPLAY_OFF = 0x08
    DISPLAY_SHIFT = 0x04
    LCD_FUNCTION_SET = 0x20

    def __init__(self, i2cobj, address):
        self.i2cobj = i2cobj
        self.address = address
        self.initLCD()

    def writeLCDNibble(self, bytetoWrite, commandType):
        data = bytearray(1)
        data[0] = (bytetoWrite & 0x00F0) + ENABLE + (self.BACKLIGHT * 8) + commandType
        self.i2cobj.writeto(self.address, data)
        data[0] = data[0] & ~ENABLE
        self.i2cobj.writeto(self.address, data)

    def initLCD(self):
        sleep(0.015)
        self.writeLCDNibble(0x30,0)
        sleep(0.010)
        self.writeLCDNibble(0x30,0)
        sleep(0.001)
        self.writeLCDNibble(0x30,0)
        sleep(0.010)
        self.writeLCDNibble(0x20,0)   # Set 4 bit mode
        sleep(0.010)

        self.writeLCDByte(self.LCD_FUNCTION_SET + 0xC, 0)   # Set lines 1, 5x11 dot matrix
        sleep(0.001)
        self.writeLCDByte(self.LCD_CLEAR_DISPLAY, 0)   # Clear screen
        sleep(0.003)
        self.writeLCDByte(self.LCD_DISPLAY_OFF, 0)    # Display Cursor Blink Off
        sleep(0.003)
        self.writeLCDByte(0x06, 0)   # Cursor right don't shift screen
        sleep(0.001)
        self.writeLCDByte(0x0C, 0)   # Display on
        sleep(0.001)

    def writeLCDByte(self, bytetoWrite, commandType):
        data = bytearray(1)
        data[0] = (bytetoWrite & 0x00F0) + ENABLE + (self.BACKLIGHT * 8) + commandType
        self.i2cobj.writeto(self.address, data)
        data[0] = data[0] & ~ENABLE
        self.i2cobj.writeto(self.address, data)
        data[0] =((bytetoWrite << 4) & 0x00F0) +ENABLE + (self.BACKLIGHT * 8) + commandType
        self.i2cobj.writeto(self.address, data)
        data[0]= data[0] & ~ENABLE
        self.i2cobj.writeto(self.address, data)

    # row 0 or 1
    def WriteString(self, row, pos, displayString):
        dataaddress = 0
        dataaddress = row + pos + 0x80
        if(row==1): dataaddress = pos + 0x40 + 0x80
        if(row==2): dataaddress = 0x14 + pos + 0x80
        if(row==3): dataaddress = 0x54 + pos + 0x80
        self.writeLCDByte(dataaddress,0)
        for x in displayString:
            self.writeLCDByte(ord(x),1)
            
    def lcdClear(self):
        self.writeLCDByte(self.LCD_CLEAR_DISPLAY, 0)

def main():
  
    i2cobj = I2C(0,
        scl=machine.Pin(17),
        sda=machine.Pin(16),
        freq=400000)

    charlcd = CharLCD(i2cobj, 0x27)
    charlcd.BACKLIGHT = 1
    print('Backlight =', charlcd.BACKLIGHT)
    charlcd.WriteString(0,0,'Hello')
    charlcd.WriteString(1,0,'Feb 29 2024')
    charlcd.WriteString(2,0,'Cold Rain Today')
    charlcd.WriteString(3,0,'Much snow tomorrow')
   
    print('*', end = '')
    print("End of program", end = "E")
    sleep(1)
    charlcd.lcdClear()

    
if __name__ == "__main__":
    main()

    
