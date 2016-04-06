CC=gcc
CFLAGS=-Wall -Werror
OBJECTS = serial.o main.o 
INCLUDE=-I./include

all:   bbgps

bbgps:  $(OBJECTS)  
	$(CC) $(CFLAGS) $(OBJECTS) -L/usr/local/lib -llcdchar  -lnmea -o bbgps  

i2cdisp.o: i2cdisp.c i2cdisp.h 
	$(CC) $(CFLAGS) -c i2cdisp.c 

main.o:	main.c
	$(CC) $(CFLAGS) $(INCLUDE) -c main.c 

serial.o: serial.c
	$(CC) $(CFLAGS) -c serial.c 

nmea.o: nmea.c  
	$(CC) $(CFLAGS) $(INCLUDE) -c nmea.c 

clean:
	 $(RM) bbgps $(OBJECTS)  
