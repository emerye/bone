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

def init():
    path = os.path.realpath(ADCSETUP)
    if not os.path.isfile(path):
        print(f"Error directory {path} does not exist. Exiting!")
        exit()
        
 #   open(ADCSETUP,'w').write(str("mcp3424 0x68"))
 #   p = DEVDIR + 'in_voltage2_scale'
 #   f = open(p,'w+').write(SCALE)
    
#main
#ls
#init()
 
#Setup for mcp3422  Channel 1

p = (DEVDIR + '/in_voltage0_raw')

for x in range(10):
    sleep(0.5)
    f = open(p,'r')
    volt = f.readline().strip()

    print(int(volt))


    

        