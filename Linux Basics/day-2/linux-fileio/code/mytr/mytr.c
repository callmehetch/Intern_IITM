#include <unistd.h>
#include <error.h>
#include <errno.h>

#define BUF_SIZE 256

int main(int argc, char *argv[])
{
	char buf[BUF_SIZE];
	char *bufp;
	ssize_t rlen, wlen;
	int i;
	char from;
	char to;

	if (argc != 3) {
		error(1, 0, "too many args or required args not specified");
	}

	from = argv[1][0];
	to = argv[2][0];

	while (1) {
		rlen = read(STDIN_FILENO, buf, sizeof(buf));
		if (rlen == -1)
			error(1, errno, "error reading input");
		if (rlen == 0)
			return 0;
		
		for (i = 0; i < rlen; i++) {
			if (buf[i] == from)
				buf[i] = to;
		}
		
		wlen = 0;
		bufp = buf;
		do {
			wlen = write(STDOUT_FILENO, bufp, rlen);
			if (wlen == -1)
				error(1, errno, "error writing to output");
			
			rlen -= wlen;
			bufp += wlen;
		} while (rlen != 0);
	}

	return 0;
}
