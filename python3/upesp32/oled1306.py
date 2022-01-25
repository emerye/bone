from machine import Pin, SPI
import ssd1306
from time import sleep

hspi = SPI(1)  # sck=14 (scl), mosi=13 (sda), miso=12 (unused)

dc = Pin(4)    # data/command
rst = Pin(5)   # reset
cs = Pin(15)   # chip select, some modules do not have a pin for this

display = ssd1306.SSD1306_SPI(128, 64, hspi, dc, rst, cs)

display.text('Hello, World!', 0, 0, 1)
display.vline(50,0,64,1)

sevseg32x50 = [ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x60,0x0C,0xFF,0xFE,0xF0,0x1E,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3E,0x00,0x00,0x78,0x38,0x00,0x00,0x18,0x20,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x38,0x00,0x00,0x18,0x3E,0x00,0x00,0x78,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x1E,0x00,0x00,0xF0,0x0C,0xFF,0xFE,0x60,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,   #// 0
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0xF0,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #  // 1
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x60,0x00,0xFF,0xFE,0xF0,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x78,0x01,0xFF,0xFE,0x18,0x03,0xFF,0xFF,0x88,0x0F,0xFF,0xFF,0xE0,0x27,0xFF,0xFF,0xC0,0x39,0xFF,0xFF,0x00,0x3E,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x1E,0x00,0x00,0x00,0x0C,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  # // 2
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x60,0x00,0xFF,0xFE,0xF0,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x78,0x01,0xFF,0xFE,0x18,0x03,0xFF,0xFF,0x88,0x0F,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xC0,0x01,0xFF,0xFF,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0xFF,0xFE,0x60,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  # // 3
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x0C,0x00,0x00,0xF0,0x1E,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3E,0x00,0x00,0x78,0x39,0xFF,0xFE,0x18,0x23,0xFF,0xFF,0x88,0x0F,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xC0,0x01,0xFF,0xFF,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  # // 4
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x0C,0xFF,0xFE,0x00,0x1E,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x39,0xFF,0xFE,0x00,0x23,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xC0,0x01,0xFF,0xFF,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0xFF,0xFE,0x60,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  # // 5
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x0C,0xFF,0xFE,0x00,0x1E,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x39,0xFF,0xFE,0x00,0x23,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,0xE0,0x27,0xFF,0xFF,0xC0,0x39,0xFF,0xFF,0x18,0x3E,0x00,0x00,0x78,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x1E,0x00,0x00,0xF0,0x0C,0xFF,0xFE,0x60,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  # // 6
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x60,0x00,0xFF,0xFE,0xF0,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0x78,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  # // 7
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x60,0x0C,0xFF,0xFE,0xF0,0x1E,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3E,0x00,0x00,0x78,0x39,0xFF,0xFE,0x18,0x23,0xFF,0xFF,0x88,0x0F,0xFF,0xFF,0xE0,0x27,0xFF,0xFF,0xC0,0x39,0xFF,0xFF,0x18,0x3E,0x00,0x00,0x78,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x1E,0x00,0x00,0xF0,0x0C,0xFF,0xFE,0x60,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  # // 8
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x60,0x0C,0xFF,0xFE,0xF0,0x1E,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3F,0x00,0x01,0xF8,0x3E,0x00,0x00,0x78,0x39,0xFF,0xFE,0x18,0x23,0xFF,0xFF,0x88,0x0F,0xFF,0xFF,0xE0,0x07,0xFF,0xFF,0xC0,0x01,0xFF,0xFF,0x18,0x00,0x00,0x00,0x78,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF0,0x00,0xFF,0xFE,0x60,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x00,0x00,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  # // 9
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF  ]  # // 0xFF


