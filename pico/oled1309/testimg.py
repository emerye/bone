"""SSD1309 demo (images)."""
from time import sleep
from machine import Pin, SPI
from ssd1309 import Display


def test():
    """Test code. Andy Pico"""
    spi = SPI(0, baudrate=1000000, sck=Pin(18), mosi=Pin(19))
    display = Display(spi, dc=Pin(17), cs=Pin(16), rst=Pin(20), width=132)

    display.clear_buffers()
    display.present()

    display.draw_bitmap("eyes_128x42.mono", 0, display.height // 2 - 21, 128, 42)
    display.present()
    sleep(5)

    display.cleanup()
    print('Done.')

test()
