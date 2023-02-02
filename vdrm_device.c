#include <drm/drm_drv.h>

#include "vdrm_device.h"

struct vdrm_device *vdrm_device_init(struct drm_driver *driver, struct device *parent) {
	int err = 0;
	struct vdrm_device *vdrm_dev;

	//devm_drm_dev_alloc (and any other drm dev alloc function) will fail without parent device
	vdrm_dev = devm_drm_dev_alloc(parent, driver, struct vdrm_device, drm_dev);
	if (IS_ERR(vdrm_dev)) {
		printk("failed to create vdrm_driver, what: %li\n", PTR_ERR(vdrm_dev));
		goto exit_init;
	}

	//minimum requirement to register drm device
	err = drmm_mode_config_init(&vdrm_dev->drm_dev);
	if (err) {
		printk("failed to initialize drm device mode config, what: %d\n", err);
		goto exit_init;
	}

	//register the drm device to the system and userspace
	err = drm_dev_register(&vdrm_dev->drm_dev, 0);
	if (err) {
		printk("failed to register drm device, what: errno %d\n", err);
		goto exit_init;
	}

	return vdrm_dev;
exit_init:
	return NULL;
}

void vdrm_device_clean(struct vdrm_device *vdev) {
	drm_dev_unregister(&vdev->drm_dev);
	drm_dev_put(&vdev->drm_dev);
	//no need to kfree the driver, the allocation has auto cleanup with devres
}
