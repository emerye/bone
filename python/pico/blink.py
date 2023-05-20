# Blink LED on pico w
import sys
from machine import Pin
import utime as Time
import uasyncio


sleeptime = 0.10
led = Pin("LED", Pin.OUT)

async def blink(delay):
    
    while(True):
        await uasyncio.sleep_ms(delay)
        led.toggle()
        print("in blink")
            
            
async def main():
    uasyncio.run(blink(500))

uasyncio.run(main())
