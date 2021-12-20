from machine import ADC, Pin
import lcd
import time

adc9bitslope = 538.94
adc9bitslope = 36.748

#9bit ADC slope is 538.94 offset is 36.748 
# ADCCnt = 538.94V -36.748
# V = i(36.748 + ADCCnt)/538.94
def adcReading(slope, offset, adcCount):
    return ((offset + adcCount) / slope)

def adc12bitRead(samples):
    accum = 0
    for i in range(1,samples):
        adcRead = adc.read()
        accum = accum + adcRead
    adcCnt = accum/samples
    return(adcReading(4251.8892, 274.8816, adcCnt))
         

lcdobj = lcd.CharLCD(0x27)

adc = ADC(Pin(32))          # create ADC object on ADC pin
adc.atten(ADC.ATTN_0DB)    # set 0dB input attenuation (voltage range roughly 0.0v - 1.0v)
adc.width(ADC.WIDTH_12BIT)   # set 9 bit return values (returned range 0-511)
time.sleep(0.5)
allCounts = 0
for i in range(0, 99):
    adcValue = adc.read()         
    allCounts = allCounts + adcValue 

buf = "Ave:%d L:%d" % (allCounts/100,adcValue)
lcdobj.WriteString(1,0, buf)
adcValue = adc12bitRead(100)
reading = '%.4fV Sam:%d' % (adcValue, 100)
lcdobj.WriteString(0,0,reading)
