#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <time.h>

#define LM75ADDRESS 0x4C

int initi2c (int);

int i2cfd;
char *dev = "/dev/i2c-2";

double
cvt2comp (int numtocvt)
{

  int inverse;
  int cvtNum;
  double degC; 
  double degF; 

  inverse = ~numtocvt;
  inverse = inverse & 0x1FF;
  cvtNum = (inverse + 1) * -1;
  degC = (double) cvtNum/2.0; 
  degF = degC * 1.8 + 32; 
  printf("Deg C %.1f  Deg F %.1f\n", degC, degF); 
  return degF; 
  
}

int
readTemperature (int numBytes)
{

  unsigned char data[35];
  int retBlock;
  double degC;
  int dataLSB, degCint;
  int deginner, numCvt;
  int adcCount;

  retBlock = i2c_smbus_read_i2c_block_data (i2cfd, 0, numBytes, &data[0]);
  dataLSB = data[1] & 0x80;
  if (dataLSB == 0x80)
    {
      dataLSB = 1;
    }
  else
    {
      dataLSB = 0;
    }
  if ((data[0] & 0x80) == 0x80)
    {
    adcCount = data[0] << 1;
    adcCount = adcCount + dataLSB; 
    degC = cvt2comp(adcCount);    
    }
  else
    {
      degC = (((data[0] & 0x7F) << 1) | dataLSB) / 2.0;
    }

  printf ("DegC: %.1f  DegF: %.1f\n", degC, degC * 1.8 +32);

  return 0;
}


//Interval Timer Handler
void
timer_handler (int signum)
{
  static int count = 0;
  int cumcount = 0;
  unsigned char buf[100];
  FILE *fp;

  if ((fp = fopen ("./cumcount.txt", "w+")) == NULL)
    {
      puts ("Error opening file cumcount.txt\n");
      return;
    }

  int retval = fread ((void *) &cumcount, 4, 1, fp);
  if (retval < 0)
    {
      printf ("read error %d\n", errno);
    }
  cumcount++;
  fseek (fp, 0, SEEK_SET);

  fwrite (&cumcount, 4, 1, fp);
  fclose (fp);

  printf ("Elasped: %d Cumcount: %d\n", ++count, cumcount);

  readTemperature (2);

}


int
main ()
{
  int status;
  struct sigaction sa;
  struct itimerval timer;

  status = initi2c (LM75ADDRESS);
  if (status < 0)
    {
      printf ("Error opening i2c device at address %d\n", LM75ADDRESS);
    }


  /* Install timer_handler as the signal handler for SIGVTALRM. */
  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = &timer_handler;
  sigaction (SIGVTALRM, &sa, NULL);

  /* Configure the timer to expire after 250 msec... */
  timer.it_value.tv_sec = 1;
  timer.it_value.tv_usec = 0;
  /* ... and every 250 msec after that. */
  timer.it_interval.tv_sec = 1;
  timer.it_interval.tv_usec = 0;
  /* Start a virtual timer. It counts down whenever this process is
     executing. */
  setitimer (ITIMER_VIRTUAL, &timer, NULL);

  /* Do busy work. */
  while (1)
    {

    }

  close (i2cfd);
}

int
initi2c (int tgtAddress)
{
  int r;

  i2cfd = open (dev, O_RDWR);
  if (i2cfd < 0)
    {
      perror ("Opening i2c device node.\n");
      return -1;
    }
  r = ioctl (i2cfd, I2C_SLAVE, tgtAddress);
  if (r < 0)
    {
      perror ("Selecting i2c device.\n");
      return -1;
    }

  return 0;
}
