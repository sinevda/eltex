#include <linux/module.h>
#include <linux/configfs.h>
#include <linux/init.h>
#include <linux/vt_kern.h>
#include <linux/timer.h>
 
 
MODULE_AUTHOR("Danil Sinev");
MODULE_LICENSE("GPL");

static struct kobject *myobj;
static struct timer_list my_timer;
static struct tty_driver *my_driver;

static int ledstatus = 0;
static int test = 7;

#define BLINK_DELAY   HZ/5
#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF


static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", test);
}
 
static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
        sscanf(buf, "%du", &test);
        return count;
}

static struct kobj_attribute foo_attribute =__ATTR(test, 0660, foo_show, foo_store);


static void my_timer_func(struct timer_list *ptr)
{
        int *pstatus = &ledstatus;

        *pstatus = *pstatus == test ? RESTORE_LEDS : test;

        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, *pstatus);

        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
}

static int __init leds_init(void)
{
        printk(KERN_INFO "leds: loading\n");
        printk(KERN_INFO "leds: fgconsole is %x\n", fg_console);

        my_driver = vc_cons[fg_console].d->port.tty->driver;
        
        myobj = kobject_create_and_add("sysled", kernel_kobj);
        if (!myobj)
                return -ENOMEM;

        int error = sysfs_create_file(myobj, &foo_attribute.attr);
        if (error)
                pr_debug("failed to create file /sys/kernel/sysled \n");

        timer_setup(&my_timer, my_timer_func, 0);
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);

        return 0;
}

static void __exit leds_exit(void)
{
        printk(KERN_INFO "leds: unloading...\n");
        del_timer(&my_timer);
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED, RESTORE_LEDS);

        kobject_put(myobj);
}

module_init(leds_init);
module_exit(leds_exit);