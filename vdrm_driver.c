#include <linux/slab.h>

#include <drm/drm_drv.h>
#include <drm/drm_gem.h>
#include <drm/drm_file.h>
#include <drm/drm_ioctl.h>

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



struct vdrm_driver *vdrm_driver_init(void) {
	struct vdrm_driver *drv;
	drv = (struct vdrm_driver *)kzalloc(sizeof(struct vdrm_driver), GFP_KERNEL);
	if (drv == NULL) {
		return NULL;
	}
	drv->drm_drv = &vdrm_drv;
	return drv;
}

void vdrm_driver_clean(struct vdrm_driver *drv) {
	kfree(drv);
}