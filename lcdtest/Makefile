CC = aarch64-linux-gnu-gcc

CFLAGS = 
LDFLAGS = -static

default: lcdtest

lcdtest: lcdtest.o
	$(CC) $(LDFLAGS) -o lcdtest lcdtest.o
lcdtest.o: lcdtest.c
	$(CC) $(CFLAGS) -o lcdtest.o -c lcdtest.c
clean:
	rm lcdtest.o lcdtest
