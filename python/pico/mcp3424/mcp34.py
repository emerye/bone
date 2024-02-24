import machine
import time
import struct
import sys

# MCP 3424
DEVADDR = 0x68
RESR1=194200.0
RESR2=8090.0

MUL = 0.039992



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

#data = bytearray()
trig = b'\x94'   # 14 bit   60 samples per second
#trig = b'\x98'   # 16 bit

i2c.writeto(DEVADDR,trig)

time.sleep_ms(100)

while(True):
    vlist = []
    sample = 4
    for count in range(sample):
        time.sleep_ms(20)
        data = i2c.readfrom(DEVADDR, 3)
        #print("Data tuple Length ", len(data))
        voltage = (struct.unpack('>h', data)[0]) * 0.000250   #14 bit
      
        #print(struct.unpack('s', data)[0])
        
        #print(struct.unpack('ss', data)[1])
        #print(struct.unpack('sss', data)[2])
        #print(data)
        #print(hex(data[2]))
        
        vlist.append(voltage)
        #vadc = voltage
    
    vadc = float((sum(vlist)) / sample)
    time.sleep_ms(500)
    print("vadc =", vadc * 25)
    currentinput = vadc / RESR2
#    print("R1 current =", currentinput)
#    print(currentinput * (RESR1 + RESR2))
          
    #print(f'0x{data.hex()}')
    #print(f'Config Register 0x{data[3]:x}')
    voltage = (struct.unpack('>h', data)[0]) * 0.000250   #14 bit
#    voltage = (ustruct.unpack('>h', data)[0]) * 0.0000625   #16 bit
#    print(f'{voltage:.3f} v  {count}')
    #print(ustruct.unpack('>h', data)[0]/4)


































































































































































































