#include <linux/slab.h>

#include <drm/drm_drv.h>
#include <drm/drm_gem.h>
#include <drm/drm_file.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_device.h>

#include "vdrm_driver.h"
#include "vdrm_device.h"
#include "vdrm_ioctl.h"
#include "vdrm_print.h"
#include "vdrm_pipe.h"

#include "controller.h"


long vdrm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	//getting drm_device based on how drm_open stored it in drm_file
	struct drm_file *priv = filp->private_data;
	struct drm_device *dev = priv->minor->dev;
	struct vdrm_driver *vdrm = container_of(dev->driver, struct vdrm_driver, drm_drv);
	struct controller *con = (struct controller *)dev_get_drvdata(vdrm->parent);
	struct ioctl_data *ioctl;
	unsigned int arg_size;
	int err = 0;
	uint8_t ioctl_dir;

	//ioctl command number contain the size of argument type
	arg_size = _IOC_SIZE(cmd);
	ioctl_dir = _IOC_DIR(cmd);
	ioctl = ioctl_data_init(0, cmd, arg_size);
	if (!ioctl) {
		return -ENOMEM;
	}
	if (ioctl_dir & _IOC_WRITE) {
		err = copy_from_user(ioctl->data, (void __user *)arg, arg_size);
		if (err) {
			printv("copy from user error: %d\n", err);
			return -EFAULT;
		}
	}
	printv("ioctl type: %x, ioctl dir: %d, ioctl data length: %d, ioctl data: %.*s\n", _IOC_TYPE(cmd),ioctl_dir, ioctl->size, ioctl->size, ioctl->data);
	err = vdrm_pipe_set_data(con->pipe, ioctl, sizeof(struct ioctl_data) + arg_size);
	if (err) {
		return err;
	}
	return 0;
}

static const struct file_operations vdrm_fops = {
	.owner		= THIS_MODULE,
	.open		= drm_open,
	.release	= drm_release,
	.unlocked_ioctl	= vdrm_ioctl,
	.compat_ioctl	= drm_compat_ioctl,
	.poll		= drm_poll,
	.read		= drm_read,
	.llseek		= noop_llseek,
	.mmap		= drm_gem_mmap,
};

struct drm_driver vdrm_drv = {
	.driver_features = DRIVER_GEM | DRIVER_MODESET | DRIVER_ATOMIC,
	.name = "vdrm",
	.desc = "vitrual drm",
	.date = "20221016",
	.major = 1,
	.minor = 0,
	.fops = &vdrm_fops,
};


struct vdrm_driver *vdrm_driver_init(struct device *parent) {
	struct vdrm_driver *drv;

	drv = kzalloc(sizeof(struct vdrm_driver), GFP_KERNEL);
	if (IS_ERR(drv)) {
		printk("failed to allocate memory for vdrm driver, what: %li\n", PTR_ERR(drv));
		return NULL;
	}

	drv->drm_drv = vdrm_drv;
	drv->parent = parent;

	drv->drm_dev = vdrm_device_init(&drv->drm_drv, drv->parent);
	if (!drv->drm_dev) {
		printk("failed to initialize vdrm device container\n");
		kfree(drv);
		return NULL;
	}

	return drv;
}
/*
struct vdrm_driver *vdrm_driver_init(struct device *parent) {
	int err = 0;
	//struct device *dev;
	struct vdrm_driver *drv;


	//devm_drm_dev_alloc (and any other drm dev alloc function) will fail without parent device
	drv = devm_drm_dev_alloc(parent, &vdrm_drv, struct vdrm_driver, drm_dev);
	if (IS_ERR(drv)) {
		printk("failed to create vdrm_driver, what: %li\n", PTR_ERR(drv));
		goto exit_init;
	}

	drv->drm_drv = &vdrm_drv;
	drv->parent = parent;

	
	//minimum requirement to register drm device
	err = drmm_mode_config_init(&drv->drm_dev);
	if (err) {
		printk("failed to initialize drm device mode config, what: %d\n", err);
		return NULL;
	}

	//register the drm device to the system and userspace
	err = drm_dev_register(&drv->drm_dev, 0);
	if (err) {
		printk("failed to register drm device, what: errno %d\n", err);
		goto clean_and_exit;
	}

	return drv;
clean_and_exit:
	kfree(drv);
exit_init:
	return NULL;
}

*/
void vdrm_driver_clean(struct vdrm_driver *drv) {
	vdrm_device_clean(drv->drm_dev);
	kfree(drv);
}