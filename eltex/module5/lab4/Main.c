#include <linux/device.h>
#include <linux/init.h> 
#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/kernel.h> 
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Danil Sinev");

#define MESSAGE_LEN 100
#define DEVICE_NAME "chardev"
#define SUCCESS 0

static char msg[MESSAGE_LEN];
static int major;
static struct class* cls;

static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static ssize_t device_read(struct file*, char __user*, size_t, loff_t*);
static ssize_t device_write(struct file*, const char __user*, size_t, loff_t*);

static const struct file_operations fops = 
{
    .open = device_open,
    .read = device_read,
    .write = device_write,
    .release = device_release,
};



static int __init devmodule_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Failed to register device %s\n", DEVICE_NAME);
        return major; // Return the error code
    }

    cls = class_create(DEVICE_NAME);

    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

    printk(KERN_INFO "Device %s registered with major number %d.\n", DEVICE_NAME, major);
    return SUCCESS;
}

static void __exit devmodule_cleanup(void)
{
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);

    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Device %s released\n", DEVICE_NAME);
}

static int device_open(struct inode* inode, struct file* file)
{
    static int counter = 0;
    counter++;
    sprintf(msg, "You opened device %s %d times!\n", DEVICE_NAME, counter);
    return SUCCESS;
}

static int device_release(struct inode* inode, struct file* file)
{
    printk(KERN_NOTICE "Device %s released\n", DEVICE_NAME);
    return SUCCESS;
}

static ssize_t device_read(struct file* filep, char __user* buffer, size_t length, loff_t* offset)
{
	printk(KERN_NOTICE "Access to device %s: read", DEVICE_NAME);
    int bytes_read = 0;
    const char *msg_ptr = msg;

    if (*offset >= strlen(msg)) {
        *offset = 0; // Reset offset if we've read all data
        return 0; // End of file
    }

    msg_ptr += *offset;

    while (length && *msg_ptr) {
        put_user(*(msg_ptr++), buffer++);
        --length;
        ++bytes_read;
    }

    *offset += bytes_read; // Update offset
    return bytes_read;
}

static ssize_t device_write(struct file* filep, const char __user* buffer, size_t length, loff_t* offset)
{
	printk(KERN_NOTICE "Access to device %s: write", DEVICE_NAME);

    if (length > MESSAGE_LEN - 1)
        length = MESSAGE_LEN - 1; // Prevent buffer overflow

    if (copy_from_user(msg, buffer, length)) // Copy from user space
        return -EFAULT;

    msg[length] = '\0'; // Null-terminate the string

    return length; // Return number of bytes written
}

module_init(devmodule_init);
module_exit(devmodule_cleanup);