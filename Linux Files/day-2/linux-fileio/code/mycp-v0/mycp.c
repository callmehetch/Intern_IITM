#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>

void write_buf(int fd, char *bufp, size_t nbytes, char *filename)
{
	int written;

	while (nbytes != 0) {
		written = write(fd, bufp, nbytes);
		if (written == -1)
			error(1, errno, "error writing to file %s", filename);

		if (written <= nbytes) {
			bufp += written;
			nbytes -= written;
		}
	}
}

int main(int argc, char *argv[])
{
	char *source;
	char *dest;
	int source_fd, dest_fd;
	char buf[1024];
	ssize_t read_len;

	if (argc != 3) {
		printf("Usage: mycp <src-filename> <dest-filename>\n");
		exit(1);
	}

	source = argv[1];
	dest = argv[2];

	source_fd = open(source, O_RDONLY);
	if (source_fd == -1)
		error(1, errno, "error opening file %s", source);

	dest_fd = open(dest, O_WRONLY);
	if (dest_fd == -1)
		error(1, errno, "error opening file %s", dest);

	while (1) {
		read_len = read(source_fd, buf, sizeof(buf));
		if (read_len == -1)
			error(1, errno, "error reading file %s", source);
		
		if (read_len == 0)
			break;

		write_buf(dest_fd, buf, read_len, dest);
	}

	if (close(source_fd) == -1) {
		error(1, errno, "error closing file %s", source);
	}

	if (close(dest_fd) == -1) {
		error(1, errno, "error closing file %s", dest);
	}

	return 0;
}