zero16x16 = [ 0x00,0x00,0x00,0x00,0x03,0x80,0x04,0x40,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x04,0x40,0x03,0x80,0x00,0x00,0x00,0x00, # // 0
0x00,0x00,0x00,0x00,0x00,0x40,0x00,0xC0,0x01,0x40,0x02,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x00,0x00,0x00, # // 1
0x00,0x00,0x00,0x00,0x03,0xC0,0x04,0x40,0x08,0x20,0x00,0x20,0x00,0x20,0x00,0x40,0x00,0x40,0x00,0x80,0x01,0x00,0x02,0x00,0x04,0x00,0x0F,0xE0,0x00,0x00,0x00,0x00,  # // 2
0x00,0x00,0x00,0x00,0x03,0x80,0x04,0x40,0x08,0x40,0x00,0x40,0x00,0x80,0x03,0x80,0x00,0x40,0x00,0x20,0x00,0x20,0x08,0x20,0x0C,0x40,0x03,0x80,0x00,0x00,0x00,0x00,  # // 3
0x00,0x00,0x00,0x00,0x00,0x40,0x00,0xC0,0x01,0x40,0x01,0x40,0x02,0x40,0x02,0x40,0x04,0x40,0x08,0x40,0x0F,0xF0,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x00,0x00,0x00,  # // 4
0x00,0x00,0x00,0x00,0x01,0xF8,0x01,0x00,0x01,0x00,0x02,0x00,0x03,0xE0,0x02,0x10,0x00,0x08,0x00,0x08,0x00,0x08,0x02,0x08,0x01,0x10,0x00,0xE0,0x00,0x00,0x00,0x00,  # // 5
0x00,0x00,0x00,0x00,0x03,0x80,0x04,0x60,0x08,0x20,0x08,0x00,0x0B,0x80,0x0C,0x40,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x04,0x40,0x03,0x80,0x00,0x00,0x00,0x00,  # // 6
0x00,0x00,0x00,0x00,0x03,0xF8,0x00,0x10,0x00,0x10,0x00,0x20,0x00,0x20,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x00,0x00,0x00,  # // 7
0x00,0x00,0x00,0x00,0x00,0xE0,0x01,0x10,0x02,0x08,0x02,0x08,0x01,0x10,0x00,0xE0,0x01,0x10,0x02,0x08,0x02,0x08,0x02,0x08,0x01,0x10,0x00,0xE0,0x00,0x00,0x00,0x00,  # // 8
0x00,0x00,0x00,0x00,0x03,0x80,0x04,0x40,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x04,0x60,0x03,0xA0,0x00,0x20,0x08,0x20,0x04,0x40,0x07,0x80,0x00,0x00,0x00,0x00 ]  # // 9

