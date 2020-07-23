#include <linux/kdev_t.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>

int main(int argc, char *argv[])
{
	char *fname;
	struct stat sbuf;
	int ret;

	bool ischar;
	bool isblk;

	if (argc != 2) {
		printf("Usage: devfile <device-filename>\n");
		exit(1);
	}
	
	fname = argv[1];

	ret = stat(fname, &sbuf);
	if (ret == -1) 
		error(1, errno, "error stating devicefile");

	ischar = S_ISCHR(sbuf.st_mode);
	isblk = S_ISBLK(sbuf.st_mode);

	if (ischar || isblk) {
		if (ischar)
			printf("Character Device\n");
		else
			printf("Block Device\n");

		printf("Major: %d Minor: %d\n", MAJOR(sbuf.st_rdev), 
		       MINOR(sbuf.st_rdev));
	}

	return 0;
}
