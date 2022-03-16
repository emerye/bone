import machine
import utime

RefVoltage = 3.231

sensor_temp = machine.ADC(4)
conversion_factor = RefVoltage / (65535)

while True:
    reading = sensor_temp.read_u16() * conversion_factor
    temperatureC = 27 - (reading - 0.706)/0.001721
    temperatureF = (temperatureC * 1.8) + 32
    print(temperatureF)
    utime.sleep(2)