CC = aarch64-linux-gnu-gcc

CFLAGS = 
LDFLAGS = -static

default: exam

exam: exam.o
	$(CC) $(LDFLAGS) -o exam exam.o
exam.o: exam.c
	$(CC) $(CFLAGS) -o exam.o -c exam.c
clean:
	rm exam.o exam
