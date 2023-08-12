#ifndef _VDRM_DRIVER_H_
#define _VDRM_DRIVER_H_

#include <linux/device.h>

#include <drm/drm_drv.h>
#include <drm/drm_device.h>

#include "vdrm_device.h"


/**
 * driver for a vdrm device
 */
struct vdrm_driver {
	struct drm_driver drm_drv;
	struct vdrm_device *drm_dev;
	struct device *parent; /*dummy device*/
};

/**
 * initialize vdrm driver (and a device for it)
 * parent - device responisble for the driver
 */
struct vdrm_driver *vdrm_driver_init(struct device *parent);

/**
 * remove vdrm driver from the system and clean allocated memory
 * drv - driver to remove and clean
 */
void vdrm_driver_clean(struct vdrm_driver *drv);


#endif /*_VDRM_DRIVER_H_*/
