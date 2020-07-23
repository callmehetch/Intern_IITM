#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

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
	while (ret != 0) {
		/* Reading into buffer */
		ret = read(fd, buf, sizeof(buf));
		for (i = 0; i < ret; i++) {
			putchar(buf[i]);
		}
	}
  
	close(fd);
  
	return 0;
}
