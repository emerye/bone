import sys
import os
import subprocess
from smbus2 import SMBus
from time import sleep

correction = 0.042201
samples = 60

ch2file = "cat /sys/bus/iio/devices/iio:device0/in_voltage1_raw"
adcdir = "/sys/bus/iio/devices/iio:device0/"

  
voltage = int(subprocess.check_output(ch2file, shell=True))
print("Voltage read:", voltage)
voltage = (voltage + (voltage * correction)) / 4.0   #60 samples per second
print('returned value:', voltage )