#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define PBUTTON_CNT 9
#define DOTM_MAGIC 0xBC
#define DOTM_SET_ALL _IOW(DOTM_MAGIC, 0, int)
#define DOTM_SET_CLEAR _IOW(DOTM_MAGIC, 1, int)

int main(int argc, char **argv)
{
	int pbutton, dotmatrix, dotmatrix2;
	ssize_t ret;
	unsigned char clicked[PBUTTON_CNT];
	int student_id[8] = {2, 0, 1, 6, 2, 6, 0, 4};
	pbutton = open("/dev/pbutton", O_RDONLY);
	if (pbutton == -1) {
		printf("Failed to open pbutton!");
		return EXIT_FAILURE;
	}

	dotmatrix = open("/dev/dotmatrix", O_RDWR);
	if (dotmatrix == -1) {
		printf("Failed to open dotmatrix!");
		return EXIT_FAILURE;
	}

	dotmatrix2 = open("/dev/dotmatrix2", O_RDWR);
	if (dotmatrix2 == -1) {
		printf("Failed to open dotmatrix2!");
		return EXIT_FAILURE;
	}

	while (true) {
		ret = read(pbutton, clicked, PBUTTON_CNT * sizeof(unsigned char));
		if (ret != PBUTTON_CNT * sizeof(unsigned char)) {
			printf("Pbutton read Error!\n");
			return EXIT_FAILURE;
		}

		unsigned char dm_args[3] = {0, 0, 0};
		if (clicked[0]) {
			ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);
			dm_args[0] = '0';
			write(dotmatrix2, dm_args, sizeof(dm_args));
		} else if (clicked[1]) {
			ioctl(dotmatrix2, DOTM_SET_CLEAR, NULL);
			dm_args[0] = '1';
			write(dotmatrix2, dm_args, sizeof(dm_args));
		} else if (clicked[2]) {
			ioctl(dotmatrix2, DOTM_SET_CLEAR, NULL);
			dm_args[0] = '2';
			write(dotmatrix2, dm_args, sizeof(dm_args));
		} else if (clicked[3]) {
			for (int i = '0'; i < '3'; ++i) {
				ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);
				dm_args[0] = i;
				write(dotmatrix2, dm_args, sizeof(dm_args));
				usleep(500000);
			}
			ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);

		} else if (clicked[4]) {
			for (int i = 0; i < 8; ++i) {
				ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);
				write(dotmatrix, &student_id[i], sizeof(unsigned char));
				usleep(500000);
			}
		} else if (clicked[5]) {
			for (int i = 0; i < 30; ++i) {
				ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);
				dm_args[0] = '3';
				dm_args[1] = '0' + i / 10;
				dm_args[2] = '0' + i % 10;
				write(dotmatrix2, dm_args, sizeof(dm_args));
				usleep(50000);
			}
			ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);
		} else if (clicked[6]) {
			for (int i = 0; i < 21; ++i) {
				ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);
				dm_args[0] = '4';
				dm_args[1] = '0' + i / 10;
				dm_args[2] = '0' + i % 10;
				write(dotmatrix2, dm_args, sizeof(dm_args));
				usleep(50000);
			}
			ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);
		} else if (clicked[7]) {
			for (int i = 20; i >= 0; --i) {
				ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);
				dm_args[0] = '4';
				dm_args[1] = '0' + i / 10;
				dm_args[2] = '0' + i % 10;
				write(dotmatrix2, dm_args, sizeof(dm_args));
				usleep(50000);
			}
			ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);
		} else if (clicked[8]) {
			for (int i = 29; i >= 0; --i) {
				ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);
				dm_args[0] = '3';
				dm_args[1] = '0' + i / 10;
				dm_args[2] = '0' + i % 10;
				write(dotmatrix2, dm_args, sizeof(dm_args));
				usleep(50000);
			}
			ioctl(dotmatrix, DOTM_SET_CLEAR, NULL);
		}
	}
	close(pbutton);
	close(dotmatrix);
	return 0;
}
