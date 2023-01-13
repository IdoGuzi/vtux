#ifndef _VDRM_DRIVER_H_
#define _VDRM_DRIVER_H_

#include <drm/drm_drv.h>

struct vdrm_driver {
	struct drm_driver *drm_drv;
};

struct vdrm_driver *vdrm_driver_init(void);
void vdrm_driver_clean(struct vdrm_driver *drv);


#endif /*_VDRM_DRIVER_H_*/