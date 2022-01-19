from machine import Pin, SPI
import ssd1306

hspi = SPI(1)  # sck=14 (scl), mosi=13 (sda), miso=12 (unused)

dc = Pin(4)    # data/command
rst = Pin(5)   # reset
cs = Pin(15)   # chip select, some modules do not have a pin for this

display = ssd1306.SSD1306_SPI(128, 64, hspi, dc, rst, cs)

display.text('Hello, World!', 0, 0, 1)
display.vline(50,0,64,1)

zero16x16 = [ 0x00,0x00,0x00,0x00,0x03,0x80,0x04,0x40,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x04,0x40,0x03,0x80,0x00,0x00,0x00,0x00 ]# // 0

def writeByte(x, y, dta):
    for i in range(8,0,-1):
        if ((dta >> i) & 1) == True:
            display.pixel(x,y,1)
        else: 
            display.pixel(x,y,0)
        x=x+1

x=0
y=0
byteCnt = 0
for yinc in range (16):
    for xinc in range(0,2):
        writeByte( (x), (y + yinc), zero16x16[byteCnt] )
        byteCnt += 1
        x = x + 8
    x=0
y = y + 1



display.show()

