CC = aarch64-linux-gnu-gcc

CFLAGS = 
LDFLAGS = -static

default: 7segtest

7segtest: 7segtest.o
	$(CC) $(LDFLAGS) -o 7segtest 7segtest.o
7segtest.o: 7segtest.c
	$(CC) $(CFLAGS) -o 7segtest.o -c 7segtest.c
clean:
	rm 7segtest.o 7segtest
