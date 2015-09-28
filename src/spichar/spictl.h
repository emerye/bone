#ifndef SPICTL_H_
#define SPICTL_H_

extern unsigned int SPIfd; 

int InitSPI(void); 
int SPIWrite (int fd, unsigned char send[], unsigned char receive[], int length); 
int WriteSPIByte(unsigned char); 


#endif /* SPICTL_H_ */
