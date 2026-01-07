#!/usr/bin/env python3


import os

#from os.path import isdir, isfile, join, realpath, normpath
#import subprocess
from time import sleep

# Add mcp3422 to /etc/modules
ADCSETUP ="/sys/bus/i2c/devices/i2c-1/new_device"
DEVDIR = "/sys/bus/iio/devices/iio:device0/"
SCALE = '0.00025' # X4
SCALE = '0.0005' # X2
SCALE = '0.001'  # X1


path = os.path.realpath(ADCSETUP)
if not os.path.isfile(path):
    print(f"Error directory {path} does not exist. Exiting!")
    exit()
        
 
#cfd = open((DEVDIR + 'in_voltage_sampling_frequency'),'w', encoding='utf=8') 
#cfd.write(60);
#cfd.close()
    
 
#Setup for mcp3422  Channel 4
scalefd = open((DEVDIR + 'in_voltage3_scale'),'r', encoding='utf-8')
scale = scalefd.readline().strip()
scalefd.close()
scale_factor = float(scale)

p = (DEVDIR + '/in_voltage3_raw')
for x in range(20):
    sleep(0.3)
    f = open(p,'r', encoding='utf-8')
    volt = float(f.readline().strip())
    print(volt * scale_factor)
