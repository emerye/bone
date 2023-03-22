# Blink LED on pico w
import sys
from machine import Pin
import utime as Time
import ntptime

sleeptime = 0.10

led = Pin("LED", Pin.OUT)
for i in range(20):
    led.on()
    Time.sleep(sleeptime)
    led.off()
    Time.sleep(sleeptime)
    print(i)
    print("Time ", Time.time())
    