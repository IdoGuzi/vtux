#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include <drm/drm_drv.h>

#include "controller.h"
#include "vdrm_driver.h"

/*
static struct drm_driver vdrm_driver = {
    .ioctls
};
*/

struct vdrm_driver *driver;
struct controller *controller;

static int __init vdrm_init(void) {
    controller = controller_init();
    if (!controller) {
        return -1;
    }
    printk("vdrm - controller version: %p\n", controller);
    driver = vdrm_driver_init(controller->dev);
    if (!driver) {
        return -2;
    }
    printk("vdrm initianition successful\n");
    return 0;
}

static void __exit vdrm_exit(void) {
    vdrm_driver_clean(driver);
    controller_clean(controller);
}


module_init(vdrm_init);
module_exit(vdrm_exit);

MODULE_ALIAS("vdrm");
MODULE_LICENSE("GPL");