#include <linux/wait.h>
#include <linux/spinlock.h>

#include "vdrm_pipe.h"

int vdrm_pipe_init(struct vdrm_pipe *pipe) {
	struct vdrm_pipe *pipe;
	pipe = (struct vdrm_pipe *)kzalloc(sizeof(struct vdrm_pipe), GFP_KERNEL);
	spin_lock_init(&pipe->dataLock);
	init_waitqueue_head(&pipe->producer);
	init_waitqueue_head(&pipe->producer);
}
void vdrm_pipe_clean(struct vdrm_pipe *pipe) {
	kree(pipe);
}
