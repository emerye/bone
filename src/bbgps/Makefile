CC=gcc
CFLAGS=-Wall -g 
OBJECTS = serial.o itimer.o  
INCLUDE=-I./include

all:   bbgps

test:  sertest
 
sertest:  sertest.o  
	$(CC) $(CFLAGS) sertest.o -o sertest  

bbgps:  $(OBJECTS)  
	$(CC) $(CFLAGS) $(OBJECTS) -L/usr/local/lib -llcdchar -lrt -lnmea -o bbgps  

sertest.o: sertest.c 
	$(CC) $(CFLAGS) -c sertest.c 

i2cdisp.o: i2cdisp.c i2cdisp.h 
	$(CC) $(CFLAGS) -c i2cdisp.c 

main.o:	main.c
	$(CC) $(CFLAGS) $(INCLUDE) -c main.c 

serial.o: serial.c
	$(CC) $(CFLAGS) $(INCLUDE)  -c serial.c 


itimer.o: itimer.c itimer.h  
	$(CC) $(CFLAGS) $(INCLUDE) -c itimer.c 

nmea.o: nmea.c  
	$(CC) $(CFLAGS) $(INCLUDE) -c nmea.c 

clean:
	 $(RM) bbgps $(OBJECTS) sertest.o sertest *.o  
