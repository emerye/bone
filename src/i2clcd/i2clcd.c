#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

//install i2c-tools, libi2c-dev
int i2caddr = 0x27; 
unsigned char i2creg = 0; 


int
main (int argc, char **argv)
{
  int r, i;
  int fd;

  char *dev = "/dev/i2c-1";

  fd = open (dev, O_RDWR);
  if (fd < 0)
    {
      perror ("Opening i2c device node.\n");
      return -1;
    }

  r = ioctl (fd, I2C_SLAVE, i2caddr);
  if (r < 0)
    perror ("Selecting i2c device.\n");

  for (i=0; i < 100; i++) {
  i2creg |= 0x08; 
  r = i2c_smbus_write_byte(fd, i2creg);
  if (r < 0)
    {
      perror ("Error writing to config register.\n");
    }
  usleep(100000); 
    
  i2creg &= ~0x08; 
  r = i2c_smbus_write_byte(fd, i2creg);
  if (r < 0)
    {
      perror ("Error writing to config register.\n");
    }
  usleep(100000); 
 }

  close (fd);
  return 0;
}
