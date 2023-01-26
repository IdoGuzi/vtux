#ifndef _VDRM_DRIVER_H_
#define _VDRM_DRIVER_H_

#include <linux/device.h>

#include <drm/drm_drv.h>
#include <drm/drm_device.h>

struct vdrm_driver {
	struct drm_driver *drm_drv;
	struct drm_device drm_dev;
	struct device *parent; /*dummy device*/
};

struct vdrm_driver *vdrm_driver_init(struct device *parent);
void vdrm_driver_clean(struct vdrm_driver *drv);


#endif /*_VDRM_DRIVER_H_*/