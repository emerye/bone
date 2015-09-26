CC=gcc
CFLAGS = -Wall -g
OBJECTS = spictl.o  
FILES = spictl.c 

spictl: $(OBJECTS) 
	$(CC) $(CFLAGS) -o spictl $(FILES) 

spictl.o: spictl.c 
	$(CC) $(CFLAGS) -o spictl.o -c spictl.c 

clean:
	 rm -f spictl spictl.o  
