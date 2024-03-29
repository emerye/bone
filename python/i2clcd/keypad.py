# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

import time
import digitalio
import board
import adafruit_matrixkeypad
import i2clcd

i2clcd.initLCD()
i2clcd.WriteString(0, 0, 'Started')

print(i2clcd.address)

# Membrane 3x4 matrix keypad on Raspberry Pi -
# https://www.adafruit.com/product/419
cols = [digitalio.DigitalInOut(x) for x in (board.D26, board.D20, board.D21)]
rows = [digitalio.DigitalInOut(x) for x in (board.D5, board.D6, board.D13, board.D19)]

# 3x4 matrix keypad on Raspberry Pi -
# rows and columns are mixed up for https://www.adafruit.com/product/3845
# cols = [digitalio.DigitalInOut(x) for x in (board.D13, board.D5, board.D26)]
# rows = [digitalio.DigitalInOut(x) for x in (board.D6, board.D21, board.D20, board.D19)]

keys = ((1, 2, 3), (4, 5, 6), (7, 8, 9), ("*", 0, "#"))

keypad = adafruit_matrixkeypad.Matrix_Keypad(rows, cols, keys)

while True:
    keys = keypad.pressed_keys
    if keys:
        validkey = keys
        while keypad.pressed_keys == validkey:
            time.sleep(0.01)

        listkey = str(keys).replace('[','').replace(']','').replace('\'','')
        i2clcd.WriteString(0,20,listkey)
        print(listkey, keys)
        if listkey == '*':
            print("* Pressed")
            
            i2clcd.WriteString(0,0,' ')

    time.sleep(0.05)
