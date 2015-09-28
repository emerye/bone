CC=gcc
CFLAGS = -Wall -g
OBJECTS = spictl.o lcdchar.o 
FILES = spictl.c lcdchar.c 

spictl: $(OBJECTS) 
	$(CC) $(CFLAGS) -o spictl $(FILES) 

spictl.o: spictl.c spictl.h 
	$(CC) $(CFLAGS) -o spictl.o -c spictl.c 

lcdchar.o: lcdchar.c lcdchar.h 
	$(CC) $(CFLAGS) -o lcdchar.o -c lcdchar.c 

clean:
	 rm -f spictl spictl.o lcdchar.o   
