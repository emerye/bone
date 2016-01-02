#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "tmp100.h" 

unsigned int i2cfd; 

int
initTMP100(int addr)
{
  int r;
  __u8 wrdata[20];

  char *dev = "/dev/i2c-1";
//  int addr = 0x48;		//TMP100 with A0 and A1 low

  i2cfd  = open (dev, O_RDWR);
  if (i2cfd < 0)
    {
      perror ("Opening i2c device node.\n");
      return -1;
    }

  r = ioctl (i2cfd, I2C_SLAVE, addr);
  if (r < 0)
    perror ("Selecting i2c device.\n");

  //TMP100 12 bit resolution
  wrdata[0] = 0x60;
  r = i2c_smbus_write_i2c_block_data (i2cfd, 0x1, 1, wrdata);
  if (r < 0)
    {
      perror ("Error writing to config register.\n");
    }

  r = i2c_smbus_write_i2c_block_data (i2cfd, 0x0, 0, wrdata);
  if (r < 0)
    {
      perror ("Error writing config register to output temperature.\n");
    }
  return r; 
}


//Take a reading and return a string. 
int ReadTemperature(char * tmpReading) 
{
  int r; 
  float tempC;
  float tempF; 
  unsigned char value[4];

      r = i2c_smbus_read_i2c_block_data (i2cfd, 0, 2, value);
      if (r < 0)
	{
	  perror ("reading i2c device\n");
	}
      else
	{
	  tempC = (((value[1] >> 4) & 0xFF) * 0.0625) + (int) value[0];
	  tempF = (tempC * 9.0 / 5.0 + 32);
          //printf("Deg C %f  Deg F %f \n", tempC, tempF);
          sprintf(tmpReading, "%.2f",tempF); 
	}
  return (r);
}
