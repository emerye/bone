from machine import Pin, I2C 
from time import sleep
import time
import machine

class LCDChar_4bit: 

    def __init__(self):

        self.lcdenable = Pin(2, Pin.OUT)
        self.lcdenable.value(0)
        self.lcdcommand = Pin(3, Pin.OUT)
        self.lcdcommand.value(1)
        self.lcdd7 = Pin(6, Pin.OUT)
        self.lcdd6 = Pin(7, Pin.OUT)
        self.lcdd5 = Pin(8, Pin.OUT)
        self.lcdd4 = Pin(9, Pin.OUT)
        self.initDisplay()
     

    def writeNibble(self, nibble, command):

        self.lcdcommand.value(command)

        if nibble > 15: 
            print('Argument in write_nibble is greater than 15.')

        if (nibble & 0x08) == 0x08:
            self.lcdd7.value(1)
        else:
            self.lcdd7.value(0)

        if (nibble & 0x04) == 0x04:
            self.lcdd6.value(1)
        else:
            self.lcdd6.value(0)

        if (nibble & 0x02) == 0x02:
            self.lcdd5.value(1)
        else:
            self.lcdd5.value(0)

        if (nibble & 0x01) == 0x01:
            self.lcdd4.value(1)
        else:
            self.lcdd4.value(0)
        time.sleep_us(1)

        #Toggle Enable
        self.lcdenable.value(1)
        time.sleep_us(10)
        self.lcdenable.value(0)
        time.sleep_us(10)


    def writeByte(self, byte, command):
        highByte = (byte >> 4) & 0x0f
        self.writeNibble(highByte, command)
        self.writeNibble(byte & 0x0F, command)


    def initDisplay(self):
        print("Init Display")
        time.sleep_ms(15)

        self.writeNibble(0x3, 0)
        time.sleep_ms(5)

        self.writeNibble(0x3, 0)
        time.sleep_ms(1)

        self.writeNibble(0x3, 0)
        time.sleep_ms(5)

        self.writeNibble(0x2, 0)   #Set 4 bit mode
        time.sleep_ms(1)

        self.writeByte(0x28, 0)     # Set 2 lines 5 by 8
        time.sleep_us(50)

        self.writeByte(0x01, 0)     #Clear screen return home
        time.sleep_ms(2)

        self.writeByte(0x8, 0)      #Display off, clear off, blink off
        time.sleep_us(50)

        self.writeByte(0x06, 0)     #Increment cursor don't shift display
        time.sleep_ms(2)

        self.writeByte(0xC, 0)      # Display on
        time.sleep_us(50)

    def clearScreen(self):
        self.writeByte(0x01, 0)     #Clear screen return home
        time.sleep_ms(2)

    def clearChars(self,row,startPos,numChars):
        startAddress = startPos + 0x80
        if row == 2:
            startAddress = startAddress + 0x40
        self.writeByte(startAddress,0)
        for i in range(numChars):
            self.writeByte(ord(' '), 1)

    def writeString(self, row, pos, displayString):
     
        startAddress = pos + 0x80
        if row == 2:
            startAddress = startAddress + 0x40
        self.writeByte(startAddress,0)
        for x in displayString:
            self.writeByte(ord(x),1)



def main():
    lcd = LCDChar_4bit()
    lcd.writeString(1,0,"HelloThere1111111111eeeeee")
    lcd.writeString(2,0,"abcdefghijkl")
    time.sleep(2) 
    lcd.clearChars(2,5,4)
    lcd.writeString(1,9,'Done')
    print('Done')
    

main()