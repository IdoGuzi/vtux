#include "client.h"
#include "vdrm_ioctl.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int sendToServer(struct Client *client, struct ioctl_data *data) {
	int err = send(client->sock_fd, (void*)data, sizeof(struct ioctl_data) + data->size, 0);
	if (err < 0) {
		err = errno;
		printf("failed to send data to server, what: %s\n", strerror(err));
		return err;
	}
	return 0;
}

int receiveFromServer(struct Client *client, void *data, size_t size) {
	int err = recv(client->sock_fd, data, sizeof(size), 0);
	if (err < 0) {
		err = errno;
		printf("failed to receive data from server, what: %s\n", strerror(err));
		return err;
	}
	return 0;
}

int connectToServer(struct Client *client, char *ip, int port) {
	struct sockaddr_in server_info;
	memset(&server_info, 0, sizeof(struct sockaddr_in));
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = inet_addr(ip);
	server_info.sin_port = htons(port);
	int err = connect(client->sock_fd, (struct sockaddr*)&server_info, sizeof(server_info));
	if (err) {
		err = errno;
		printf("failed to connect to server, what: %s\n", strerror(err));
		return err;
	}
	return 0;
}

struct Client* createClient() {
	struct Client *client = (struct Client*) malloc(sizeof(struct Client));
	client->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client->sock_fd < 0) {
		printf("failed to create client, what: %s\n", strerror(client->sock_fd));
		free(client);
		return NULL;
	}
	client->connect = connectToServer;
	client->receive = receiveFromServer;
	client->send = sendToServer;
	return client;
}

void destroyClient(struct Client *client) {
	close(client->sock_fd);
	free(client);
}

