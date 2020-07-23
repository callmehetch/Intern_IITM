#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <errno.h>

int main()
{
	int fd;
	unsigned char buf[3];
	int ret;

	fd = open("/dev/input/mice", O_RDONLY);
	if (fd == -1) {
		error(1, errno, "error opening mouse device");
	}

	while (1) {
		ret = read(fd, buf, sizeof(buf));
		if (ret == -1) {
			error(1, errno, "error reading mouse device");
		}

		if (buf[2] != 0) {
			if (buf[0] & 0x20) {
				printf("Mouse down\n");
			} else {
				printf("Mouse up\n");
			}
		}

		if (buf[1] != 0) {
			if (buf[0] & 0x10) {
				printf("Mouse left\n");
			} else {
				printf("Mouse right\n");
			}
		}

	}
}
