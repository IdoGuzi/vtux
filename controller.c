#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/device/class.h>

#include "controller.h"

static int controller_open(struct inode *inode, struct file *file) {
    struct controller *con;
    con = container_of(inode->i_cdev, struct controller, cdev);
    file->private_data = con;
    printk("open\n");
    return 0;
}

static int controller_release(struct inode *inode, struct file *file) {
    printk("release\n");
    return 0;
}

static ssize_t controller_read(struct file *file, char __user *user, size_t size, loff_t * offset) {
    printk("read\n");
    return 0;
}

static ssize_t controller_write(struct file *file, const char __user *user, size_t size, loff_t *offset) {
    printk("write\n");
    return 0;
}

static struct file_operations controller_ops = {
    .owner = THIS_MODULE,
    .open = controller_open,
    .release = controller_release,
    .read = controller_read,
    .write = controller_write,
};

static int controller_dev_uevent(struct device *dev, struct kobj_uevent_env *env) {
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

struct controller *controller_init(void) {
    int err;
    struct controller *con;
    con = (struct controller *)kzalloc(sizeof(struct controller), GFP_KERNEL);
    if (!con) {
        printk("failed to allocate controller\n");
        return NULL;
    }
    err = alloc_chrdev_region(&con->version, 0, 1, "vtux");
    if (err) {
        printk("failed to register char dev region\n");
        kfree(con);
        return NULL;
    }
    cdev_init(&con->cdev, &controller_ops);
    err = cdev_add(&con->cdev, con->version, 1);
    if (err) {
        printk("failed to add char device\n");
        unregister_chrdev_region(con->version, 1);
        kfree(con);
        return NULL;
    }
    con->class = class_create(THIS_MODULE, "vtux");
    if (IS_ERR(con->class)) {
        printk("failed to create class\n");
        cdev_del(&con->cdev);
        unregister_chrdev_region(con->version, 1);
        kfree(con);
        return NULL;
    }
    con->class->dev_uevent = controller_dev_uevent;
    con->dev = device_create(con->class, NULL, con->version, NULL, "vtux");
    if (IS_ERR(con->dev)) {
        printk("failed to create class device\n");
        class_destroy(con->class);
        cdev_del(&con->cdev);
        unregister_chrdev_region(con->version, 1);
        kfree(con);
        return NULL;
    }
    return con;
}

void controller_clean(struct controller *con) {
    device_destroy(con->class, con->version);
    class_destroy(con->class);
    cdev_del(&con->cdev);
    unregister_chrdev_region(con->version, 1);
    kfree(con);
}