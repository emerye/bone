#ifndef TMP100_H_
#define TMP100_H_

extern unsigned int i2cfd; 

int initTMP100(int address); 
int ReadTemperature(char * tmpReading); 


#endif
