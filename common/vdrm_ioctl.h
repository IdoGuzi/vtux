#ifndef _VDRM_IOCTL_H_
#define _VDRM_IOCTL_H_

#include <stdint.h>

#define DATA_SIZE 16384 //2^14
#define IOCTL_DATA_BASE_SIZE 14


struct ioctl_data {
	unsigned int id;
	unsigned int dev_id;
	unsigned int request;
	uint16_t size;
	char data[];
};

struct ioctl_data *ioctl_data_init(unsigned int dev_id, unsigned int id, unsigned int request, uint16_t size);



#endif /*_VDRM_IOCTL_H_*/
