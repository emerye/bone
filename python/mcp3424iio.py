import sys

#from os.path import isdir, isfile, join, realpath, normpath
#import subprocess
from time import sleep

# Add mcp3422 to /etc/modules
ADCSETUP ="/sys/bus/i2c/devices/i2c-1/new_device"
DEVDIR = "/sys/bus/iio/devices/iio:device0/"

def init():
    open(ADCSETUP,'w').write(str("mcp3424 0x68"))
    
    p = DEVDIR + 'in_voltage2_scale'
    f = open(p,'w+').write('0.001')
    #f.flush()
    sleep(0.1)
    
init()    

p = (DEVDIR + 'in_voltage2_raw')

for x in range(10):
    sleep(1.1)
    f = open(p,'r')
    volt = f.readline().strip()

    print(volt)


    

        