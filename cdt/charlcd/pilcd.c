#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <string.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>

#include "pilcdchar.h"

//install packages i2c-tools, libi2c-dev

// Constants from the data sheet Command Table Instruction codes and timings
#define LCD_CLEAR_DISPLAY 	0b00000001
#define LCD_RETURN_HOME 	0b00000010
#define LCD_ENTRY_MODE_SET 	0b00000100
#define ENTRY_MODE_LEFT		0b00000010
#define ENTRY_MODE_S		0b00000001
#define LCD_DISPLAY_ON_OFF 	0b00001000
#define DISPLAY_ENTIRE		0b00000100
#define DISPLAY_CURSOR		0b00000010
#define	DISPLAY_CURSOR_POS	0b00000001
#define LCD_CURSOR_DISPLAY   	0b00010000
#define CURSOR_DISPLAY_SC	0b00001000
#define CURSOR_DISPLAY_RL	0b00000100
#define LCD_FUNCTION_SET     	0b00100000
#define LCD_CGRAM_ADDR		0b01000000
#define LCD_DDRAM_ADDR		0b10000000

#define LCD_LONG_DELAY 			1520
#define LCD_SHORT_DELAY			37
#define LCD_ROW_OFFSET_ADDR		0x40

#define LM75I2CADD		0x48

int i2caddr = 0x27;
unsigned char i2creg = 0;
int i2cfd;
char timeBuff[50];
char dateBuff[50];

//
void WriteI2CNibble(int i2cfd, unsigned char msbtoWrite, int cmd) {
	int ret;
	unsigned char bytetoWrite = BACKLED;

	if (cmd > 1)
		puts("Type is greater than 1 in function WriteI2CByte\n");

	bytetoWrite = bytetoWrite | (msbtoWrite & 0xF0) | ENABLE | cmd;
	ret = wiringPiI2CWrite(i2cfd, bytetoWrite);
	if (ret < 0)
		puts("Failed to write byte 1 in WriteNibble.\n");
	bytetoWrite &= ~ENABLE;
	ret = wiringPiI2CWrite(i2cfd, bytetoWrite);
	if (ret < 0)
		puts("Failed to write byte 2 in WriteNibble\n");

	bytetoWrite |= ENABLE;
	ret = wiringPiI2CWrite(i2cfd, bytetoWrite);
	if (ret < 0)
		puts("Failed to write byte 3 in WriteNIbble.\n");
}

//Command = 0  Data = 1
void WriteI2CByte(int i2cfd, unsigned char bytetoWrite, int cmd) {
	unsigned char lower = (bytetoWrite << 4) & 0b11110000;
	unsigned char upper = bytetoWrite & 0b11110000;

	if (cmd > 1)
		puts("Type is greater than 1 in function WriteI2CByte\n");

	WriteI2CNibble(i2cfd, upper, cmd);
	WriteI2CNibble(i2cfd, lower, cmd);
}

/**
 Setup 4 bit display
 */
void Setup4bit(int i2cfd) {
	usleep(20000);		//Wait 15msec after power on.
	WriteI2CNibble(i2cfd, 0x30, 0);	//Manual write of Wake up!(first)
	usleep(LCD_LONG_DELAY);	//Sleep for at least 5ms

	WriteI2CNibble(i2cfd, 0x30, 0);	//Toggle the E bit, sends on falling edge
	usleep(LCD_SHORT_DELAY);	//Sleep for at least 160us

	WriteI2CNibble(i2cfd, 0x30, 0);	//Manual write of Wake up! (second)
	usleep(LCD_SHORT_DELAY);

	WriteI2CNibble(i2cfd, 0x20, 0);	//Function set to 4 bit
	usleep(LCD_SHORT_DELAY);

	WriteI2CByte(i2cfd, 0x28, 0);	//Set 4-bit/2-line
	// Default Cursor, Display and Entry states set in the constructor
	usleep(100);

	WriteI2CByte(i2cfd, LCD_CLEAR_DISPLAY, 0);
	usleep(2000);
	WriteI2CByte(i2cfd, LCD_RETURN_HOME, 0);
	usleep(2000);
	WriteI2CByte(i2cfd, LCD_CURSOR_DISPLAY, 0);
	usleep(100);
	WriteI2CByte(i2cfd, LCD_DISPLAY_ON_OFF | DISPLAY_ENTIRE, 0);

	usleep(100);
	WriteI2CByte(i2cfd, LCD_ENTRY_MODE_SET | ENTRY_MODE_LEFT, 0);

	WriteI2CByte(i2cfd, LCD_RETURN_HOME, 0);
	usleep(2000);

}

