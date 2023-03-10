#ifndef _VDRM_DEVICE_H_
#define _VDRM_DEVICE_H_


/**
 * @brief vdrm_device is a drm_device container
 * initlization of drm_device is only available containeraized
 * 
 */
struct vdrm_device {
	unsigned int dev_id;
	struct drm_device drm_dev;
};

struct vdrm_device *vdrm_device_init(struct drm_driver *driver, struct device *parent);
void vdrm_device_clean(struct vdrm_device *vdev);

#endif /*_VDRM_DEVICE_H_*/