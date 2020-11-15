/* This is both a voltmeter and a 30 Amp current meter. Hardware
Hardware is a raspberryPi, ADS1015, and ACS712-30. The 
screen is a character lcd display using an I2C backpack.
This uses SIGALRM signal with a timer.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include "ads1115.h"
#include "lcdchar.h"

int handle;
int i2caddr = 0x27;
int dispfd;
timer_t gTimerid;
int count = 0;
double wattHours = 0;

/// Take ADC reading, scale to gain, and return value.
double readADS1115()
{
	int adcReading, ch;
	unsigned char buffer[30];
	double voltage;

	usleep(2000);
	read(handle, buffer, 2);
	adcReading = ((buffer[0] << 8) | buffer[1]);
	//printf("ADC %d\n", adcReading);
	voltage = (double)adcReading * (double)0.000125; //4.096
	//voltage = (double)adcReading * (double)0.0001875; //6.144V
	//voltage = (double)adcReading * (double)0.000015625; //256mvV
	usleep(1000);
	return voltage;
}

/// Write complete ADC configuration.
int configADS1115(uint16_t adcCfg)
{
	unsigned char buffer[4];
	int status;

	buffer[0] = ADS1015_REG_POINTER_CONFIG;
	buffer[1] = adcCfg >> 8;
	buffer[2] = adcCfg & 0x00FF;

	//Write to conversion register. This is a block write.
	usleep(10000);
	status = write(handle, buffer, 3);
	if (status == -1)
	{
		printf("Error writing block to configuration register.\n");
		return status;
	}
	wiringPiI2CWrite(handle, ADS1015_REG_POINTER_CONVERT);
	usleep(20000);
	return 0;
}

int intI2CcharDisplay()
{
	int r;
	const char *dev = "/dev/i2c-1";

	dispfd = open(dev, O_RDWR);
	if (dispfd < 0)
	{
		perror("Opening i2c device node.\n");
		return -1;
	}
	r = ioctl(dispfd, I2C_SLAVE, i2caddr);
	if (r < 0)
		perror("Selecting i2c device.\n");
	Setup4bit(dispfd);
	DisplayClear(dispfd);
}

void start_timer(void)
{
	struct itimerspec value;

	value.it_value.tv_sec = 0; //waits for 1 seconds before sending timer signal
	value.it_value.tv_nsec = 500000000;

	value.it_interval.tv_sec = 0; //sends timer signal every 1 seconds
	value.it_interval.tv_nsec = 500000000;
	timer_create(CLOCK_REALTIME, NULL, &gTimerid);
	timer_settime(gTimerid, 0, &value, NULL);
}

void stop_timer(void)
{
	struct itimerspec value;

	value.it_value.tv_sec = 0;
	value.it_value.tv_nsec = 0;
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_nsec = 0;
	timer_settime(gTimerid, 0, &value, NULL);
}

void timer_callback(int sig)
{
	double power = 203.7;
	double vMeasure, vMeasure1;
	char strBuffer[256];
	double current, vIn;
	double offsetCal = 2.571;
	double VOFFSET = 0.0;
	double VSLOPE = 15.99;
	FILE *vLog;

	
	//printf("Catch timer: %d  Count: %d\n", sig, count);
	wattHours = wattHours + power / 3600; //Watt minute
										  //printf("Seconds: %d  WattHr: %f\n", count, wattHours);
	configADS1115(ADS1015_REG_CONFIG_MUX_SINGLE_0 | ADS1015_REG_CONFIG_PGA_4_096V |
				  ADS1115_REG_CONFIG_DR_32SPS | ADS1015_REG_CONFIG_MODE_CONTIN);
	vMeasure = readADS1115();
	vIn = vMeasure * VSLOPE + VOFFSET;
	sprintf(strBuffer, "Ch1: %.3f     \n", vIn);
	printf("%d  %.3fV\n", count, vIn);
	WriteString(dispfd, 0, 0, strBuffer);
	vLog = fopen("/home/andy/bone/plot/voltmeter.txt", "a");
	if (vLog == NULL)
	{
		printf("Could not open voltage log file.");
	}
	else
	{
		fprintf(vLog, "%d  %.3f\n", count, vIn);
		fclose(vLog);
	}
	count += 1;

	configADS1115(ADS1015_REG_CONFIG_MUX_SINGLE_1 | ADS1015_REG_CONFIG_PGA_4_096V |
				  ADS1115_REG_CONFIG_DR_32SPS | ADS1015_REG_CONFIG_MODE_CONTIN);
	vMeasure1 = readADS1115();
	current = (vMeasure1 - offsetCal) / 0.066;
	//	printf("Volts: %f  Current %f\n", vMeasure1, current);
	sprintf(strBuffer, "Ch2: %.3f     \n", vMeasure1);
	//	WriteString(dispfd, 1, 0, strBuffer);
	sprintf(strBuffer, "Ch2: %.3fV %.3fA  \n", vMeasure1, current);
	//	WriteString(dispfd, 2, 0, strBuffer);
	//usleep(1000);
	fflush(stdout);
}

int main(int argc, char *args[])
{
	int i, status;
	double vMeasure;
	double vMeasure1;
	char strBuffer[256];
	FILE *vLog;

	handle = wiringPiI2CSetup(ADS1015_ADDRESS);
	intI2CcharDisplay();

	status = configADS1115(ADS1015_REG_CONFIG_MUX_SINGLE_0 | ADS1015_REG_CONFIG_PGA_6_144V |
						   ADS1115_REG_CONFIG_DR_32SPS | ADS1015_REG_CONFIG_MODE_CONTIN);
	if (status == -1)
	{
		strcpy(strBuffer, "Error: I2C write\n");
		WriteString(dispfd, 4, 0, strBuffer);
	}

	vLog = fopen("/home/andy/bone/plot/voltmeter.txt", "w");
	if (vLog == NULL) {
		printf("Could not open voltage log file.");
	}
	else {
	
		fclose(vLog);
	}

	//Start timer
	(void)signal(SIGALRM, timer_callback);
	start_timer();

	while (1)
	{
		/* main function is in timer callback. */
	}
	close(handle);

	return EXIT_SUCCESS;
}
