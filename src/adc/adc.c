#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

const char *IN0 = "/sys/bus/iio/devices/iio:device0/in_voltage0_raw";

int
main (int argc, char **argv)
{
  int i;
  FILE *fp; 
  char data[512]; 

  fp = fopen(IN0, "r");
  if (fp == NULL) 
  {
    puts("Error opening file."); 
    return 1;
  } 
  for(i=0; i<1000; i++) 
  {
    rewind(fp); 
    fgets(data, 128, fp);  
    printf("Count %s", data); 
   } 
return 0; 
}
