import machine
import time


gp18 = machine.Pin(18, machine.Pin.OUT)
gp18.toggle()

starttick = time.ticks_ms()
while True:
    time.sleep_us(500)
    ticksdiff = time.ticks_diff(time.ticks_ms(), starttick)
    #print(ticksdiff)
    if(ticksdiff >= 499):
       # print("One second")
        starttick = time.ticks_ms()
        gp18.toggle()
        
gp18.high()
print("Done")
