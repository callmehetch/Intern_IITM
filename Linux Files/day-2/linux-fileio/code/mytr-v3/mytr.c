#include <unistd.h>
#include <error.h> 
#include <errno.h>

#define BUF_SIZE 256

int main(int argc, char *argv[])
{
	char buf[BUF_SIZE];
	ssize_t rlen, wlen;
	int i;
	char from;
	char to;

	from = 'e';
	to = 'a';

	while (1) {
		rlen = read(0, buf, sizeof(buf));
		if (rlen == -1) /* <1> */
			error(1, errno, "error reading input"); /* <2> */
		if (rlen == 0)
			return 0;
		
		for (i = 0; i < rlen; i++) {
			if (buf[i] == from)
				buf[i] = to;
		}
		
		wlen = write(1, buf, rlen);
		if (wlen == -1) /* <3> */
			error(1, errno, "error writing to output");
	}

	return 0;
}
