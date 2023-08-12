#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/device/class.h>

#include "vdrm_pipe.h"


/**
* controller is a char device for managing drm device communication to allow ioctl passthrough
*/
struct controller {
    dev_t version;
    struct cdev cdev;
    struct class *class;
    struct device *dev;
    struct vdrm_pipe *pipe;
};


/**
* intialize a controller struct and create a char device and register it to the system
*/
struct controller *controller_init(void);


/**
* delete the char device from the system and clean allocated memory
* con - controller to clean
*/
void controller_clean(struct controller *con);

#endif
