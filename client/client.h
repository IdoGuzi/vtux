#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "vdrm_ioctl.h"


struct Client {
	int sock_fd;
	int (*send)(struct Client *client, struct ioctl_data data);
	int (*recive)(struct Client *client, void *data);
	int (*connect)(struct Client *client, char *ip, int port);
};

struct Client* createClient();
void destroyClient(struct Client *client);

#endif /*__CLIENT_H__*/
