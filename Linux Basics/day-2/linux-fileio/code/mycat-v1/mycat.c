#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	char *filename;
	int fd;
	char buf[1024];
	ssize_t ret = 1;
	int i;

	if (argc != 2) {
		printf("Usage: mycat <filename>\n");
		exit(1);
	}

	filename = argv[1];

	fd = open(filename, O_RDONLY);
	if (fd == -1) { /* <1> */
		error(1, errno, "error opening file %s", filename); /* <2> */
	}
	while (ret != 0) {
		ret = read(fd, buf, sizeof(buf));
		if (ret == -1) { /* <3> */
			error(1, errno, "error reading file %s", filename);
		}
		for (i = 0; i < ret; i++) {
			putchar(buf[i]);
		}
	}

	if (close(fd) == -1) {
		error(1, errno, "error closing file %s", filename);
	}

	return 0;
}
