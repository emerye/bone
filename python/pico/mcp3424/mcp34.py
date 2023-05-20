import machine
import utime
import ustruct
import sys

DEVADDR = 0x68


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
i2c = machine.I2C(0,
                  scl=machine.Pin(17),
                  sda=machine.Pin(16),
                  freq=400000)

data = bytearray()
trig = b'\x94'   # 14 bit
#trig = b'\x98'   # 16 bit

i2c.writeto(DEVADDR,trig)

utime.sleep_ms(10)
count = 0

while(True):
    data = i2c.readfrom(DEVADDR, 4)
    utime.sleep_ms(100)
    count += 1
    #print(f'0x{data.hex()}')
    #print(f'Config Register 0x{data[3]:x}')
    voltage = (ustruct.unpack('>h', data)[0]) * 0.000250   #14 bit
#    voltage = (ustruct.unpack('>h', data)[0]) * 0.0000625   #16 bit
    print(f'{voltage:.3f} v  {count}')
    #print(ustruct.unpack('>h', data)[0]/4)

