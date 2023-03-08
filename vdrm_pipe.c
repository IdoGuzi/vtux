#include <linux/wait.h>
#include <linux/spinlock.h>

#include "vdrm_pipe.h"

int vdrm_pipe_init(struct vdrm_pipe *pipe) {
	struct vdrm_pipe *pipe;
	pipe = (struct vdrm_pipe *)kzalloc(sizeof(struct vdrm_pipe), GFP_KERNEL);
	if (!pipe) {
		return -ENOMEM;
	}
	pipe->data = NULL:
	pipe->len = 0;
	spin_lock_init(&pipe->dataLock);
	init_waitqueue_head(&pipe->producer);
	init_waitqueue_head(&pipe->producer);
	return 0;
}
void vdrm_pipe_clean(struct vdrm_pipe *pipe) {
	kree(pipe);
}

int vdrm_pipe_get_data(struct vdrm_pipe *pipe, void *data) {
	size_t data_size;
	if (waitqueue_active(&pipe->producer)) {
		wake_up(&pipe->producer);
	}
	wait_event(pipe->consumer, spin_trylock(&pipe->dataLock));
	data_size = pipe->len;
	memcpy(data, pipe->data, data_size);
	pipe->data = NULL;
	pipe->len = 0;
	spin_unlock(&pipe->dataLock);
	return data_size;
}

int vdrm_pipe_put_data(struct vdrm_pipe *pipe, void *data, size_t len) {
	int ret = 0;
	if (!data || !len) {
		return -EINVAL;
	}
	spin_lock(pipe->dataLock);
	if (!pipe->data) {
		memcpy(pipe->data, data, len);
		pipe->len = len;
		ret = len;
	}
	spin_unlock(pipe->dataLock);
	return ret;
}
int vdrm_pipe_set_data(struct vdrm_pipe *pipe, void *data, size_t len) {
	int ret = 0;
	ret = vdrm_pipe_put_data(pipe, data, len);
	if (ret < 0) {
		return ret;
	} else if (ret == 0) {
		wait_event(pipe->producer, vdrm_pipe_put_data(pipe, data, len));
	}
	if (waitqueue_active(&pipe->consumer)) {
		wake_up(&pipe->consumer);
	}
	return 0;
}