#ifndef _VDRM_PIPE_H_
#define _VDRM_PIPE_H_

#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/spinlock.h>


/**
 * pipe for data transfer with synchronization
 */
struct vdrm_pipe {
	size_t len;
	void *data;
	spinlock_t dataLock;
	wait_queue_head_t producer;
	wait_queue_head_t consumer;
};

/**
 * initialize pipe for data transfer
 */
struct vdrm_pipe *vdrm_pipe_init(void);

/**
 * delete pipe allocated memory
 * pipe - to delete
 */
void vdrm_pipe_clean(struct vdrm_pipe *pipe);

/**
 * get data from pipe (and clean pipe from data)
 * function is blocking (if pipe is empty, waits for pipe to be set)
 * pipe - to read from
 * data - in pipe, address of data array
 * return - size of data read, negetive for error
 */
int vdrm_pipe_get_data(struct vdrm_pipe *pipe, void **data);

/**
 * set data in pipe
 * function is blocking (if pipe is not empty, wait until pipe is empty)
 * pipe - to set data to
 * data - address of data array
 * len - length of data array
 * return - zero for success, negetive for error
 */
int vdrm_pipe_set_data(struct vdrm_pipe *pipe, void *data, size_t len);



#endif /*_VDRM_PIPE_H_*/
