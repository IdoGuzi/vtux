#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#include <sys/ioctl.h>

#include "vdrm_ioctl.h"

#include "server.h"

#define BUFFER_SIZE 16384+sizeof(struct ioctl_data) //2^14

const char path[] = "/dev/vtux";

int main() {
	struct Server *server = createServer();
	int err = server->start(server);
	if (err) {
		printf("failed to start up server, what: %d\n", err);
		destroyServer(server);
		return EXIT_FAILURE;
	}
	int fd = open(path, O_RDWR);
	if (fd < 0) {
		err = errno;
		printf("failed to open vtux controller, what: %s\n", strerror(err));
		return -err;
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
			err = errno;
			printf("failed to read from vtux, what: %s\n", strerror(err));
			break;
		}
		if (!byteRead) {
			printf("read 0 bytes from vtux (EOF)\n");
			continue;
		}
		if (byteRead < BUFFER_SIZE) {
			buf[byteRead] = '\0';
		}
		printf("buffer: %s\n", buf);
		ioctl = (struct ioctl_data*) buf;
		printf("ioctl command: 0x%x/ %u\n", ioctl->request, ioctl->request);
		printf("ioctl arg size: %d\n", ioctl->size);
		err = server->sender(server, ioctl->dev_id, *ioctl);
		if (err) {
			printf("failed to send data to client, what: %s\n", strerror(-err));
			break;
		}
	}
	close(fd);
	destroyServer(server);
	return EXIT_SUCCESS;
}
