"""SSD1309 demo (fonts)."""
from time import sleep
from machine import Pin, SPI
from xglcd_font import XglcdFont
from ssd1309 import Display


def test():
    """Test code."""
    spi = SPI(1, baudrate=10000000, sck=Pin(14), mosi=Pin(13))
    display = Display(spi, dc=Pin(4), cs=Pin(15), rst=Pin(5))

    print("Loading fonts.  Please wait.")
    bally = XglcdFont('fonts/Bally7x9.c', 7, 9)
    rototron = XglcdFont('fonts/Robotron13x21.c', 13, 21)
    #unispace = XglcdFont('fonts/Unispace12x24.c', 12, 24)
    #wendy = XglcdFont('fonts/Wendy7x8.c', 7, 8)
    perfect = XglcdFont('fonts/PerfectPixel_23x32.c', 23, 32)

    print("Drawing fonts.")

    #text_height = bally.height
    #display.draw_text(display.width, display.height // 2 - text_height // 2, "Bally", bally, rotate=180)

    text_width = rototron.measure_text("ROTOTRON")
    display.draw_text(display.width // 2 - text_width // 2, 0, "ROTOTRON", rototron)

    #text_width = unispace.measure_text("Unispace")
    #text_height = unispace.height
    #display.draw_text(display.width // 2 - text_width // 2, display.height - text_height, "Unispace", unispace, invert=True)

    text_width = perfect.measure_text("1234")
    text_height = perfect.height
    display.draw_text(display.width // 2 - text_width // 2, display.height - text_height, "1234", perfect, invert=False)
        
    display.present()

    sleep(10)
    display.cleanup()
    print('Done.')


test()
