# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

import time
import board
import busio
from adafruit_ads1x15 import ADS1015, AnalogIn, ads1x15 
from rpi_lcd import LCD

CHARDISP = 1

# Create the I2C bus

if CHARDISP != 0:
    lcdobj = LCD()
    lcdobj.text("LCD Message", 1)
    
i2c = board.I2C()
ads = ADS1015(i2c)

# Create the ADC object using the I2C bus
#ads = ADS.ADS1115(i2c, gain = 2)
# you can specify an I2C adress instead of the default 0x48
# ads = ADS.ADS1115(i2c, address=0x49)
#print(ads.gain)

# Create single-ended input on channel 0
chan = AnalogIn(ads, ads1x15.Pin.A0)

# Create differential input between channel 0 and 1
# chan = AnalogIn(ads, ADS.P0, ADS.P1)

print("{:>5}\t{:>5}".format("raw", "v"))
count = 0
while True:
    print("{:>5}\t{:>5.3f}".format(chan.value, chan.voltage))
    if CHARDISP != 0:
        count += 1
        lcdobj.text(f"{count}  {chan.voltage:.3f} V", 1)
    time.sleep(1.0)

    
