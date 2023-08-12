#ifndef _VDRM_IOCTL_H_
#define _VDRM_IOCTL_H_

#include <linux/types.h>

#define DATA_SIZE 16384 //2^14

/**
 * container struct for an ioctl message for communication
 */
struct ioctl_data {
	unsigned int id;
	unsigned int request;
	uint16_t size;
	char data[];
};

/**
 * initialize an ioctl data container
 * id - id of the message
 * request - request type
 * size - data size in bytes
 */
struct ioctl_data *ioctl_data_init(unsigned int id, unsigned int request, uint16_t size);



#endif /*_VDRM_IOCTL_H_*/
