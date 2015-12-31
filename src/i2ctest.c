#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

//install i2c-tools, libi2c-dev
const static char *logFile = "./temperature.log";

void
writeResulttoFile (int curTemp)
{
  FILE *fd;
  char sBuffer[128];

  if ((curTemp < 0) || (curTemp > 120))
    return;
  fd = fopen (logFile, "w+");
  if (fd < 0)
    return;
  sprintf (sBuffer, "%d\n", curTemp);
  fputs (sBuffer, fd);
  fclose (fd);
}


int
main (int argc, char **argv)
{
  int r;
  int fd;
  signed char rbyte; 
  unsigned char value[4];
  __u8 wrdata[20];
  float tempC, fTempF;
  int tempF;

  char *dev = "/dev/i2c-1";
  int addr = 0x48;		//TMP100 with A0 and A1 low

  fd = open (dev, O_RDWR);
  if (fd < 0)
    {
      perror ("Opening i2c device node.\n");
      return -1;
    }

  r = ioctl (fd, I2C_SLAVE, addr);
  if (r < 0)
    perror ("Selecting i2c device.\n");

  //TMP100 12 bit resolution
  wrdata[0] = 0x60;

  r = i2c_smbus_write_i2c_block_data (fd, 0x1, 1, wrdata);
  if (r < 0)
    {
      perror ("Error writing to config register.\n");
    }

  r = i2c_smbus_read_i2c_block_data (fd, 0x01, 1, value);
  if (r < 0) 
	perror("Error reading configuration register.\n"); 

  printf("Value is %x\n", value[0]);  

  r = i2c_smbus_write_i2c_block_data (fd, 0x0, 0, wrdata);
  if (r < 0)
    {
      perror ("Error writing config register to output temperature.\n");
    }


  while (1)
    {
      usleep (500 * 1000);
      r = i2c_smbus_read_i2c_block_data (fd, 0, 2, value);
      if (r < 0)
	{
	  perror ("reading i2c device\n");
	}
      else
	{
          tempF = (value[0] << 4) | (value[1] >> 4); 
          printf ("Direct value %x Float %f\n", tempF, tempF * 0.0625);  
	  tempC = (((value[1] >> 4) & 0xFF) * 0.0625) + (int) value[0];
	  tempF = (int) ((tempC * 9.0 / 5.0) + 32);
	  fTempF = ((tempC * 9.0 / 5.0) + 32);
          printf("Deg C %f  Deg F %d \n", tempC, tempF);
          printf("Deg F %f \n", fTempF);
          printf("RAW LSB %2x RAW MSB %2x\n", value[0], value[1]); 
          
	  writeResulttoFile (tempF);
	}
    }
  close (fd);
  return (tempF);
}
