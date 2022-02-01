import tm1638
from machine import Pin
tm = tm1638.TM1638(stb=Pin(16), clk=Pin(22), dio=Pin(23))

from time import sleep_ms

# display "01234567" using bytes from the tm1638._SEGMENTS font
# tm.segments([0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07])

# encode string will give you a byte array which you can pass to .segments()
# tm.encode_string('abcdefgh')

# display "abcdefgh"
tm.segments(tm.encode_string('01234567'))
sleep_ms(1000)
tm.clear()
tm.temperature(34,0)
