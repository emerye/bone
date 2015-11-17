CC=gcc
CFLAGS=-Wall 
OBJECTS = sonic.o ultrasonic.bin  

main:  sonic.o ultrasonic.bin   
	$(CC) ${CFLAGS} sonic.c -o sonic  -lpthread -lprussdrv -llcdchar

ultrasonic.bin: ultrasonic.p
	pasm -b ultrasonic.p

clean:
	 rm -f sonic sonic.o  
