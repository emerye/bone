#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <inttypes.h>

#define TGTADDR 0x49
#define REG12 0x60
#define CFGREG 0x1


class TMP100 {

  private:
    int tgtAddress;
    int i2cHandle;
    int decimalTemp;
    int integerTemp;
    double fraction;
    double tempReadingF;
    double tempReadingC;


  public: TMP100(int tgtAddress) {
	this->tgtAddress = tgtAddress;
  }
  public:int readTemperature() {
	int retVal;

	retVal = read2Bytes(0);
	if (retVal == -1) {
	    perror("Error reading Temperature from device.");
	    return retVal;
	}
	integerTemp = retVal & 0xFF;
	decimalTemp = (retVal & 0xFF00) >> 8;
	fraction = ((decimalTemp & 0xF0) >> 4) * 0.0625;
	tempReadingC = integerTemp + fraction;
	tempReadingF = (tempReadingC * (9.0 / 5.0)) + 32;
    }


  public:int read2Bytes(int reg) {
	int retVal;
	retVal = wiringPiI2CReadReg16(i2cHandle, 0);
	if (retVal == -1) {
	    perror("Reading I2C device");
	}
	return retVal;
    }


  public:int I2CInit() {
	int retVal;
	int data;

	if ((retVal = wiringPiI2CSetup(tgtAddress)) == -1) {
	    perror("Error opening I2C system");
	} else {
	    i2cHandle = retVal;
	}

	retVal = wiringPiI2CWrite(i2cHandle, CFGREG);
	if (retVal == -1) {
	    perror("Writing I2C device configuration");
	}

	retVal = wiringPiI2CWriteReg8(i2cHandle, 1, REG12);
	if (retVal == -1) {
	    perror("Error Writing value to cfg register.");
	}

	retVal = wiringPiI2CReadReg16(i2cHandle, 0);
	if (retVal == -1) {
	    perror("Reading I2C device");
	}
    }

  public:double getTemperatureF() {
	return (tempReadingF);
    }
};


int main(int argc, char **argv)
{
    int retVal;

    TMP100 sensor(TGTADDR);
    sensor.I2CInit();
    for (int i; i<10000; i++) {
    retVal = sensor.readTemperature();
    if (retVal != -1) {
	printf("Temperature in F %g\n", sensor.getTemperatureF());
    }
    sleep(1);  
    }

    return 0;
}
