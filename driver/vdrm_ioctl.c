#include "vdrm_ioctl.h"

#include <linux/slab.h>

struct ioctl_data *ioctl_data_init(unsigned int id, unsigned int request, uint16_t size) {
	struct ioctl_data *ioctl;
	ioctl = (struct ioctl_data *)kzalloc(sizeof(struct ioctl_data) + size, GFP_KERNEL);
	if (!ioctl) {
		return NULL;
	}
	ioctl->id = id;
	ioctl->request = request;
	ioctl->size = size;
	return ioctl;
}