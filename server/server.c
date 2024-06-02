#include "server.h"
#include "vdrm_ioctl.h"

#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <features.h>
#include <signal.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>

struct CompactHandlerData {
	struct Server *server;
	struct Connection conn;
};

void* handle_connection(void *data);

void connectionListener(struct Server *server) {
	int server_status;
	while (1) {
		pthread_spin_lock(&server->lock);
		server_status = server->server_status;
		pthread_spin_unlock(&server->lock);
		if (!server_status) {
			break;
		}
		struct sockaddr_in new_connection;
		unsigned int conn_len = sizeof(new_connection);
		int conn_fd = accept(server->sock_fd, (struct sockaddr*)&new_connection, &conn_len);
		if (conn_fd < 0) {
			printf("failed to accept connection\n");
			continue;
		}
		printf("new client connected with fd %d\n", conn_fd);
		struct Connection conn = {
			.fd = conn_fd,
			.address = new_connection
		};
		printf("setting connection info in server\n");
		server->connections[server->connection_number] = conn;
		printf("connection info set\n");
		server->connection_number++;
		pthread_t thread;
		struct CompactHandlerData chd = {
			.server=server,
			.conn=conn
		};
		printf("starting client %d handler\n", conn_fd);
		pthread_create(&thread, NULL, handle_connection, (void*)&chd);
	}
}

int startServer(struct Server *server) {
	int err = listen(server->sock_fd, MAX_CONNECTIONS);
	if (err != 0) {
		return err;
	}
	server->server_status = 1;
	pthread_t thread;
	connectionListener(server);
	pthread_create(&thread, NULL, handle_connection, (void*)server/*&chd*/);
	return 0;
}

int stopServer(struct Server *server) {
	pthread_spin_lock(&server->lock);
	server->server_status = 0;
	pthread_spin_unlock(&server->lock);
	return 0;
}


int send_to_connection(struct Server *server, int fd, struct ioctl_data *data, size_t size) {
	int found = 0;
	for (int i=0; i<server->connection_number; i++) {
		if (server->connections[i].fd == fd) {
			found = 1;
			break;
		}
	}
	if (!found) {
		return -EBADF;
	}
	printf("*****************\n");
	printf("sending:\n");
	printf("*****************\n");
	printf("ioctl dev id: %d, ioctl id: %d\n", data->dev_id, data->id);
	printf("ioctl command: 0x%x/ %u\n", data->request, data->request);
	printf("ioctl arg size: %d\n", data->size);
	printf("ioctl data: %s\n", data->data);
	int err = send(fd, (void*)data, IOCTL_DATA_BASE_SIZE+data->size, 0);
	if (err) {
		err = errno;
		return -err;
	}
	return 0;
}

void* handle_connection(void *data) {
	struct CompactHandlerData *chd = (struct CompactHandlerData*)data;
	struct Server *server = chd->server;
	int conn_fd = (chd->conn.fd);
	//size_t buf_size = sizeof(struct ioctl_data)+16384;
	#define BUF_SIZE IOCTL_DATA_BASE_SIZE+16384
	char buf[BUF_SIZE] = {0};
	int server_status;
	while (1) {
		pthread_spin_lock(&server->lock);
		server_status = server->server_status;
		pthread_spin_unlock(&server->lock);
		if (!server_status) {
			break;
		}
		int err = recv(conn_fd, buf, BUF_SIZE, 0);
		if (err < 0) {
			printf("failed to read from client, closing connection, what: %s\n", strerror(errno));
			continue;
			close(conn_fd);
			return NULL;
		}
		err = server->processor(server, conn_fd, buf, err);
		if (err < 0) {
			printf("failed to write to client, closing connection, what: %s\n", strerror(errno));
			continue;
			close(conn_fd);
			return NULL;
		}
	}
	close(conn_fd);
	return NULL;
}



struct Server* createServer(int (*processor)(struct Server *server, int sender_fd, void *data, size_t size)) {
	struct Server *server = malloc(sizeof(struct Server));
	server->server_status = 0;
	server->connection_number = 0;
	server->connections = (struct Connection*)malloc(sizeof(struct Connection)*MAX_CONNECTIONS);
	server->start = startServer;
	server->stop = stopServer;
	server->sender = send_to_connection;
	server->handler = handle_connection;
	server->processor = processor;
	if (pthread_spin_init(&server->lock, PTHREAD_PROCESS_PRIVATE) != 0) {
		free(server);
		return NULL;
	}
	server->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->sock_fd < 0) {
		pthread_spin_destroy(&server->lock);
		free(server);
		return NULL;
	}
	server->server_info.sin_family = AF_INET;
	server->server_info.sin_port   = htons(8000);
	server->server_info.sin_addr.s_addr = INADDR_ANY;
	int err = bind(server->sock_fd, (struct sockaddr*)&server->server_info, sizeof(server->server_info));
	if (err) {
		pthread_spin_destroy(&server->lock);
		close(server->sock_fd);
		free(server);
		return NULL;
	}
	return server;
}

void destroyServer(struct Server *server) {
	server->stop(server);
	pthread_spin_destroy(&server->lock);
	close(server->sock_fd);
	free(server);
}
