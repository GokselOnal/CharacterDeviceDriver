/*
    This is a Character Device Driver. This Device Driver have read and
    write functionality.
    Copyright (C) 2021  Göksel can ÖNAL, 
    			Nurettin Burak Altıntaş, 
    			Alperen Fatih Zengin.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see https://www.gnu.org/licenses/.
    GitHub link:  https://github.com/GokselOnal/CharacterDeviceDriver
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/kdev_t.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alperen Zengin, Göksel Ünal, Nurettin Altıntaş");
MODULE_DESCRIPTION("A Linux character device driver");
MODULE_VERSION("0.01");

#define DRIVER_NAME "character_driver_project"
#define MSG_BUFFER_LEN 150

struct fake_device
{
    char data[200];
    struct semaphore sem;
} virtual_device;


/* Prototypes for device functions */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static int major_num;
static int device_open_count = 0;
static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr;
static struct class* cl;

/* This structure points to all of the device functions */
static struct file_operations file_ops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release};


static int project_driver_uevent(struct device *dev, struct kobj_uevent_env *env)

{

    add_uevent_var(env, "DEVMODE=%#o", 0666);

    return 0;

}

/*Called when the driver is initialised*/
static int __init char_driver_project_init(void)
{

    /* Try to register character device */
    major_num = register_chrdev(0, DRIVER_NAME, &file_ops);
    if (major_num < 0)
    {
        printk(KERN_ALERT "Could not register device: %d\n",major_num);
        return major_num;
    }
    else
    {
        printk(KERN_INFO "character_driver_project module loaded with device major number %d\n", major_num);
        
        /*Create a device file in /dev/ with the designated major number */ 
        cl = class_create(THIS_MODULE, "project_device_class");
        cl->dev_uevent = project_driver_uevent;
        device_create(cl, NULL, MKDEV(major_num, 0), NULL, "project_char_device");
        printk(KERN_INFO "project_char_device is created with major number %d and minor number 0\n", major_num);
        
        
        return 0;
    }
}
/* When a process reads from our device, this gets called. */
static ssize_t device_read(struct file *flip, char *buffer, size_t len, loff_t *offset)
{
    printk(KERN_INFO "Reading from device file");

    int ret = copy_to_user(buffer, virtual_device.data + *offset, len);
    return ret;
}
/* Called when a process tries to write to our device */
static ssize_t device_write(struct file *flip, const char *buffer, size_t len, loff_t *offset)
{

    printk(KERN_INFO "Writing to device file");
    int ret = copy_from_user(virtual_device.data + *offset, buffer, len);
    return ret;
}
/* Called when a process opens our device */
static int device_open(struct inode *inode, struct file *file)
{
    /* If device is open, return busy */
    if (device_open_count)
    {
        return -EBUSY;
    }
    device_open_count++;
    try_module_get(THIS_MODULE);
    return 0;
}
/* Called when a process closes our device */
static int device_release(struct inode *inode, struct file *file)
{
    /* Decrement the open counter and usage count. Without this, the module would not unload. */
    device_open_count--;
    module_put(THIS_MODULE);
    return 0;
}
/*Called when the driver is removed*/
static void __exit char_driver_project_exit(void)
{
    /* Remember — we have to clean up after ourselves. Unregister the character device. */
    device_destroy(cl,MKDEV(major_num,0));
    class_destroy(cl);
    printk(KERN_INFO "project_char_device has been removed\n");
    unregister_chrdev(major_num, DRIVER_NAME);
    printk(KERN_INFO "project_character_driver has been unloaded\n");
}
/* Register module functions */
module_init (char_driver_project_init);
module_exit (char_driver_project_exit);
