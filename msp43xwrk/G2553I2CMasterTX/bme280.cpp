/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "bme280.h"

#define SEALEVELPRESSURE_HPA (1013.25)

extern void I2CWriteBlock(unsigned char , unsigned char *, int);
extern void I2CReadBlock(unsigned char tgtAddress, unsigned char reg, unsigned char *dataRead, int numBytestoRead);

/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/
Adafruit_BME280::Adafruit_BME280() {
}


void Adafruit_BME280::setI2CHandle(int handle) {
     i2cHandle = handle;
}


bool Adafruit_BME280::begin(uint8_t a) {
  _i2caddr = a;


  if (read8(BME280_REGISTER_CHIPID) != 0x60)
    return false;

  readCoefficients();

  //Set before CONTROL_meas (DS 5.4.3)
  write8(BME280_REGISTER_CONTROLHUMID, 0x05); //16x oversampling 

  write8(BME280_REGISTER_CONTROL, 0xB7); // 16x ovesampling, normal mode
  return true;
}


/**************************************************************************/
/*!
    @brief  Writes an 8 bit value over I2C
*/
/**************************************************************************/
void Adafruit_BME280::write8(unsigned char reg, unsigned char value)
{
   int retVal; 
   unsigned char buffer[2];

   buffer[0] = value;

     I2CWriteBlock(BME280_ADDRESS, buffer, 2);
}


/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
uint8_t Adafruit_BME280::read8(unsigned char reg)
{
  uint8_t value;
  unsigned char buffer[2];

  I2CReadBlock(BME280_ADDRESS, reg, buffer, 1);

  return (uint8_t) buffer[0];
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit value over I2C
    Return the data big endian
*/
/**************************************************************************/
uint16_t Adafruit_BME280::read16(unsigned char reg)
{
  uint16_t value;

 // value = wiringPiI2CReadReg16(i2cHandle, reg);
          if (value <= -1) {
              perror("Error reading 16 bit value.");
          }

  //Return the data big endian 
  return ( (value << 8) | ((value >> 8) & 0xFF) ); 
}


uint16_t Adafruit_BME280::read16_LE(unsigned char reg) {
  uint16_t temp = read16(reg);
  return (temp >> 8) | (temp << 8);

}

/**************************************************************************/
/*!
    @brief  Reads a signed 16 bit value over I2C
*/
/**************************************************************************/
int16_t Adafruit_BME280::readS16(unsigned char reg)
{
  return (int16_t)read16(reg);

}

int16_t Adafruit_BME280::readS16_LE(unsigned char reg)
{
  return (int16_t)read16_LE(reg);

}


/**************************************************************************/
/*!
    @brief  Reads a 24 bit value over I2C
*/
/**************************************************************************/

uint32_t Adafruit_BME280::read24(unsigned char reg)
{
  uint32_t value;
  int32_t lowByte;  
  uint32_t highBytes; 

  highBytes = read16(reg); 
  value = (highBytes << 8);  
//  lowByte = wiringPiI2CRead(i2cHandle);
  if (lowByte < 0) {
          if (value < 0) {
               perror("Error reading last byte of  24 bit value.");
           }
   }
   value += lowByte; 
   

  return value;
}


/**************************************************************************/
/*!
    @brief  Reads the factory-set coefficients
*/
/**************************************************************************/
void Adafruit_BME280::readCoefficients(void)
{
    _bme280_calib.dig_T1 = read16_LE(BME280_REGISTER_DIG_T1);
    _bme280_calib.dig_T2 = readS16_LE(BME280_REGISTER_DIG_T2);
    _bme280_calib.dig_T3 = readS16_LE(BME280_REGISTER_DIG_T3);

    _bme280_calib.dig_P1 = read16_LE(BME280_REGISTER_DIG_P1);
    _bme280_calib.dig_P2 = readS16_LE(BME280_REGISTER_DIG_P2);
    _bme280_calib.dig_P3 = readS16_LE(BME280_REGISTER_DIG_P3);
    _bme280_calib.dig_P4 = readS16_LE(BME280_REGISTER_DIG_P4);
    _bme280_calib.dig_P5 = readS16_LE(BME280_REGISTER_DIG_P5);
    _bme280_calib.dig_P6 = readS16_LE(BME280_REGISTER_DIG_P6);
    _bme280_calib.dig_P7 = readS16_LE(BME280_REGISTER_DIG_P7);
    _bme280_calib.dig_P8 = readS16_LE(BME280_REGISTER_DIG_P8);
    _bme280_calib.dig_P9 = readS16_LE(BME280_REGISTER_DIG_P9);

    _bme280_calib.dig_H1 = read8(BME280_REGISTER_DIG_H1);
    _bme280_calib.dig_H2 = readS16_LE(BME280_REGISTER_DIG_H2);
    _bme280_calib.dig_H3 = read8(BME280_REGISTER_DIG_H3);
    _bme280_calib.dig_H4 = (read8(BME280_REGISTER_DIG_H4) << 4) | (read8(BME280_REGISTER_DIG_H4+1) & 0xF);
    _bme280_calib.dig_H5 = (read8(BME280_REGISTER_DIG_H5+1) << 4) | (read8(BME280_REGISTER_DIG_H5) >> 4);
    _bme280_calib.dig_H6 = (int8_t)read8(BME280_REGISTER_DIG_H6);
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float Adafruit_BME280::readTemperature(void)
{
  int32_t var1, var2;

  int32_t adc_T = read24(BME280_REGISTER_TEMPDATA);
  adc_T >>= 4;

  var1  = ((((adc_T>>3) - ((int32_t)_bme280_calib.dig_T1 <<1))) *
	   ((int32_t)_bme280_calib.dig_T2)) >> 11;

  var2  = (((((adc_T>>4) - ((int32_t)_bme280_calib.dig_T1)) *
	     ((adc_T>>4) - ((int32_t)_bme280_calib.dig_T1))) >> 12) *
	   ((int32_t)_bme280_calib.dig_T3)) >> 14;

  t_fine = var1 + var2;

  float T  = (t_fine * 5 + 128) >> 8;
  return T/100;
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
float Adafruit_BME280::readPressure(void) {
  int64_t var1, var2, p;

  readTemperature(); // must be done first to get t_fine

  int32_t adc_P = read24(BME280_REGISTER_PRESSUREDATA);
  adc_P >>= 4;

  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)_bme280_calib.dig_P6;
  var2 = var2 + ((var1*(int64_t)_bme280_calib.dig_P5)<<17);
  var2 = var2 + (((int64_t)_bme280_calib.dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)_bme280_calib.dig_P3)>>8) +
    ((var1 * (int64_t)_bme280_calib.dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)_bme280_calib.dig_P1)>>33;

  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = 1048576 - adc_P;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)_bme280_calib.dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)_bme280_calib.dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)_bme280_calib.dig_P7)<<4);
  return (float)p/256;
}


