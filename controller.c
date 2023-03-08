#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/device/class.h>


#include "vdrm_pipe.h"
#include "controller.h"

#define NAME "vtux"

#define BASE_MINOR 0
#define MINOR_COUNT 1

#define printv(s, ...) printk("%s: %s", NAME, s, ##__VA_ARGS__)

static int controller_open(struct inode *inode, struct file *file) {
	struct controller *con;
	con = container_of(inode->i_cdev, struct controller, cdev);
	file->private_data = con;
	printv("open\n");
	return 0;
}

static int controller_release(struct inode *inode, struct file *file) {
	printv("release\n");
	return 0;
}

static ssize_t controller_read(struct file *file, char __user *user, size_t size, loff_t * offset) {
	struct controller *con = file->private_data;
	struct vdrm_ioctl ctl;
	char *data;
	size_t len;
	printv("read\n");
	len = vdrm_pipe_get_data(con->pipe, data);

	return 0;
}

static ssize_t controller_write(struct file *file, const char __user *user, size_t size, loff_t *offset) {
	printv("write\n");
	return 0;
}

static struct file_operations controller_ops = {
	.owner = THIS_MODULE,
	.open = controller_open,
	.release = controller_release,
	.read = controller_read,
	.write = controller_write,
};

static int controller_dev_uevent(struct device *dev, struct kobj_uevent_env *env) {
	add_uevent_var(env, "DEVMODE=%#o", 0666);
	return 0;
}

struct controller *controller_init(void) {
	int err;
	struct controller *con;
	con = (struct controller *)kzalloc(sizeof(struct controller), GFP_KERNEL);
	if (!con) {
		printv("failed to allocate controller\n");
		goto controller_alloc;
	}
	err = vdrm_pipe_init(con->pipe);
	if (err) {
		goto pipe_init_error;
	}

	err = alloc_chrdev_region(&con->version, BASE_MINOR, MINOR_COUNT, NAME);
	if (err) {
		printv("failed to register char dev region\n");
		goto region_error;
	}
	cdev_init(&con->cdev, &controller_ops);
	err = cdev_add(&con->cdev, con->version, 1);
	if (err) {
		printv("failed to add char device\n");
		goto chr_dev_add_error;
	}
	con->class = class_create(THIS_MODULE, NAME);
	if (IS_ERR(con->class)) {
		printv("failed to create class\n");
		goto class_creation_error;
	}
	con->class->dev_uevent = controller_dev_uevent;
	con->dev = device_create(con->class, NULL, con->version, NULL, "vtux");
	if (IS_ERR(con->dev)) {
		printv("failed to create class device\n");
		goto dev_creation_error;
	}
	con->dev->p = con;
	return con;
dev_creation_error:
	class_destroy(con->class);
class_creation_error:
	cdev_del(&con->cdev);
chr_dev_add_error:
	unregister_chrdev_region(con->version, 1);
region_error:
	vdrm_pipe_clean(con->pipe);
pipe_init_error:
	kfree(con);
controller_alloc:
	return NULL;
}

void controller_clean(struct controller *con) {
	device_destroy(con->class, con->version);
	class_destroy(con->class);
	cdev_del(&con->cdev);
	unregister_chrdev_region(con->version, 1);
	vdrm_pipe_clean(con->pipe);
	kfree(con);
}