grotesk32x64 = [ 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #// <space>
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x3F,0xFC,0x00,0x00,0x7F,0xFE,0x00,0x00,0x7F,0xFF,0x00,0x00,0xFF,0xFF,0x80,0x01,0xFE,0x7F,0x80,0x01,0xF8,0x1F,0xC0,0x03,0xF0,0x0F,0xC0,0x03,0xF0,0x07,0xE0,0x03,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xF0,0x07,0xC0,0x0F,0xF0,0x07,0xC0,0x0F,0xF0,0x0F,0xC0,0x1F,0xF0,0x0F,0xC0,0x1F,0xF0,0x0F,0xC0,0x3F,0xF0,0x0F,0xC0,0x3D,0xF0,0x0F,0x80,0x7D,0xF8,0x0F,0x80,0x79,0xF8,0x0F,0x80,0xF9,0xF8,0x0F,0x80,0xF1,0xF8,0x0F,0x81,0xF1,0xF8,0x0F,0x81,0xE1,0xF8,0x0F,0x83,0xE1,0xF8,0x0F,0x83,0xC1,0xF8,0x0F,0x87,0xC1,0xF8,0x0F,0x87,0x81,0xF8,0x0F,0x8F,0x81,0xF8,0x0F,0x8F,0x01,0xF8,0x0F,0x8F,0x01,0xF8,0x0F,0x9E,0x01,0xF8,0x0F,0x9E,0x01,0xF8,0x0F,0xBC,0x01,0xF8,0x0F,0xFC,0x01,0xF0,0x0F,0xF8,0x01,0xF0,0x0F,0xF8,0x01,0xF0,0x0F,0xF0,0x01,0xF0,0x07,0xF0,0x03,0xF0,0x07,0xE0,0x03,0xF0,0x07,0xE0,0x03,0xE0,0x07,0xE0,0x03,0xE0,0x03,0xE0,0x07,0xE0,0x03,0xF0,0x07,0xC0,0x03,0xF8,0x0F,0xC0,0x01,0xFC,0x1F,0xC0,0x01,0xFF,0xFF,0x80,0x00,0xFF,0xFF,0x00,0x00,0x7F,0xFF,0x00,0x00,0x3F,0xFE,0x00,0x00,0x1F,0xFC,0x00,0x00,0x07,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #// 0
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xF8,0x00,0x00,0x0F,0xF8,0x00,0x00,0x1F,0xF8,0x00,0x00,0x7F,0xF8,0x00,0x00,0xFF,0xF8,0x00,0x03,0xFF,0xF8,0x00,0x03,0xFF,0xF8,0x00,0x03,0xFC,0xF8,0x00,0x03,0xF1,0xF8,0x00,0x03,0xC1,0xF8,0x00,0x03,0x81,0xF8,0x00,0x02,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x01,0xFF,0xFF,0xF8,0x01,0xFF,0xFF,0xF8,0x01,0xFF,0xFF,0xF8,0x01,0xFF,0xFF,0xF8,0x01,0xFF,0xFF,0xF8,0x01,0xFF,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #// 1
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0x7F,0xFC,0x00,0x01,0xFF,0xFF,0x00,0x07,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,0xC0,0x0F,0xFF,0xFF,0xE0,0x0F,0xFE,0x3F,0xE0,0x0F,0xE0,0x07,0xF0,0x0F,0x80,0x03,0xF0,0x0E,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x0F,0xC0,0x00,0x00,0x0F,0xC0,0x00,0x00,0x1F,0x80,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0xFC,0x00,0x00,0x01,0xF8,0x00,0x00,0x03,0xF8,0x00,0x00,0x03,0xF0,0x00,0x00,0x07,0xE0,0x00,0x00,0x0F,0xC0,0x00,0x00,0x1F,0xC0,0x00,0x00,0x3F,0x80,0x00,0x00,0x3F,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0xFC,0x00,0x00,0x01,0xFC,0x00,0x00,0x03,0xF8,0x00,0x00,0x03,0xF0,0x00,0x00,0x07,0xE0,0x00,0x00,0x0F,0xFF,0xFF,0xFC,0x0F,0xFF,0xFF,0xFC,0x0F,0xFF,0xFF,0xFC,0x0F,0xFF,0xFF,0xFC,0x0F,0xFF,0xFF,0xFC,0x0F,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #// 2
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x80,0x00,0x00,0xFF,0xF8,0x00,0x07,0xFF,0xFE,0x00,0x07,0xFF,0xFF,0x00,0x07,0xFF,0xFF,0x80,0x07,0xFF,0xFF,0xC0,0x07,0xF8,0x7F,0xC0,0x07,0x80,0x0F,0xE0,0x04,0x00,0x07,0xE0,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x01,0xF0,0x00,0x00,0x01,0xF0,0x00,0x00,0x01,0xF0,0x00,0x00,0x01,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x0F,0xE0,0x00,0x00,0x3F,0xC0,0x00,0x1F,0xFF,0x80,0x00,0x3F,0xFF,0x00,0x00,0x3F,0xFC,0x00,0x00,0x3F,0xFC,0x00,0x00,0x3F,0xFF,0x00,0x00,0x1F,0xFF,0x80,0x00,0x00,0x3F,0xC0,0x00,0x00,0x0F,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x03,0xF0,0x00,0x00,0x01,0xF0,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x03,0xF0,0x08,0x00,0x07,0xF0,0x0E,0x00,0x0F,0xF0,0x0F,0xC0,0x3F,0xE0,0x0F,0xFF,0xFF,0xE0,0x0F,0xFF,0xFF,0xC0,0x0F,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,0x00,0x03,0xFF,0xFC,0x00,0x00,0x7F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #// 3
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x3F,0x80,0x00,0x00,0x7F,0x80,0x00,0x00,0x7F,0x80,0x00,0x00,0xFF,0x80,0x00,0x00,0xFF,0x80,0x00,0x01,0xFF,0x80,0x00,0x01,0xFF,0x80,0x00,0x01,0xDF,0x80,0x00,0x03,0xDF,0x80,0x00,0x03,0xDF,0x80,0x00,0x07,0x9F,0x80,0x00,0x07,0x9F,0x80,0x00,0x0F,0x1F,0x80,0x00,0x0F,0x1F,0x80,0x00,0x1E,0x1F,0x80,0x00,0x1E,0x1F,0x80,0x00,0x3C,0x1F,0x80,0x00,0x3C,0x1F,0x80,0x00,0x7C,0x1F,0x80,0x00,0x78,0x1F,0x80,0x00,0xF8,0x1F,0x80,0x00,0xF0,0x1F,0x80,0x01,0xF0,0x1F,0x80,0x01,0xE0,0x1F,0x80,0x03,0xE0,0x1F,0x80,0x03,0xC0,0x1F,0x80,0x07,0xC0,0x1F,0x80,0x07,0xC0,0x1F,0x80,0x0F,0x80,0x1F,0x80,0x0F,0x80,0x1F,0x80,0x1F,0x00,0x1F,0x80,0x1F,0x00,0x1F,0x80,0x1F,0xFF,0xFF,0xF8,0x1F,0xFF,0xFF,0xFC,0x1F,0xFF,0xFF,0xFC,0x1F,0xFF,0xFF,0xFC,0x1F,0xFF,0xFF,0xFC,0x1F,0xFF,0xFF,0xFC,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #// 4
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xFF,0xC0,0x03,0xFF,0xFF,0xE0,0x03,0xFF,0xFF,0xE0,0x03,0xFF,0xFF,0xE0,0x03,0xFF,0xFF,0xE0,0x03,0xFF,0xFF,0xE0,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE3,0xC0,0x00,0x03,0xFF,0xF8,0x00,0x03,0xFF,0xFE,0x00,0x03,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x03,0xFF,0xFF,0xC0,0x03,0xF0,0xFF,0xE0,0x03,0x00,0x1F,0xE0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x07,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x03,0xF0,0x08,0x00,0x07,0xF0,0x0E,0x00,0x0F,0xE0,0x0F,0xC0,0x3F,0xE0,0x0F,0xFF,0xFF,0xC0,0x0F,0xFF,0xFF,0x80,0x0F,0xFF,0xFF,0x80,0x0F,0xFF,0xFE,0x00,0x07,0xFF,0xFC,0x00,0x00,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #// 5
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x00,0x00,0x07,0xFF,0x00,0x00,0x0F,0xFF,0xC0,0x00,0x3F,0xFF,0xC0,0x00,0x7F,0xFF,0xC0,0x00,0x7F,0xFF,0xC0,0x00,0xFF,0x8F,0xC0,0x01,0xFE,0x00,0xC0,0x01,0xF8,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x07,0xC0,0x00,0x00,0x07,0xC0,0x00,0x00,0x07,0xC0,0x00,0x00,0x0F,0x80,0x00,0x00,0x0F,0x80,0x00,0x00,0x0F,0x80,0xE0,0x00,0x0F,0x87,0xFC,0x00,0x0F,0x8F,0xFF,0x00,0x0F,0x9F,0xFF,0x80,0x0F,0x3F,0xFF,0xC0,0x0F,0x7F,0xFF,0xC0,0x0F,0xFC,0x1F,0xE0,0x0F,0xF8,0x07,0xE0,0x0F,0xF0,0x03,0xF0,0x0F,0xE0,0x03,0xF0,0x0F,0xE0,0x01,0xF0,0x0F,0xC0,0x01,0xF8,0x0F,0xC0,0x01,0xF8,0x0F,0xC0,0x01,0xF8,0x0F,0xC0,0x01,0xF8,0x0F,0xC0,0x00,0xF8,0x0F,0xC0,0x00,0xF8,0x0F,0xC0,0x00,0xF8,0x0F,0xC0,0x00,0xF8,0x0F,0xC0,0x00,0xF8,0x0F,0xC0,0x00,0xF8,0x07,0xC0,0x01,0xF8,0x07,0xC0,0x01,0xF8,0x07,0xC0,0x01,0xF8,0x07,0xE0,0x01,0xF0,0x03,0xE0,0x03,0xF0,0x03,0xF0,0x03,0xF0,0x03,0xF0,0x07,0xF0,0x01,0xF8,0x0F,0xE0,0x01,0xFF,0x3F,0xE0,0x00,0xFF,0xFF,0xC0,0x00,0x7F,0xFF,0x80,0x00,0x3F,0xFF,0x00,0x00,0x1F,0xFE,0x00,0x00,0x07,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #// 6
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0xFF,0xF8,0x0F,0xFF,0xFF,0xF8,0x0F,0xFF,0xFF,0xF8,0x0F,0xFF,0xFF,0xF8,0x0F,0xFF,0xFF,0xF8,0x0F,0xFF,0xFF,0xF8,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x07,0xC0,0x00,0x00,0x0F,0xC0,0x00,0x00,0x0F,0xC0,0x00,0x00,0x0F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x01,0xF8,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x03,0xF0,0x00,0x00,0x07,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x07,0xC0,0x00,0x00,0x0F,0xC0,0x00,0x00,0x0F,0xC0,0x00,0x00,0x0F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x1F,0x80,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #// 7
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x1F,0xFC,0x00,0x00,0x7F,0xFE,0x00,0x00,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0x80,0x01,0xFF,0xFF,0xC0,0x03,0xFC,0x1F,0xE0,0x03,0xF0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x03,0xF0,0x07,0xE0,0x03,0xF0,0x07,0xC0,0x03,0xF0,0x07,0xC0,0x01,0xF0,0x07,0xC0,0x01,0xF0,0x07,0xC0,0x01,0xF0,0x07,0xC0,0x01,0xF0,0x07,0xC0,0x01,0xF0,0x07,0xC0,0x03,0xF0,0x07,0xE0,0x03,0xE0,0x03,0xE0,0x03,0xE0,0x03,0xF0,0x07,0xE0,0x01,0xF8,0x0F,0xC0,0x01,0xFE,0x3F,0x80,0x00,0xFF,0xFF,0x00,0x00,0x3F,0xFE,0x00,0x00,0x1F,0xFC,0x00,0x00,0x7F,0xFF,0x00,0x00,0xFF,0xFF,0x80,0x01,0xFE,0x3F,0xC0,0x03,0xF0,0x0F,0xE0,0x07,0xE0,0x07,0xE0,0x07,0xE0,0x03,0xF0,0x0F,0xC0,0x01,0xF0,0x0F,0xC0,0x01,0xF8,0x0F,0x80,0x01,0xF8,0x0F,0x80,0x01,0xF8,0x0F,0x80,0x00,0xF8,0x0F,0x80,0x00,0xF8,0x0F,0x80,0x00,0xF8,0x0F,0x80,0x00,0xF8,0x0F,0x80,0x01,0xF8,0x0F,0x80,0x01,0xF8,0x0F,0xC0,0x01,0xF8,0x0F,0xC0,0x01,0xF8,0x0F,0xE0,0x03,0xF0,0x07,0xE0,0x07,0xF0,0x07,0xF0,0x0F,0xE0,0x03,0xFE,0x3F,0xE0,0x03,0xFF,0xFF,0xC0,0x01,0xFF,0xFF,0xC0,0x00,0xFF,0xFF,0x00,0x00,0x3F,0xFE,0x00,0x00,0x0F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #// 8
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xC0,0x00,0x00,0x3F,0xF8,0x00,0x00,0x7F,0xFC,0x00,0x00,0xFF,0xFE,0x00,0x01,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x80,0x03,0xF8,0x3F,0x80,0x07,0xF0,0x0F,0xC0,0x07,0xE0,0x07,0xC0,0x0F,0xC0,0x07,0xE0,0x0F,0xC0,0x03,0xE0,0x0F,0xC0,0x03,0xE0,0x0F,0x80,0x03,0xF0,0x0F,0x80,0x01,0xF0,0x1F,0x80,0x01,0xF0,0x1F,0x80,0x01,0xF0,0x1F,0x80,0x01,0xF0,0x1F,0x80,0x01,0xF0,0x1F,0x80,0x01,0xF8,0x1F,0x80,0x01,0xF8,0x1F,0x80,0x01,0xF8,0x0F,0x80,0x01,0xF8,0x0F,0x80,0x03,0xF8,0x0F,0x80,0x03,0xF8,0x0F,0xC0,0x03,0xF8,0x0F,0xC0,0x03,0xF8,0x07,0xC0,0x07,0xF8,0x07,0xE0,0x0F,0xF8,0x07,0xF0,0x1F,0xF8,0x03,0xFE,0x7F,0xF8,0x01,0xFF,0xFE,0xF8,0x01,0xFF,0xFC,0xF8,0x00,0xFF,0xFC,0xF8,0x00,0x3F,0xF0,0xF8,0x00,0x0F,0xE1,0xF0,0x00,0x00,0x01,0xF0,0x00,0x00,0x01,0xF0,0x00,0x00,0x01,0xF0,0x00,0x00,0x01,0xF0,0x00,0x00,0x01,0xF0,0x00,0x00,0x03,0xE0,0x00,0x00,0x03,0xE0,0x00,0x00,0x07,0xE0,0x00,0x00,0x07,0xC0,0x00,0x00,0x0F,0xC0,0x03,0x00,0x1F,0x80,0x03,0xC0,0x7F,0x80,0x03,0xFF,0xFF,0x00,0x03,0xFF,0xFF,0x00,0x03,0xFF,0xFE,0x00,0x03,0xFF,0xFC,0x00,0x01,0xFF,0xF0,0x00,0x00,0x7F,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  #// 9
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xF0,0x00,0x00,0x0F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 ] #// :


