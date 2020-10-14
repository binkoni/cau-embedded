CC = aarch64-linux-gnu-gcc

CFLAGS = 
LDFLAGS = -static

default: ledtest

ledtest: ledtest.o
	$(CC) $(LDFLAGS) -o ledtest ledtest.o
ledtest.o: ledtest.c
	$(CC) $(CFLAGS) -o ledtest.o -c ledtest.c
clean:
	rm ledtest.o ledtest
