#include <linux/slab.h>

#include <drm/drm_drv.h>
#include <drm/drm_gem.h>
#include <drm/drm_file.h>
#include <drm/drm_ioctl.h>
#include <drm/drm_device.h>

#include "vdrm_driver.h"



DEFINE_DRM_GEM_FOPS(vdrm_fops);

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
	//int err=0;
	struct device *dev;
	struct vdrm_driver *drv;
	// drv = (struct vdrm_driver *)kzalloc(sizeof(struct vdrm_driver), GFP_KERNEL);
	// if (drv == NULL) {
	// 	return NULL;
	// }
	// drv->drm_drv = &vdrm_drv;

	// dev = (struct device *)kzalloc(sizeof(struct device), GFP_KERNEL);
	// if (IS_ERR(dev)) {
	// 	printk("failed to create dummy parent device, what: %li\n", PTR_ERR(dev));
	// 	return NULL;
	// }
	// err = device_register(dev);
	// if (err) {
	// 	printk("failed to register dummy parent device, what: %d\n", err);
	// 	return NULL;
	// }

	//devm_drm_dev_alloc (and any other drm dev alloc function) will fail without parent device
	drv = devm_drm_dev_alloc(parent, &vdrm_drv, struct vdrm_driver, drm_dev);
	if (IS_ERR(drv)) {
		printk("failed to create vdrm_driver, what: %li\n", PTR_ERR(drv));
		return NULL;
	}
	drv->drm_drv = &vdrm_drv;
	drv->parent = dev;
	return drv;
}

void vdrm_driver_clean(struct vdrm_driver *drv) {
	kfree(drv);
}