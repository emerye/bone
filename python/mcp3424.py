import sys
import os
import subprocess
from rpi_lcd import LCD
from smbus2 import SMBus
from time import sleep


correction = 0.000000

samples = 60
ch2file = "cat /sys/bus/iio/devices/iio:device0/in_voltage1_raw"
adcdir = "/sys/bus/iio/devices/iio:device0/"

lcdobj= LCD()

while True:
  voltage = int(subprocess.check_output(ch2file, shell=True))
  print("Voltage read:", voltage)
  voltage = (voltage + (voltage * correction)) / 4.0   #60 samples per second

  
  lcdobj.text(format(voltage, ".1f")+ " mV", 1)
  voltlcd = format(voltage, ".1f")
  print(voltlcd)
  sleep(0.5)
