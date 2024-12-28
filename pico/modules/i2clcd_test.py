from machine import Pin, I2C 
from time import sleep
import machine
import lcdBackPack as CharLCD

i2cobj = I2C

def main():
  
    i2cobj = I2C(0,
        scl=machine.Pin(17),
        sda=machine.Pin(16),
        freq=100000)

    charlcd = CharLCD.CharLCD(i2cobj, 0x27)
    charlcd.BACKLIGHT = 1
    print('Backlight =', charlcd.BACKLIGHT)
    charlcd.WriteString(0,0,'Hello')
    charlcd.WriteString(1,0,'Feb 29 2024')
    charlcd.WriteString(2,0,'Cold Rain Today')
    charlcd.WriteString(3,0,'Much snow tomorrow')
   
    print('*', end = '')
    print("End of program", end = "E")
    sleep(2)
    charlcd.lcdClear()

    
if __name__ == "__main__":
    main()

