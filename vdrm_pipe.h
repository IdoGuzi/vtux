#ifndef _VDRM_PIPE_H_
#define _VDRM_PIPE_H_

#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/spinlock.h>

struct vdrm_pipe {
	size_t len;
	void *data;
	spinlock_t dataLock;
	wait_queue_head_t producer;
	wait_queue_head_t consumer;
};


int vdrm_pipe_init(struct vdrm_pipe *pipe);
void vdrm_pipe_clean(struct vdrm_pipe *pipe);
int vdrm_pipe_get_data(struct vdrm_pipe *pipe, void *data);
int vdrm_pipe_set_data(struct vdrm_pipe *pipe, void *data, size_t len);



#endif /*_VDRM_PIPE_H_*/