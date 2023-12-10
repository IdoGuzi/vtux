#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include <xf86drm.h>

#define GPU "/dev/dri/card0"

int main(int argc, char *argv[]) {
	int fd, ret;

	int available = drmAvailable();
	if (available) {
		printf("drm available\n");
	} else {
		printf("libdrm not available, exiting...\n");
		goto unavailable_drm;
	}

	fd = open(GPU, O_RDWR);
	if (fd < 0) {
		ret = -errno;
		printf("failed to open %s, what: %s\n", GPU, strerror(errno));
		return ret;
	}
	int err = drmSetMaster(fd);
	if (err) {
		printf("failed to set drm master, what: %d\n", err);
		goto failed_drm_master;
	}
	int isMaster = drmIsMaster(fd);
	if (isMaster) {
		printf("got master access\n");
	} else {
		printf("failed to get drm master\n");
		goto failed_drm_master;
	}

//master_droping:
	err = drmDropMaster(fd);
	if (err) {
		printf("failed to drop master, what: %d\n", err);
	}
	printf("dropped master\n");
failed_drm_master:
	close(fd);
unavailable_drm:
	return 0;
}
