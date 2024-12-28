import machine
import time
import struct
import sys
import binascii
from machine import I2C
import lcdBackPack as CharLCD

i2c = I2C

# MCP 3424
DEVADDR = 0x68
RESR1=194200.0
RESR2=8090.0


def reg_write(i2c, addr, reg, data):
    """
    Write bytes to the specified register.
    """
    
    # Construct message
    msg = bytearray()
    msg.append(data)
    
    # Write out message to register
    i2c.writeto_mem(addr, reg, msg)
    
def reg_read(i2c, addr, reg, nbytes=1):
    """
    Read byte(s) from specified register. If nbytes > 1, read from consecutive
    registers.
    """
    
    # Check to make sure caller is asking for 1 or more bytes
    if nbytes < 1:
        return bytearray()
    
    # Request data from specified register(s) over I2C
    data = i2c.readfrom_mem(addr, reg, nbytes)
    
    return data


# Initialize I2C with pins
i2c = I2C(0,
        scl=machine.Pin(17),
        sda=machine.Pin(16),
        freq=400000)

#nit lcd display
charlcd = CharLCD.CharLCD(i2c, 0x27)
charlcd.WriteString(0,0,'Hello')
charlcd.WriteString(1,0,'Feb 29 2024')
charlcd.WriteString(2,0,'Cold Rain Today')
charlcd.WriteString(3,0,'Much snow tomorrow')
time.sleep(2)
charlcd.lcdClear()

#Channel 1 blown
#trig = b'\x94'   # 14 bit   60 samples per second
#Channel 2 OK 2/29/2024
trig = b'\x34'   # 14 bit   60 samples per second

#trig = b'\x98'   # 16 bit

i2c.writeto(DEVADDR,trig)

time.sleep_ms(100)

while(True):
    vlist = []
    sample = 4
    divider = 24.95  #8.1K + 194K
    divider = 1.0
    
    for count in range(sample):
        time.sleep_ms(20)
        data = i2c.readfrom(DEVADDR, 3)
        if count == 0:
            print(binascii.hexlify(data,'-').decode())
        #print(data.hex())
        
        voltage = (struct.unpack('>h', data)[0]) * 0.000250   #14 bit
#       voltage = (ustruct.unpack('>h', data)[0]) * 0.0000625   #16 bit
       
        vlist.append(voltage)
                 
    vadc = float((sum(vlist)) / sample)
    charlcd.WriteString(3,0,'                         ')
    charlcd.WriteString(3,0,f"{vadc:.4f} VDC")
    time.sleep_ms(500)
    print("vadc =", vadc * divider)
    currentinput = vadc / RESR2
    
#R2/R1+R2
#
#    voltage = (struct.unpack('>h', data)[0]) * 0.000250   #14 bit
#    voltage = (ustruct.unpack('>h', data)[0]) * 0.0000625   #16 bit
#    print(f'{voltage:.3f} v  {count}')
    #print(ustruct.unpack('>h', data)[0]/4)


































































































































































































