#ifndef _SPI595_H
#define _SPI595_H


int transfer (int fd, unsigned char send[], unsigned char receive[], int length); 

int SendWord (int data);
int Init_SPI (void); 
int SPISend (unsigned int fd, int data);  

extern int spiFD; 

#endif
