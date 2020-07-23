#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <stdint.h>
#include <error.h>
#include <errno.h>
#include <stdio.h>

#include <linux/ppdev.h>

int main()
{
	int fd;
	int ret;
	uint8_t cdata;

	fd = open("/dev/parport0", O_RDONLY);
	if (fd == -1)
		error(1, errno, "error opening parallel port device");

	ret = ioctl(fd, PPCLAIM);
	if (ret == -1)
		error(1, errno, "error claiming parallel port");
	
	ret = ioctl(fd, PPRCONTROL, &cdata);
	if (ret == -1)
		error(1, errno, "error reading control port");

	while (1) {
		cdata |= 0x02;
		ret = ioctl(fd, PPWCONTROL, &cdata);
		if (ret == -1)
			error(1, errno, "error setting control port");

		sleep(1);
		
		cdata &= ~0x02;
		ret = ioctl(fd, PPWCONTROL, &cdata);
		if (ret == -1)
			error(1, errno, "error setting control port");
	
		sleep(1);
	}

	ret = ioctl(fd, PPRELEASE);
	if (ret == -1)
		error(1, errno, "error releasing parallel port");

	return 0;
}
