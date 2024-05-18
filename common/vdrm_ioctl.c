#include "vdrm_ioctl.h"

#include <stdlib.h>
#include <stdint.h>

struct ioctl_data *ioctl_data_init(unsigned int dev_id, unsigned int id, unsigned int request, uint16_t size) {
	struct ioctl_data *ioctl;
	ioctl = (struct ioctl_data *)malloc(sizeof(struct ioctl_data) + size);
	if (!ioctl) {
		return NULL;
	}
	ioctl->id = id;
	ioctl->dev_id = dev_id;
	ioctl->request = request;
	ioctl->size = size;
	return ioctl;
}
