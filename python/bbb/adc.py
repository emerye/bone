#!/usr/bin/env python3

import Adafruit_BBIO.ADC as adc 
import time

adc.setup()
while (1): 
    value = adc.read("P9_40")
    print("Value %2.3f" %(value * 1.8))
    time.sleep(0.5)
adc.cleanup()
