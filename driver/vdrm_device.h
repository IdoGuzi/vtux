#ifndef _VDRM_DEVICE_H_
#define _VDRM_DEVICE_H_


/**
 * vdrm_device is a drm_device container
 * initlization of drm_device is only available containeraized
 */
struct vdrm_device {
	unsigned int dev_id;
	struct drm_device drm_dev;
};

/**
 * initialize a vdrm_device and register a new drm device (gpu) to the system
 * driver - to use for the device
 * parent - the responsible device for the drm device
 */
struct vdrm_device *vdrm_device_init(struct drm_driver *driver, struct device *parent);

/**
 * unregister the drm device from the system and cleaning allocated memory
 * vdev - vdrm_device to unregister and clean memory
 */
void vdrm_device_clean(struct vdrm_device *vdev);

#endif /*_VDRM_DEVICE_H_*/
