#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

#include <xf86drm.h>

#define GPU "/dev/dri/card0"

#include "client.h"
#include "vdrm_ioctl.h"

int main(int argc, char *argv[]) {
	int err = 0;
	struct Client *client = createClient();
	err = client->connect(client, "127.0.0.1", 8000);
	if (err) {
		goto failedConnection;
	}
	struct ioctl_data *resp = ioctl_data_init(0, 0, 0, DATA_SIZE);
	int index = 0;
	while (1) {
		nanosleep((const struct timespec[]){{0,10000}}, NULL);
		char str[1024];
		sprintf(str, "test number %d", index);
		struct ioctl_data *data = ioctl_data_init(0, index, 123, strlen(str));
		strncpy(data->data, str, data->size);
		printf("ioctl dev id: %d, ioctl id: %d\n", data->dev_id, data->id);
		printf("ioctl command: 0x%x/ %u\n", data->request, data->request);
		printf("ioctl arg size: %d\n", data->size);
		printf("ioctl data: %s\n", data->data);
		err = client->send(client, data);
		free(data);
		if (err) {
			goto failedSend;
		}
		printf("sent to server\n");
		err = client->receive(client, (void*)resp, sizeof(struct ioctl_data)+DATA_SIZE);
		if (err) {
			goto failedReceive;
		}
		/*
		printf("ioctl dev id: %d, ioctl id: %d\n", resp->dev_id, resp->id);
		printf("ioctl command: 0x%x/ %u\n", resp->request, resp->request);
		printf("ioctl arg size: %d\n", resp->size);
		printf("ioctl data: %s\n", resp->data);
		*/
		index++;
	}
failedReceive:
failedSend:
	free(resp);
failedConnection:
	destroyClient(client);
	return err;
}

int main2(int argc, char *argv[]) {
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
