#include <unistd.h>

#define BUF_SIZE 256

int main(int argc, char *argv[])
{
	char buf[BUF_SIZE];
	ssize_t rlen;
	int i;
	char from;
	char to;

	from = 'e';
	to = 'a';

	while (1) {
		rlen = read(0, buf, sizeof(buf));
		if (rlen == 0)
			return 0;
		
		for (i = 0; i < rlen; i++) {
			if (buf[i] == from)
				buf[i] = to;
		}
		
		write(1, buf, rlen);
	}

	return 0;
}
