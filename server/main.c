#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include <sys/ioctl.h>

#include "vdrm_ioctl.h"

#define BUFFER_SIZE 16384+sizeof(struct ioctl_data) //2^14

const char path[] = "/dev/vtux";

int main() {
	int fd = open(path, O_RDWR);
	if (fd < 0) {
		printf("failed to open vtux controller, what: %s\n", strerror(-fd));
		return fd;
	}
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);
	struct ioctl_data *ioctl;
	ssize_t byteRead = 0;
	while (1) {
		printf("before read\n");
		byteRead = read(fd, buf, BUFFER_SIZE);
		printf("after read\n");
		if(byteRead < 0) {
			printf("failed to read from vtux, what: %ld\n", byteRead);
			continue;
		}
		if (!byteRead) {
			printf("read 0 bytes from vtux\n");
			continue;
		}
		if (byteRead < BUFFER_SIZE) {
			buf[byteRead] = '\0';
		}
		printf("buffer: %s\n", buf);
		ioctl = (struct ioctl_data*) buf;
		printf("ioctl command: 0x%x/ %u\n", ioctl->request, ioctl->request);
		printf("ioctl arg size: %d\n", ioctl->size);
		close(fd);
		break;
	}
	return EXIT_SUCCESS;
}