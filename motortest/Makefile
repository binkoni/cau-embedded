CC = aarch64-linux-gnu-gcc

CFLAGS = 
LDFLAGS = -static

default: motortest

motortest: motortest.o
	$(CC) $(LDFLAGS) -o motortest motortest.o
motortest.o: motortest.c
	$(CC) $(CFLAGS) -o motortest.o -c motortest.c
clean:
	rm motortest.o motortest
