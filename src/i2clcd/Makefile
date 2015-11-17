CC=gcc
CFLAGS=-Wall -Werror
OBJECTS = i2clcd.o lcdchar.o

main:  liblcdchar.o 
	$(CC) $(CFLAGS) -fPIC -c liblcdchar.c -o liblcdchar.o  
	$(CC) $(CFLAGS) -shared -o liblcdchar.so liblcdchar.o 
	mv liblcdchar.so /usr/local/lib
	cp -f lcdchar.h /usr/local/include
	ldconfig -v -n /usr/local/lib
	$(CC) $(CFLAGS) -L/usr/local/lib lcdlibtest.c -llcdchar -o libtest  

lcdchar.o: lcdchar.h lcdchar.c
#	$(CC) $(CFLAGS) -o lcdchar.o -c lcdchar.c 

clean:
	 rm -f i2clcd i2clcd.o lcdchar.o liblcdchar.so 
