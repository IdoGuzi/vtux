#ifndef __SERVER_H__
#define __SERVER_H__
//#define _POSIX_C_SOURCE 200112L

#include "vdrm_ioctl.h"

#include <pthread.h>
#include <netinet/in.h>
#include <sched.h>

#define MAX_CONNECTIONS 64

struct Connection {
	int fd;
	struct sockaddr_in address;
};

struct Server {
	int sock_fd;
	int server_status;
	int connection_number;
	pthread_spinlock_t lock;
	struct sockaddr_in server_info;
	struct Connection *connections;
	int (*start)(struct Server *server);
	int (*stop)(struct Server *server);
	int (*sender)(struct Server *server, int fd, struct ioctl_data *data, size_t size);
	int (*processor)(struct Server *server, int sender_fd, void *data, size_t size);
	void* (*handler)(void *data);
};

struct Server* createServer(int (*processor)(struct Server *server, int sender_fd, void *data, size_t size));
void destroyServer(struct Server *server);

void start();


#endif /* __SERVER_H__ */
