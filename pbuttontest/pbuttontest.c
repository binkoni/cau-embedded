#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define PBUTTON_CNT 9

int main(int argc, char **argv)
{
	int fd;
	ssize_t ret;
	unsigned char clicked[PBUTTON_CNT];
	fd = open("/dev/pbutton", O_RDONLY);
	ret = read(fd, clicked, PBUTTON_CNT * sizeof(unsigned char));
	if (ret < 0 || ret != PBUTTON_CNT * sizeof(unsigned char)) {
		printf("Read Error!\n");
		return -1;
	}

	for (int i = 0; i < PBUTTON_CNT; ++i) {
		printf("PBUTTON[%d]: 0x%x\n", i, clicked[i]);
	}
	close(fd);
	return 0;
}
