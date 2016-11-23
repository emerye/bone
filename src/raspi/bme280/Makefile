CC=g++
CFLAGS=-Wall -g 
OBJECTS = bme280.o    

TMP100:   $(OBJECTS)  
	$(CC) $(CFLAGS) $(OBJECTS) -lwiringPi -o bme280  

bme280.o: bme280.cpp 
	$(CC) $(CFLAGS) -c bme280.cpp -lwiringPi 

clean:
	 $(RM) bme280 $(OBJECTS) *.o  
