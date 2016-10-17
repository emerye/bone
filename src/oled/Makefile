CC=gcc
CFLAGS=-Wall -g 
OBJECTS = oled.o    
INCLUDE=-I./include

all:   oled 

test:  gpiotest 
 
gpiotest:  gpiotoggle.o  
	$(CC) $(CFLAGS) gpiotoggle.c -o gpiotoggle  

oled:   $(OBJECTS)  
	$(CC) $(CFLAGS) $(OBJECTS) -lwiringPi -o oled  

oled.o: oled.c 
	$(CC) $(CFLAGS) -c oled.c -lwiringPi 

gpio.o: gpio.c 
	$(CC) $(CFLAGS) -c gpio.c 

spi.o: spi.c spi.h
	$(CC) $(CFLAGS) -c spi.c 

main.o:	main.c
	$(CC) $(CFLAGS) $(INCLUDE) -c main.c 


clean:
	 $(RM) oled  $(OBJECTS)  *.o  
