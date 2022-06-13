from uctypes import INT32, UINT32
from machine import Pin, I2C 
from time import sleep
import machine
import time

cdsPeriodStart=time.ticks_ms()
cdsPeriodDuration=0
newcdsReady=False
p2=Pin

class CharLCD:
    BACKLIGHT = 1
    ENABLE = 4
    def __init__(self, i2cobj, address):
        self.i2cobj = i2cobj
        self.address = address
        self.initLCD()

    def writeLCDNibble(self, bytetoWrite, commandType):
        data = bytearray(1)
        data[0] = (bytetoWrite & 0x00F0) + self.ENABLE + (self.BACKLIGHT * 8) + commandType
        self.i2cobj.writeto(self.address, data)
        data[0] = data[0] & ~self.ENABLE
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
        data[0] = (bytetoWrite & 0x00F0) + self.ENABLE + (self.BACKLIGHT * 8) + commandType
        self.i2cobj.writeto(self.address, data)
        data[0] = data[0] & ~self.ENABLE
        self.i2cobj.writeto(self.address, data)
        data[0] =((bytetoWrite << 4) & 0x00F0) + self.ENABLE + (self.BACKLIGHT * 8) + commandType
        self.i2cobj.writeto(self.address, data)
        data[0]= data[0] & ~self.ENABLE
        self.i2cobj.writeto(self.address, data)

    # row 0 or 1
    def WriteString(self, row, pos, displayString):
        dataaddress = 0
        dataaddress = row + pos + 0x80
        if(row==1): dataaddress = dataaddress + 0x40 - 1
        self.writeLCDByte(dataaddress,0)
        for x in displayString:
            self.writeLCDByte(ord(x),1)


def readTemperature(charlcd):
    refVoltage = 3.23
    conversion_factor = refVoltage / (65535)
    offsetF = 0.5
    
    reading = machine.ADC(4).read_u16() * conversion_factor
    temperatureF = ((27 - (reading - 0.706)/0.001721) * 1.8) + 32 + offsetF
    charlcd.WriteString(0,0, ("%2.0f DegF   " % temperatureF))
           

def pinHandler(pin):
    global cdsPeriodDuration
    global cdsPeriodStart
    global newcdsReady

    now = time.ticks_ms()
    cdsPeriodDuration = time.ticks_diff(now, cdsPeriodStart)
    cdsPeriodStart = now
    newcdsReady=True

 
def main():
    global newcdsReady
    global cdsPeriodStart
    global cdsPeriodDuration
    global p2
    now=None

    p2 = Pin(2, Pin.IN, Pin.PULL_UP)
    p2.irq(handler=pinHandler, trigger=Pin.IRQ_FALLING)

    pwma = machine.PWM(Pin(16))    #Test signal
    pwma.freq(8)
    pwma.duty_u16(32768)

    i2cobj = I2C(0, scl=Pin(5), sda=Pin(4), freq=400000)
    print(i2cobj) 
    charlcd = CharLCD(i2cobj, 0x27)

    newcdsReady = False
    cdsPeriodStart = time.ticks_ms()
    readTemperature(charlcd)
    temperatureUpdateTime = time.ticks_add(time.ticks_ms(), 3000)

    #while(True):
    for i in range(100000):
        cdstimeout = time.ticks_add(cdsPeriodStart , 3000)
        now = time.ticks_ms()

        if now > cdstimeout:
            charlcd.WriteString(1,0,'----             ')
            cdsPeriodStart = now
            newcdsReady = False
            cdstimeout = time.ticks_add(cdsPeriodStart , 3000)

        if newcdsReady == True:
            if(cdsPeriodDuration > 5):
                charlcd.WriteString(1,0, (str((1000.0/cdsPeriodDuration)*60.0)+'   RPM    '))
            newcdsReady=False
            cdstimeout=time.ticks_add(time.ticks_ms(), 3000)

        if now > temperatureUpdateTime:
            readTemperature(charlcd)
            now = time.ticks_ms()
            temperatureUpdateTime = time.ticks_add(now, 3000)

        
        

if __name__ == "__main__":
    main()

    
