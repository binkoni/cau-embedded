CC = aarch64-linux-gnu-gcc

CFLAGS = 
LDFLAGS = -static

default: pbuttontest

pbuttontest: pbuttontest.o
	$(CC) $(LDFLAGS) -o pbuttontest pbuttontest.o
pbuttontest.o: pbuttontest.c
	$(CC) $(CFLAGS) -o pbuttontest.o -c pbuttontest.c
clean:
	rm pbuttontest.o pbuttontest