def writeByte(x, y, dta):
    for i in range(7,-1,-1):
        if ((dta >> i) & 1) == True:
            display.pixel(x,y,1)
        else: 
            display.pixel(x,y,0)
        x=x+1


def disp16x16():
    x=10
    y=10
    byteCnt = 160
    for yinc in range (16):
        for xinc in range(0,2):
            writeByte( x, y + yinc, zero16x16[byteCnt] )
            byteCnt += 1
            x = x + 8
        x=0
   

def disp32x50(xstart,ystart,dtaAddr):

    y = ystart
    x = xstart
  
    for yinc in range (50):
        for xinc in range(0,4):
            writeByte( x, y + yinc, sevseg32x50[dtaAddr] )
            dtaAddr += 1
            x = x + 8
        x = xstart


def dispgrotesk32x64(xstart,ystart,dtaAddr):  
    
    y = ystart
    x = xstart
  
    for yinc in range (64):
        for xinc in range(0,4):
            writeByte( x, y + yinc, grotesk32x64[dtaAddr] )
            dtaAddr += 1
            x = x + 8
        x = xstart
   


###################################################
while(True):
    display.fill(0x0)
    display.show()
    x=0
    y=0
    xpos=32
    byteCnt = 0
    for cnt in range(11):
        for yinc in range (64):
            for xinc in range(0,4):
                writeByte( x, y + yinc, grotesk32x64[byteCnt] )
                byteCnt += 1
                x = x + 8
            x=0
        dispgrotesk32x64(xpos*1, 0, 256*cnt)
        dispgrotesk32x64(xpos*2, 0, 256*cnt)
        dispgrotesk32x64(xpos*3, 0, 256*cnt)
        display.show()
        y=0

