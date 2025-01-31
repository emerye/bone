# SPDX-FileCopyrightText: Copyright (c) 2024 Liz Clark for Adafruit Industries
#
# SPDX-License-Identifier: MIT

import time
import board
import adafruit_mcp3421.mcp3421 as ADC
from adafruit_mcp3421.analog_in import AnalogIn
import binascii
from rpi_lcd import LCD
from smbus2 import SMBus
from time import sleep

i2c = board.I2C()

adcaddr = 0x68
count = int(0)
lcdobj = LCD()

def printlist():
    print(f"{count} ADC value: {adc_channel.value}")
    if adc.continuous_mode:
        mode = "continuous"
    else:
        mode = "one-shot"
    print(f"Mode: {mode}")
    print(f"ADC Voltage: {volts:.4f}")
    print(f"Current gain: {adc.gain}X")
    print(f"Current resolution: {adc.resolution}-bit")
    

lcdobj.text('Line 1', 1)
adc = ADC.MCP3421(i2c, gain=1, resolution=14, continuous_mode=True)
adc_channel = AnalogIn(adc)
# gain, resolution and mode can also be set after instantiation:

# set gain to 1, 2, 4 or 8x
# defaults to 1
# adc.gain = 1

# set resolution to 12, 14, 16 or 18
# defaults to 14
# adc.resolution = 14

# set continuous read mode True or False for one-shot
# defaults to True
# adc.continuous_mode = True

while True:
    count = count + 1
    volts = adc_channel.value * 0.000250
    vString = f"{volts:.4f}"
    lcdobj.text(vString, 3)
    print(f"{count} ADC Voltage: {volts:.4f}")
    #printlist()
    time.sleep(0.5)