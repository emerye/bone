from machine import Pin, I2C 
from time import sleep

BACKLIGHT = 1
ENABLE = 4

class CharLCD:
    classvar = 1

    def __init__(self, address):
        self.address = address
        #Micropython hardward default
        #self.i2cobj = I2C(0, scl=Pin(18), sda=Pin(19), freq=100000) 
        self.i2cobj = I2C(1)   #default SDA=GPIO26 SCL=GPIO25
        self.initLCD()

    def writeLCDNibble(self, bytetoWrite, commandType):
        data = bytearray(1)
        data[0] = (bytetoWrite & 0x00F0) + ENABLE + (BACKLIGHT * 8) + commandType
        self.i2cobj.writeto(self.address, data)
        data[0] = data[0] & ~ENABLE
        self.i2cobj.writeto(self.address, data)

    def initLCD(self):
        LCD_CLEAR_DISPLAY = 1
        LCD_RETURNHOME = 2
        LCD_CURSOR_DISPLAY = 0x10
        LCD_DISPLAY_OFF = 0x08
        DISPLAY_SHIFT = 0x04
        LCD_FUNCTION_SET = 0x20
        sleep(0.015)
        self.writeLCDNibble(0x30,0)
        sleep(0.010)
        self.writeLCDNibble(0x30,0)
        sleep(00.001)
        self.writeLCDNibble(0x30,0)
        sleep(0.010)
        self.writeLCDNibble(0x20,0)   # Set 4 bit mode
        sleep(00.010)

        self.writeLCDByte(LCD_FUNCTION_SET + 0xC, 0)   # Set lines 1, 5x11 dot matrix
        sleep(0.001)
        self.writeLCDByte(LCD_CLEAR_DISPLAY, 0)   # Clear screen
        sleep(0.003)
        self.writeLCDByte(LCD_DISPLAY_OFF, 0)    # Display Cursor Blink Off
        sleep(0.003)
        self.writeLCDByte(0x06, 0)   # Cursor right don't shift screen
        sleep(0.001)
        self.writeLCDByte(0x0C, 0)   # Display on
        sleep(0.001)

    def writeLCDByte(self, bytetoWrite, commandType):
        data = bytearray(1)
        data[0] = (bytetoWrite & 0x00F0) + ENABLE + (BACKLIGHT * 8) + commandType
        self.i2cobj.writeto(self.address, data)
        data[0] = data[0] & ~ENABLE
        self.i2cobj.writeto(self.address, data)
        data[0] =((bytetoWrite << 4) & 0x00F0) +ENABLE + (BACKLIGHT * 8) + commandType
        self.i2cobj.writeto(self.address, data)
        data[0]= data[0] & ~ENABLE
        self.i2cobj.writeto(self.address, data)

    # row 0 or 1
    def WriteString(self, row, pos, displayString):
        dataaddress = 0
        dataaddress = row + pos + 0x80
        if(row==1): dataaddress = dataaddress + 0x40 - 1
        self.writeLCDByte(dataaddress,0)
        for x in displayString:
            self.writeLCDByte(ord(x),1)

def main():
    charlcd = CharLCD(0x27)
   # charlcd.initLCD()
    charlcd.WriteString(0, 0, "Line1")
    charlcd.WriteString(1, 20, "Line4")
    charlcd.WriteString(0, 20, "Line3")
    charlcd.WriteString(1, 0, "Line2")
    for i in range(1, 20):
        charlcd.WriteString(0,9,str(i))
        sleep(0.5)

if __name__ == "__main__":
    main()

    
