CC=gcc
CFLAGS=-Wall -Werror 
OBJECTS = gpio.o  

gpio: gpio.o
	$(CC) $(CFLAGS) -o gpio -lwiringPi gpio.o   

gpio.o: gpio.c  
	$(CC) $(CFLAGS) -c gpio.c   

clean:
	 $(RM) gpio *.o 
