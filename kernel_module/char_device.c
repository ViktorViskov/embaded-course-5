#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "char_device"
#define CLASS_NAME "custom_devices"
#define HELLO_STRING "Char device\n"


static int major_number;
static struct class* custom_devices = NULL;
static struct device* char_device = NULL;

static int device_open(struct inode *inode, struct file *file) { return 0; }
static int device_release(struct inode *inode, struct file *file) { return 0; }

static ssize_t device_read(struct file *file, char __user *buffer, size_t len, loff_t *offset) {
    size_t string_len = strlen(HELLO_STRING);

    if (*offset >= string_len) return 0;
    if (len > string_len - *offset) len = string_len - *offset;

    if (copy_to_user(buffer, HELLO_STRING + *offset, len)) return -EFAULT;

    *offset += len;
    return len;
}

static struct file_operations fops = {
    .read = device_read,
    .open = device_open,
    .release = device_release,
};

// Create modul in system
static int __init char_init(void) {
    // Device register
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Char device failed to register a major number\n");
        return major_number;
    }

    // Create device class
    custom_devices = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(custom_devices)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(custom_devices);
    }

    // Create device
    char_device = device_create(custom_devices, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(char_device)) {
        class_destroy(custom_devices);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(char_device);
    }

    printk(KERN_INFO "Char device created successfully\n");
    return 0;
}

// Delete model from system
static void __exit char_exit(void) {
    device_destroy(custom_devices, MKDEV(major_number, 0));
    class_unregister(custom_devices);
    class_destroy(custom_devices);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Char device unregistered\n");
}

module_init(char_init);
module_exit(char_exit);

// modul info
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Viktor Viskov");
MODULE_DESCRIPTION("A simple Linux char device that returns a string");
MODULE_VERSION("1.0");