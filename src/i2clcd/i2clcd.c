#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <time.h>
#include "lcdchar.h"

//install i2c-tools, libi2c-dev
int i2caddr = 0x27;
unsigned char i2creg = 0;
int i2cfd;
char timeBuff[50]; 
char dateBuff[50]; 

char * GetTime() 
{
  time_t curTime; 
  struct tm * timeInfo;

  time(&curTime); 
  timeInfo = localtime(&curTime);  
  strftime(timeBuff, 50, "%r", timeInfo);  
  strftime(dateBuff, 50, "%D", timeInfo); 
  return timeBuff; 
}
 

int
main (int argc, char **argv)
{
  int r;
  char *dev = "/dev/i2c-1";

  i2cfd = open (dev, O_RDWR);
  if (i2cfd < 0)
    {
      perror ("Opening i2c device node.\n");
      return -1;
    }
  r = ioctl (i2cfd, I2C_SLAVE, i2caddr);
  if (r < 0)
    perror ("Selecting i2c device.\n");

  Setup4bit ();

//  WriteI2CByte(0x0F, 0); 

  DisplayClear ();
  GetTime(); 
  WriteString (0,0, dateBuff); 
  WriteString (1,0, GetTime()); 

//  DisplayClear ();
//  WriteString (3, 0, "Hello There");

  while(1) 
  {
  	GetTime(); 
  WriteString (0,0, dateBuff); 
  WriteString (1,0, GetTime()); 
  sleep(1); 
}

  close (i2cfd);
  return 0;
}
