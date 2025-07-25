import os
import sys
import datetime
import time
import board
import adafruit_mcp3421.mcp3421 as ADC
from adafruit_mcp3421.analog_in import AnalogIn
import binascii
from rpi_lcd import LCD
from smbus2 import SMBus
from time import sleep
from adafruit_bus_device.i2c_device import I2CDevice

#set to zero to remove LCD character display
CHARDISP = 0

def printlist(ADC):
    print(f"ADC value: {adc_channel.value}")
    if adc.continuous_mode:
        mode = "continuous"
    else:
        mode = "one-shot"
    print(f"Mode: {mode}")
    #print(f"ADC Voltage: {volts:.4f}")
    print(f"Current gain: {adc.gain}X")
    print(f"Current resolution: {adc.resolution}-bit")
    

i2c = board.I2C()
if CHARDISP != 0:
    lcdobj = LCD()


def radc():
    count = int(0)
    
    now = datetime.datetime.now()
    file_name = "/home/andy/bone/python/blinka/adc.log"
    try:
        n = str(datetime.datetime.now())
        n = '#' + n
        with open(file_name, 'w+') as f:
            f.write(n)
            f.write('\n')
    except FileNotFoundError:
            print("File not found!", file=sys.stderr)
            pass


    adc = ADC.MCP3421(i2c, gain=1, resolution=14, continuous_mode=True, channel=2)
    adc_channel = AnalogIn(adc)
    i2c_device = I2CDevice(i2c, 0x68)
    with i2c_device as bus_device:
        result = bytearray(3)
        bus_device.readinto(result)
        print(result.hex())
        
        config_byte = bytearray(b'\xb7')
        bus_device.write(bytearray(config_byte))
    
        result = bytearray(6)
        bus_device.readinto(result)
        print(result.hex())
    
    
# gain, resolution and mode can also be set after instantiation:

# set gain to 1, 2, 4 or 8x
# defaults to 1
# adc.gain = 1

# set resolution to 12, 14, 16 or 18
# defaults to 14
# adc.resolution = 14

# set continuous read mode True or False for one-shot
# defaults to True
#    adc.continuous_mode = False
#    adc.channel = 2
    adc.gain = 1
#    adc.continuous_mode = True
  

#Current reading
    count = 0
    while True:
        #Current reading
        count = count + 1
        with i2c_device as bus_device:
            config_byte = bytearray(b'\xb4')  #gain 1
            bus_device.write(bytearray(config_byte))
        
        sleep(0.05)
        volts = adc_channel.value * 0.000250
        current = volts * 6.66 
        vString = f"{count} {volts:.4f} {current:.2f}"
        if CHARDISP != 0:
            lcdobj.text(vString, 3)
            
        print(f"{count} ADC: {adc_channel.value} ADC Voltage: {volts:.4f} Current: {current:.2f}")
        
        #50 Volt reading
        with i2c_device as bus_device:
            result = bytearray(6)
            bus_device.readinto(result)
            print(result.hex())
            
        with i2c_device as bus_device:
            config_byte = bytearray(b'\xC4')  #gain 1
            bus_device.write(bytearray(config_byte))
        sleep(0.05)
        volts = adc_channel.value * 0.000250
        print(f"Channel 3: {adc_channel.value} ADC Voltage: {volts:.4f}")
        
        #printlist()
        try:
            with open(file_name, 'a') as f:
                f.write(f"{count},{volts:.4f},{current:.2f}\n")
                f.close()
        except FileNotFoundError:
            print("File not found!", file=sys.stderr)
            
        time.sleep(3.0)
        
if __name__ == "__main__":
    radc()