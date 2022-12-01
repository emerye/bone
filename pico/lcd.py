from machine import Pin, I2C 
from time import sleep
import machine

ENABLE = 4

class CharLCD:
    BACKLIGHT = 1

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
        if(row==1): dataaddress = dataaddress + 0x40 - 1
        self.writeLCDByte(dataaddress,0)
        for x in displayString:
            self.writeLCDByte(ord(x),1)

def main():
    RefVoltage = 3.218
    sensor_temp = machine.ADC(4)
    conversion_factor = RefVoltage / (65535)
    offsetF = 1.0

    i2cobj = I2C(0, scl=Pin(5), sda=Pin(4), freq=400000)
    print(i2cobj) 
    charlcd = CharLCD(i2cobj, 0x27)
    charlcd.BACKLIGHT = 1
    print('Backlight =', charlcd.BACKLIGHT)

    loopCnt = 0
    for i in range(10):
        reading = sensor_temp.read_u16() * conversion_factor
        temperatureC = 27 - (reading - 0.706)/0.001721
        temperatureF = ((temperatureC * 1.8) + 32) + offsetF
        charlcd.WriteString(0,0, ("%.1f  " % temperatureF))
        charlcd.WriteString(1,0, f'{temperatureF:0.0}  ')
        charlcd.WriteString(0,20,str(int(temperatureF)))
        sleep(1)
        loopCnt += 1
        if loopCnt > 60:
            loopCnt = 0
            print('*')
            break
        else:
            print('*', end = '')    

    
if __name__ == "__main__":
    main()

    
