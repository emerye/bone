# Rui Santos & Sara Santos - Random Nerd Tutorials
# Complete project details at https://RandomNerdTutorials.com/micropython-i2c-lcd-esp32-esp8266/

from machine import Pin, SoftI2C
from machine_i2c_lcd import I2cLcd
from time import sleep

# Define the LCD I2C address and dimensions
I2C_ADDR = 0x27
I2C_NUM_ROWS = 4

I2C_NUM_COLS = 20

# Initialize I2C and LCD objects
i2c = SoftI2C(sda=Pin(21), scl=Pin(22), freq=400000)

lcd = I2cLcd(i2c, I2C_ADDR, I2C_NUM_ROWS, I2C_NUM_COLS)
print("Started")
lcd.putstr("It's working :)")
sleep(4)

try:
    while True:
        # Clear the LCD
        lcd.clear()
        # Display two different messages on different lines
        # By default, it will start at (0,0) if the display is empty
        lcd.putstr("Hello World!")
        sleep(1)
        lcd.clear()
        # Starting at the second line (0, 1)
        lcd.move_to(0, 1)
        lcd.putstr("Hello World!")
        sleep(1)
        lcd.move_to(0,2)
        lcd.putstr("Line 2") 
        sleep(1)
        lcd.move_to(0,3)
        lcd.putstr("Line 3")
        sleep(1)
        lcd.move_to(0,0)
        lcd.putstr("Line 0" )
        sleep(1)

except KeyboardInterrupt:
    try:
        sys.exit(0)
    except SystemExit:
        os._exit(0)
