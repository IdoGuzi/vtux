#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

#include "controller.h"

struct controller *controller;

static int __init vdrm_init(void) {
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
}


module_init(vdrm_init);
module_exit(vdrm_exit);

MODULE_ALIAS("vdrm");
MODULE_LICENSE("GPL");