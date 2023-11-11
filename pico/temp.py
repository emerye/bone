import machine
import utime

RefVoltage = 3.3

sensor_temp = machine.ADC(4)
conversion_factor = RefVoltage / (65535)
readings = 8

while True:
    reading = sensor_temp.read_u16() * conversion_factor
    temperatureC = 0.0
    for i in range(readings):
      temperatureC += 27 - (reading - 0.706)/0.001721
      utime.sleep_ms(2)
    
    temperatureC = temperatureC / readings
    temperatureF = ((temperatureC * 1.8) + 32)
    print(temperatureF)
    utime.sleep(1)