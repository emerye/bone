from machine import ADC, Pin
import machine
import lcd
import time

def adcReading(slope, offset, adcCount):
    return ((offset + adcCount) / slope)

def adc12bitRead(samples):
    accum = 0
    for i in range(0,samples):
        adcRead = adc.read()
        accum = accum + adcRead
        time.sleep(0.005)
    adcCnt = accum/samples
    return(adcReading(4287.71384, 275.16018, adcCnt))
         

lcdobj = lcd.CharLCD(0x27)
inputPin = Pin(4, Pin.IN, Pin.PULL_UP)

adc = ADC(Pin(32))          # create ADC object on ADC pin
adc.atten(ADC.ATTN_0DB)    # set 0dB input attenuation (voltage range roughly 0.0v - 1.0v)
adc.width(ADC.WIDTH_12BIT)   # set 9 bit return values (returned range 0-511)
time.sleep(0.5)
allCounts = 0
data = []
for i in range(0, 100):
    adcValue = adc.read()         
    data.append(adcValue)
    allCounts = allCounts + adcValue 
    time.sleep(0.005)

buf = "Ave:%d L:%d" % (allCounts/100,adcValue)
lcdobj.WriteString(0, 20, buf)
buf = "Max:%d Min:%d" % (max(data), min(data))
lcdobj.WriteString(1, 20, buf)

cnt = 0
while inputPin.value() == 1: 
    adcValue = adc12bitRead(100)
    reading = '%.4fV' % adcValue
    lcdobj.WriteString(0,0,reading)
    cnt = cnt + 1
