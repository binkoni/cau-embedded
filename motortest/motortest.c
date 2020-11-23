#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define MOTOR_MAGIC 0xDB
#define MOTOR_SET_ACTION _IOW(MOTOR_MAGIC, 0, int)
#define MOTOR_SET_DIRECTION _IOW(MOTOR_MAGIC, 1, int)
#define MOTOR_SET_SPEED _IOW(MOTOR_MAGIC, 2, int)

int main(int argc, char **argv)
{
	int fd, param;
	if (argc < 3) {
		fprintf(stderr, "usage: %s <motor-action> <motor-direction> <motor-speed>\n", argv[0]);
		return 1;
	}

	fd = open("/dev/motor", O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "error opening device\n");
		return 1;
	}

	param = strtol(argv[1], NULL, 10);
	ioctl(fd, MOTOR_SET_ACTION, &param, _IOC_SIZE(MOTOR_SET_ACTION));
	param = strtol(argv[2], NULL, 10);
	ioctl(fd, MOTOR_SET_DIRECTION, &param, _IOC_SIZE(MOTOR_SET_DIRECTION));
	param = strtol(argv[3], NULL, 10);
	ioctl(fd, MOTOR_SET_SPEED, &param, _IOC_SIZE(MOTOR_SET_SPEED));
	close(fd);
	return 0;
}
