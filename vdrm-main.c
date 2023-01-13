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
    driver = vdrm_driver_init();
    controller = controller_init();
    if (!controller) {
        return -1;
    }
    printk("vdrm initianition successful\n");
    printk("vdrm - controller version: %p\n", controller);
    return 0;
}

static void __exit vdrm_exit(void) {
    controller_clean(controller);
    vdrm_driver_clean(driver);
}


module_init(vdrm_init);
module_exit(vdrm_exit);

MODULE_ALIAS("vdrm");
MODULE_LICENSE("GPL");