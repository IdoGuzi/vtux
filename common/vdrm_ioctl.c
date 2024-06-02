#include "vdrm_ioctl.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct ioctl_data *ioctl_data_init(unsigned int dev_id, unsigned int id, unsigned int request, uint16_t size) {
	struct ioctl_data *ioctl;
	ioctl = (struct ioctl_data *)malloc(IOCTL_DATA_BASE_SIZE + size);
	memset(ioctl, 0, IOCTL_DATA_BASE_SIZE + size);
	if (!ioctl) {
		return NULL;
	}
	ioctl->id = id;
	ioctl->dev_id = dev_id;
	ioctl->request = request;
	ioctl->size = size;
	return ioctl;
}
