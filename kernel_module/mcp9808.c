#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/i2c.h>
#include <linux/slab.h>


#define DEVICE_NAME "mcp9808_temp"
#define CLASS_NAME "custom_devices"
#define TEMP_BUS        2
#define TEMP_ADR        0x18

static int major_number;
static struct class* custom_devices = NULL;
static struct device* char_device = NULL;

static int device_open(struct inode *inode, struct file *file) { return 0; }
static int device_release(struct inode *inode, struct file *file) { return 0; }


static int get_temp(void) {
    struct i2c_adapter *adapter;
    u8 buf[2];
    int ret;
    int raw_data;
    int temp;

    struct i2c_client *client = kzalloc(sizeof(*client), GFP_KERNEL);

    adapter = i2c_get_adapter(TEMP_BUS);
    if (!adapter) {
        printk(KERN_ALERT "Failed to get I2C adapter %d\n", TEMP_BUS);
        return -ENODEV;
    }

    // printk(KERN_ALERT "Failed to get I2C client for address %d\n");
    client->adapter = adapter;
    client->addr = TEMP_ADR;
    if (!client) {
        printk(KERN_ALERT "Failed to get I2C client for address %d\n", TEMP_ADR);
        i2c_put_adapter(adapter);
        return -ENODEV;
    }

    ret = i2c_master_recv(client, buf, 2);
    if (ret < 0) {
        printk(KERN_ALERT "Failed to read data: %d\n", ret);
    }
    raw_data = (buf[0] << 8) | buf[1];
    temp = (raw_data >> 4) & 0xff;

    return temp;
}

static ssize_t device_read(struct file *file, char __user *buffer, size_t len, loff_t *offset) {
    int temp = get_temp();

    char temp_string[16];
    size_t string_len = snprintf(temp_string, sizeof(temp_string), "%d\n", temp);

    if (*offset >= string_len) return 0;
    if (len > string_len - *offset) len = string_len - *offset;

    if (copy_to_user(buffer, temp_string + *offset, len)) return -EFAULT;

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
        printk(KERN_ALERT "MCP9808 failed to register a major number\n");
        return major_number;
    }

    // Create device class
    custom_devices = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(custom_devices)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "MCP9808 failed to register device class\n");
        return PTR_ERR(custom_devices);
    }

    // Create device
    char_device = device_create(custom_devices, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(char_device)) {
        class_destroy(custom_devices);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "MCP9808 failed to create the device\n");
        return PTR_ERR(char_device);
    }

    printk(KERN_INFO "MCP9808 device created successfully\n");
    return 0;
}

// Delete model from system
static void __exit char_exit(void) {
    device_destroy(custom_devices, MKDEV(major_number, 0));
    class_unregister(custom_devices);
    class_destroy(custom_devices);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "MCP9808 device unregistered\n");
}

module_init(char_init);
module_exit(char_exit);

// modul info
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Viktor Viskov");
MODULE_DESCRIPTION("A simple Linux MCP9808 device that returns a temperature");
MODULE_VERSION("1.0");