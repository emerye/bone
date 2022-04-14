"""SSD1309 demo (fonts)."""
import sys
from time import sleep
from machine import Pin, SPI
sleep(2)
#from xglcd_font import XglcdFont
from ssd1309 import Display
from xglcd_font import XglcdFont


def test():
    """Test code."""
    spi = SPI(0, baudrate=10000000, sck=Pin(18), mosi=Pin(19))
    display = Display(spi, dc=Pin(17), cs=Pin(16), rst=Pin(20), width=132)

    print("Loading fonts.  Please wait.")
    bally = XglcdFont('fonts/Bally7x9.c', 7, 9)
    #unispace = XglcdFont('fonts/Unispace12x24.c', 12, 24)
    perfect = XglcdFont('fonts/PerfectPixel_23x32.c', 23, 32)
    # arial = XglcdFont('fonts/arial33x47per.c', 33, 47 ,start_letter=46,letter_count=12)

    print("Drawing fonts.")
    #display.draw_text(display.width, display.height // 2 - text_height // 2, "Bally", bally, rotate=180)

   # text_width = rototron.measure_text("ROTOTRON")
   # display.draw_text(display.width // 2 - text_width // 2, 0, "ROTOTRON", rototron)

    #text_width = unispace.measure_text("Unispace")
    #text_height = unispace.height
    #display.draw_text(display.width // 2 - text_width // 2, display.height - text_height, "Unispace", unispace, invert=True)

  #  text_width = perfect.measure_text("1234")
  #  text_height = perfect.height
  #  display.draw_text(display.width // 2 - text_width // 2, display.height - text_height, "1234", perfect, invert=False)

   # text_width = arial.measure_text("0.9")
  #  text_height = arial.height
   # display.draw_text(display.width // 2 - text_width // 2, display.height - text_height, "0.9", arial, invert=False)


    display.clear()
    temperature = 9.3
    for i in range(1):
        text = '1234'
        text_width = perfect.measure_text(text)
        text_height = perfect.height
        print("text_width", text_width, "text_height", text_height)
        display.draw_text(display.width // 2 - text_width // 2, display.height - text_height, text, perfect, spacing=3 )

        text = 'This is line 1'
        text_width = bally.measure_text(text)
        text_height = bally.height
        #display.draw_text(display.width // 2 - text_width // 2, 20,text,bally)
        display.draw_text(0, bally.height,text,bally)
        print("Bally text_width", text_width, "text_height", text_height)
        display.present()
        sleep(5)
        dtemp = '{0:.1f}'.format(temperature) 
        #text_width = perfect.measure_text(dtemp)
        #text_height = perfect.height
        #display.draw_text(display.width // 2 - text_width // 2, display.height - text_height, dtemp , perfect, invert=False)
        #display.present()
        #sleep(1)
        #display.clear()
        #temperature = temperature + 4.1
        #print(temperature)
        
    display.cleanup()
    print('Done.')


test()
sleep(2)
sys.exit()
