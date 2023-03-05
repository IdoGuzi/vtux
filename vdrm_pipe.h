#ifndef _VDRM_PIPE_H_
#define _VDRM_PIPE_H_

#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/spinlock.h>

struct vdrm_pipe {
	void *data;
	spinlock_t dataLock;
	wait_queue_head_t producer;
	wait_queue_head_t consumer;
};


int vdrm_pipe_init(struct vdrm_pipe *pipe);
void vdrm_pipe_clean(struct vdrm_pipe *pipe, void *data);
int request_data(struct vdrm_pipe *pipe, void *data, size_t length) {
	if (waitqueue_active(&pipe->producer)) {
		wake_up(&pipe->producer);
	}
	wait_event(pipe->consumer, spin_trylock(&pipe->dataLock));
	memcpy(data, pipe->data, length);
}



#endif /*_VDRM_PIPE_H_*/