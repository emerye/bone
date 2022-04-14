"""SSD1309 demo (fonts)."""
import sys
import machine
from time import sleep
import utime
from machine import Pin, SPI
from ssd1309 import Display
from xglcd_font import XglcdFont

newevent = 0
starttick = utime.ticks_ms()

def intcount(timer):
    global newevent
    newevent = 1


def init():
    
    global perfect
    global display      
    global curtick
    global starttick
    global perfect
    global display

    spi = SPI(0, baudrate=2000000, sck=Pin(18), mosi=Pin(19))
    #SSD1309 with width of 128 is used on bike
    display = Display(spi, dc=Pin(17), cs=Pin(16), rst=Pin(20), width=128)
    perfect = XglcdFont('fonts/PerfectPixel_23x32.c', 23, 32)
    display.clear()
    print("Fonts Loaded")

    irqpin = machine.Pin(22, machine.Pin.IN, machine.Pin.PULL_UP)
    irqpin.irq(handler=intcount, trigger=Pin.IRQ_FALLING)
    starttick = utime.ticks_ms()    


def dispCadence(cadvalue):

    display.clear()
    dispString = str("{:.0f}".format(cadvalue))
    text_width = perfect.measure_text(dispString)
    text_height = perfect.height
    display.draw_text(display.width // 2 - text_width // 2, display.height//2 - text_height, dispString, perfect, spacing=2 )
    display.present()
      
# Start
init()

display.draw_text(1, 19, 'POWER', perfect, spacing=2 )
display.present()
utime.sleep(2)

while(True):
  
    if newevent > 0:
        curtick = utime.ticks_ms()
        period = utime.ticks_diff(curtick, starttick)
        starttick = curtick 
        print('Period', period)
        if period==0: 
            continue
        cadence = (1.0 / (period/1000.0)) * 60
        newevent=0
        if cadence < 120:
            starttick = curtick 
            dispCadence(cadence)
            #print('Elapsed ', period/1000, 'rev', (1.0 / (period/1000)) * 60)
        #else:
            #curtick = utime.ticks_ms()

        newevent=0
        utime.sleep_ms(1)
    utime.sleep_ms(10)    

sleep(4)
display.cleanup()
sys.exit()
