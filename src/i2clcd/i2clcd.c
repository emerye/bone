#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "lcdchar.h"

//install i2c-tools, libi2c-dev
int i2caddr = 0x27; 
unsigned char i2creg = 0; 
int i2cfd; 

int
main (int argc, char **argv)
{
  int r, i;
  int i2cfd;

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

  for (i=0; i < 100; i++) {
  i2creg |= 0x08; 
  r = i2c_smbus_write_byte(i2cfd, i2creg);
  if (r < 0)
    {
      perror ("Error writing to config register.\n");
    }
  usleep(100000); 
    
  i2creg &= ~0x08; 
  r = i2c_smbus_write_byte(i2cfd, i2creg);
  if (r < 0)
    {
      perror ("Error writing to config register.\n");
    }
  usleep(100000); 
 }

  close (i2cfd);
  return 0;
}
