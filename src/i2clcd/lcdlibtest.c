#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include "lcdchar.h"

//install packages i2c-tools, libi2c-dev

int i2caddr = 0x27;
unsigned char i2creg = 0;
int i2cfd;
char timeBuff[50];
char dateBuff[50];

char *GetTime()
{
    time_t curTime;
    struct tm *timeInfo;

    time(&curTime);
    timeInfo = localtime(&curTime);
    strftime(timeBuff, 50, "%r", timeInfo);
    strftime(dateBuff, 50, "%D", timeInfo);
    return timeBuff;
}


void Display(int i2cfd, unsigned char tgtAddress)
{
    int r;

    r = ioctl(i2cfd, I2C_SLAVE, tgtAddress);
    if (r < 0) {
	perror("Selecting i2c device.\n");
    }
    GetTime();
    WriteString(i2cfd, 0, 0, dateBuff);
    WriteString(i2cfd, 1, 0, GetTime());
}
int
main()
{
  int r;
  int i2cfd; 
  char *dev = "/dev/i2c-2";

  i2cfd = open (dev, O_RDWR);
  if (i2cfd < 0)
    {
      perror ("Opening i2c device node.\n");
      return -1;
    }
    r = ioctl(i2cfd, I2C_SLAVE, i2caddr);
    if (r < 0)
	perror("Selecting i2c device.\n");

    Setup4bit(i2cfd);
    DisplayClear(i2cfd);
    Display(i2cfd, i2caddr);

    r = ioctl(i2cfd, I2C_SLAVE, i2caddr);
    if (r < 0)
	perror("Selecting i2c device.\n");
    Setup4bit(i2cfd);

//  WriteI2CByte(0x0F, 0); 

    DisplayClear(i2cfd);
    GetTime();
    WriteString(i2cfd, 0, 0, dateBuff);
    WriteString(i2cfd, 1, 0, GetTime());

//  DisplayClear ();
    WriteString(i2cfd, 2, 0, "December 18");
    WriteString(i2cfd, 3, 0, "One week off.");

    while (1) {
	GetTime();
	WriteString(i2cfd, 0, 0, dateBuff);
	WriteString(i2cfd, 1, 0, GetTime());
	sleep(1);
    }

    close(i2cfd);
    return 0;
}
