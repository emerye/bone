/** SPI C Transfer Example, Written by Derek Molloy (www.derekmolloy.ie)
*    for the book Exploring BeagleBone. Based on the spidev_test.c code
*    example at www.kernel.org
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<stdint.h>
#include<linux/spi/spidev.h>
#include"tft.h"

#define SPI_PATH "/dev/spidev1.0"
//#define SPI_PATH "/dev/spidev2.0"

int
transfer (int fd, unsigned char send[], unsigned char receive[], int length)
{
  struct spi_ioc_transfer transfer;	//the transfer structure
  transfer.tx_buf = (unsigned long) send;	//the buffer for sending data
  transfer.rx_buf = (unsigned long) receive;	//the buffer for receiving data
  transfer.len = length;	//the length of buffer
  transfer.speed_hz = 12000000;	//the speed in Hz
  transfer.bits_per_word = 8;	//bits per word
  transfer.delay_usecs = 0;	//delay in us

  // send the SPI message (all of the above fields, inc. buffers)
  int status = ioctl (fd, SPI_IOC_MESSAGE (1), &transfer);
  if (status < 0)
    {
      perror ("SPI: SPI_IOC_MESSAGE Failed");
      return -1;
    }
  return status;
}


int
SendWord (int data)
//main ()
{
  unsigned int fd;		//file handle and loop counter
  uint8_t bits = 8, mode = 3;	//8-bits per word, SPI mode 3
  uint32_t speed = 12000000;	//Speed is 1 MHz
  unsigned char send[20];
  unsigned char receive[20];

  // The following calls set up the SPI bus properties
  if ((fd = open (SPI_PATH, O_RDWR)) < 0)
    {
      perror ("SPI Error: Can't open device.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_WR_MODE, &mode) == -1)
    {
      perror ("SPI: Can't set SPI mode.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_RD_MODE, &mode) == -1)
    {
      perror ("SPI: Can't get SPI mode.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1)
    {
      perror ("SPI: Can't set bits per word.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_RD_BITS_PER_WORD, &bits) == -1)
    {
      perror ("SPI: Can't get bits per word.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
    {
      perror ("SPI: Can't set max speed HZ");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1)
    {
      perror ("SPI: Can't get max speed HZ.");
      return -1;
    }

  // Check that the properties have been set

  data = data & 0xFFFF;
  send[0] = (unsigned char) ((data >> 8) & 0xFF);
  send[1] = (unsigned char) (data & 0xFF);
  // This function can send and receive data, just sending here
  if (transfer (fd, (unsigned char *) &send[0], &receive[0], 2) == -1)
    {
      perror ("Failed to update the display");
      return -1;
    }
  fflush (stdout);		//need to flush the output, no \n

  close (fd);			//close the file
  return 0;
}


int
Init_SPI (void)
{
  unsigned int fd;		//file handle and loop counter
  uint8_t bits = 8, mode = 3;	//8-bits per word, SPI mode 3
  uint32_t speed = 1000000;	//Speed is 1 MHz

  // The following calls set up the SPI bus properties
  if ((fd = open (SPI_PATH, O_RDWR)) < 0)
    {
      perror ("SPI Error: Can't open device.");
      return -1;
    }
  spiFD = fd;
  if (ioctl (fd, SPI_IOC_WR_MODE, &mode) == -1)
    {
      perror ("SPI: Can't set SPI mode.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_RD_MODE, &mode) == -1)
    {
      perror ("SPI: Can't get SPI mode.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1)
    {
      perror ("SPI: Can't set bits per word.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_RD_BITS_PER_WORD, &bits) == -1)
    {
      perror ("SPI: Can't get bits per word.");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
    {
      perror ("SPI: Can't set max speed HZ");
      return -1;
    }
  if (ioctl (fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1)
    {
      perror ("SPI: Can't get max speed HZ.");
      return -1;
    }
  return 0;
}


//SPI file descriptor and 16 bit word 
int
SPISend (unsigned int fd, int data)
{
  unsigned char send[20];
  unsigned char receive[20];


  data = data & 0xFFFF;
  send[0] = (unsigned char) ((data >> 8) & 0xFF);
  send[1] = (unsigned char) (data & 0xFF);
  // This function can send and receive data, just sending here
  if (transfer (fd, (unsigned char *) &send[0], &receive[0], 2) == -1)
    {
      perror ("Failed to update the display");
      return -1;
    }
  fflush (stdout);		//need to flush the output, no \n
  return 0;
}