//Prints a string starting at position.
void WriteString(int i2cfd, int row, int ypos, char message[]) {
	int stLength = strlen(message);
	int i, address;

	switch (row) {
	case 0:
		address = ypos;
		break;
	case 1:
		address = 0x40 + ypos;
		break;
	case 2:
		address = 20 + ypos;
		break;
	case 3:
		address = 0x54 + ypos;
		break;
	}
	address += 0x80;
	WriteI2CByte(i2cfd, (unsigned char) address, 0);
	for (i = 0; i < stLength; i++) {
		if (message[i] > 0x1f) {
			WriteI2CByte(i2cfd, message[i], 1);
		}
	}
	usleep(5);
}

/***
 * Clears the display by passing the LCD_CLEAR_DISPLAY command
 */
void DisplayClear(int i2cfd) {
	WriteI2CByte(i2cfd, LCD_CLEAR_DISPLAY, 0);
	usleep(LCD_LONG_DELAY);	//data sheets states that a usleep of 1.52ms is required
}

/***
 * Returns the cursor to the 00 address by passing the LCD_RETURN_HOME command
 */
void DisplayHome(int i2cfd) {
	WriteI2CByte(i2cfd, LCD_RETURN_HOME, 0);
	usleep(LCD_LONG_DELAY);	//data sheets states that a usleep of 1.52ms is required
}

char *GetTime() {
	time_t curTime;
	struct tm *timeInfo;

	time(&curTime);
	timeInfo = localtime(&curTime);
	strftime(timeBuff, 50, "%r", timeInfo);
	strftime(dateBuff, 50, "%D", timeInfo);
	return timeBuff;
}


float getTemperature(int fd) {
	float degC;

	int raw = wiringPiI2CReadReg16(fd, 0x00);
	raw = ((raw << 8) & 0xFF00) + (raw >> 8);
	degC = (float) ((raw / 32.0) / 8.0);
	return (float) (degC * 9.0 / 5.0 + 32);
}

void Display(int i2cfd, unsigned char tgtAddress) {
	int r;

	r = ioctl(i2cfd, I2C_SLAVE, tgtAddress);
	if (r < 0) {
		perror("Selecting i2c device.\n");
	}
	GetTime();
	WriteString(i2cfd, 0, 0, dateBuff);
	WriteString(i2cfd, 1, 0, GetTime());
}

int main(int argc, char **argv) {
	int r, gvalue;
	int i2cfd, i;
	int gpiopin = 0;
	int tempSensorHandle;
	float currentTemperature;
	char buffer[30] = { 0 };

	wiringPiSetup();
	pinMode(gpiopin, OUTPUT);

	i2cfd = wiringPiI2CSetup(0x27);
	if (i2cfd < 0) {
		perror("Opening I2C char display.\n");
		return -1;
	}

	tempSensorHandle = wiringPiI2CSetup(LM75I2CADD);
	if (tempSensorHandle < 0) {
		perror("Opening I2C Temperature Sensor");
	}

	Setup4bit(i2cfd);
	DisplayClear(i2cfd);
	Display(i2cfd, i2caddr);

	DisplayClear(i2cfd);
	GetTime();
	WriteString(i2cfd, 0, 0, dateBuff);
	WriteString(i2cfd, 1, 0, GetTime());
	WriteString(i2cfd, 2, 0, "Going to Reno");
	WriteString(i2cfd, 2, 0, "Hornet 6/10/2017");

	for (i = 0; i < 10; i++) {

//    while (1) {
		GetTime();
		WriteString(i2cfd, 0, 0, dateBuff);
		WriteString(i2cfd, 1, 0, GetTime());

		gvalue = digitalRead(gpiopin);
		if (gvalue == 0) {
			digitalWrite(gpiopin, HIGH);
		} else {
			digitalWrite(gpiopin, LOW);	// Off
		}
		currentTemperature = getTemperature(tempSensorHandle);
		sprintf(buffer, "Temperature %.1f", currentTemperature);
		WriteString(i2cfd, 2, 0, buffer);
		sleep(1);
	}

	close(tempSensorHandle);
	close(i2cfd);
	return 0;
}
