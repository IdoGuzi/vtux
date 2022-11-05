#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/device/class.h>

struct controller {
    dev_t version;
    struct cdev cdev;
    struct class *class;
    struct device *dev;
};

struct controller *controller_init(void);

void controller_clean(struct controller *con);

#endif