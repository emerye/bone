CC=gcc
CFLAGS=-Wall -g 
OBJECTS = gpio.o oled.o spi595.o   
INCLUDE=-I./include

all:   oled 

test:  gpiotest 
 
gpiotest:  gpiotoggle.o  
	$(CC) $(CFLAGS) gpiotoggle.c -o gpiotoggle  

oled:   $(OBJECTS)  
	$(CC) $(CFLAGS) $(OBJECTS) -o oled  

oled.o: oled.c 
	$(CC) $(CFLAGS) -c oled.c 

gpio.o: gpio.c 
	$(CC) $(CFLAGS) -c gpio.c 

main.o:	main.c
	$(CC) $(CFLAGS) $(INCLUDE) -c main.c 


clean:
	 $(RM) oled  $(OBJECTS)  *.o  
