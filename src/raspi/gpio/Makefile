CC=gcc
CFLAGS=-Wall -Werror -std=c99
OBJECTS = gpio.o  

gpio: gpio.o
	$(CC) $(CFLAGS) -o gpio -lwiringPi gpio.o   

gpio.o: gpio.c  
	$(CC) $(CFLAGS) -c gpio.c   

clean:
	 $(RM) gpio *.o 
