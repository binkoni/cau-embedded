CC = aarch64-linux-gnu-gcc

CFLAGS = 
LDFLAGS = -static

default: dottest

dottest: dottest.o
	$(CC) $(LDFLAGS) -o dottest dottest.o
dottest.o: dottest.c
	$(CC) $(CFLAGS) -o dottest.o -c dottest.c
clean:
	rm dottest.o dottest
