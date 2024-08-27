from rpi_lcd import LCD
from time import sleep

lcdobj= LCD()

lcdobj.text('Hello World!', 1)
lcdobj.text('Raspberry Pi', 2)
lcdobj.text('is really', 3, 'center')
lcdobj.text('awesome', 4, 'right')

sleep(5)
lcdobj.clear()
sleep(2)


del lcdobj