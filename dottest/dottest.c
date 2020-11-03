#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define PROGRAM_USAGE_STRING "usage: %s [0-9]\n"
#define DOTM_MAGIC 0xBC
#define DOTM_SET_ALL _IOW(DOTM_MAGIC, 0, int)
#define DOTM_SET_CLEAR _IOW(DOTM_MAGIC, 1, int)

int main(int argc, char **argv)
{
	int fd, num, i;
	if (argc <= 1) {
		fprintf(stderr, PROGRAM_USAGE_STRING, argv[0]);
		return -1;
	}
	num = (int) strtol(argv[1], NULL, 10);
	if (num < 0 || num > 9) {
		fprintf(stderr, PROGRAM_USAGE_STRING, argv[0]);
		return -1;
	}
	fd = open("/dev/dotmatrix", O_WRONLY);
	if (fd != -1) {
		ioctl(fd, DOTM_SET_ALL, NULL);
		usleep(500000);
		for (i = num; i < 10; i++) {
			write(fd, &i, sizeof(num));
			usleep(500000);
		}
		ioctl(fd, DOTM_SET_CLEAR, NULL);
		usleep(500000);
	} else {
		fprintf(stderr, "error opening device\n");
		return -1;
	}
	return 0;
}