/**************************************************************************/
/*!

*/
/**************************************************************************/
float Adafruit_BME280::readHumidity(void) {

  readTemperature(); // must be done first to get t_fine

  int32_t adc_H = read16(BME280_REGISTER_HUMIDDATA);

  int32_t v_x1_u32r;

  v_x1_u32r = (t_fine - ((int32_t)76800));

  v_x1_u32r = (((((adc_H << 14) - (((int32_t)_bme280_calib.dig_H4) << 20) -
		  (((int32_t)_bme280_calib.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
	       (((((((v_x1_u32r * ((int32_t)_bme280_calib.dig_H6)) >> 10) *
		    (((v_x1_u32r * ((int32_t)_bme280_calib.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
		  ((int32_t)2097152)) * ((int32_t)_bme280_calib.dig_H2) + 8192) >> 14));

  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
			     ((int32_t)_bme280_calib.dig_H1)) >> 4));

  v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
  v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
  float h = (v_x1_u32r>>12);
  return  h / 1024.0;
}

/**************************************************************************/
/*!
    Calculates the altitude (in meters) from the specified atmospheric
    pressure (in hPa), and sea-level pressure (in hPa).

    @param  seaLevel      Sea-level pressure in hPa
    @param  atmospheric   Atmospheric pressure in hPa
*/
/**************************************************************************/
float Adafruit_BME280::readAltitude(float seaLevel)
{
  // Equation taken from BMP180 datasheet (page 16):
  //  http://www.adafruit.com/datasheets/BST-BMP180-DS000-09.pdf

  // Note that using the equation from wikipedia can give bad results
  // at high altitude.  See this thread for more information:
  //  http://forums.adafruit.com/viewtopic.php?f=22&t=58064

  float atmospheric = readPressure() / 100.0f;
  return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
}


/*
int main(int args, char *argv[]) {

  int retVal; 
  int i2cHandle; 
  float temperature, pressure, humidity, altitude; 
  Adafruit_BME280 sensor;  
  int value; 

  printf("Start\n"); 


if (sensor.read8(BME280_REGISTER_CHIPID) != 0x60) {
    printf("Reading chip ID failed.\n"); 
    return false;
   }

   value = sensor.read16(0x88); 
   printf("Read 16:  %04x\n", value); 
  
   value = sensor.read16_LE(0x88); 
   printf("Read 16LE:  %04x\n", value); 

   value = sensor.read24(0x88); 
   printf("Read 24:  %06x\n", value); 

 //  return 0; 

  sensor.readCoefficients();

  //Set before CONTROL_meas (DS 5.4.3)
  sensor.write8(BME280_REGISTER_CONTROLHUMID, 0x05); //16x oversampling 

  sensor.write8(BME280_REGISTER_CONTROL, 0xB7); // 16x ovesampling, normal mode
  
  temperature = sensor.readTemperature(); 
  printf("Temperature is %f\n", temperature); 
  
  pressure = sensor.readPressure() / 100.0F;  
  printf("Pressure in hPa is %f\n", pressure); 

  printf("Pressure in inches of mercury is %f\n", pressure/33.8638866667); 
  
  altitude = sensor.readAltitude(SEALEVELPRESSURE_HPA); 
  printf("Altitude is %f meters\n", altitude); 

  humidity = sensor.readHumidity(); 
  printf("Humidity is %f \n", humidity); 

  return true;
}
*/
