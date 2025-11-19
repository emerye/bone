CC=gcc
CFLAGS=-Wall
OBJECTS = i2clcd.o liblcdchar.o

main:  liblcdchar.o 
	$(CC) $(CFLAGS) -fPIC -c liblcdchar.c -o liblcdchar.o -li2c    
	$(CC) $(CFLAGS) -shared -o liblcdchar.so liblcdchar.o -li2c 
	sudo mv liblcdchar.so /usr/local/lib
	sudo cp -f lcdchar.h /usr/local/include
	ldconfig -v -n /usr/local/lib/
	$(CC) $(CFLAGS) -L/usr/local/lib -L/usr/lib/arm-linux-gnueabihf i2clcd.c -li2c -llcdchar -o i2clcd  

lcdchar.o: lcdchar.h lcdchar.c
#	$(CC) $(CFLAGS) -o lcdchar.o -c lcdchar.c 

clean:
	 rm -f i2clcd i2clcd.o lcdchar.o liblcdchar.so 
