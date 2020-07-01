#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/uio_driver.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#include <asm/uaccess.h>
MODULE_LICENSE("GPL");

#define  HELLO_MAJOR 62
#define  HELLO_MINORS 2

#define MAX_BUFF 1024
char *data;
struct uio_info info;
static struct class *cls;
static struct platform_device *dummy_device;
static struct cdev myDev;

static int uio_probe(struct device *dev);
static int uio_remove(struct device *dev);
static struct device_driver dummy_driver = {
	.name = "charDev",
	.bus = &platform_bus_type,
	.probe = uio_probe,
	.remove = uio_remove,
};


ssize_t uio_read(struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	int ret = 0;
	if(count > MAX_BUFF)
	{
		ret = copy_to_user(buf, data, MAX_BUFF);
	}
	else
	{
		ret = copy_to_user(buf, data, count);
	}
	printk("uio_read\n");
	return ret;
}

ssize_t uio_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
	int ret = 0;
	if(count > MAX_BUFF)
	{
		ret = copy_from_user(data,  buf, MAX_BUFF);
	}
	else
	{
		ret = copy_from_user(data, buf, count);
	}
	printk("uio_write %s\n", data);
	return ret;
}

static int uio_open(struct inode *inode, struct file *file)
{
	printk("uio_open\n");
	return 0;
}
static struct file_operations ops = {
	.read = uio_read,
	.write = uio_write,
	.open = uio_open,
	.owner = THIS_MODULE
};

static int uio_probe(struct device *dev)
{
	int rt;
	printk("uio_prob\n");
	info.version = "0.0.1";
	info.irq = UIO_IRQ_NONE;
	info.name = "myuio";
	info.mem[0].name = "mem0";
	info.mem[0].memtype = UIO_MEM_VIRTUAL;
	//info.mem[0].memtype = UIO_MEM_LOGICAL;
	info.mem[0].addr = (unsigned long) data;
	info.mem[0].size = MAX_BUFF;

	rt =  uio_register_device(dev, &info);
	
	printk("uio_register_device %d\n", rt);
	return rt;
}
static int uio_remove(struct device *dev)
{
	uio_unregister_device(&info);
	return 0;
}
static int __init hello_init(void)
{
	int ret;
	printk(KERN_ALERT "hello world\n");
	//struct cdev *hello_dev = cdev_alloc();
	//hello_dev->ops = &ops; 
	
	//cdev_init(hello_dev, &ops);
	//data = (char *)kmalloc(MAX_BUFF, GFP_KERNEL);
	data = (char *)vmalloc_user(MAX_BUFF);
	printk(KERN_ALERT "register_chrdev\n");
	cdev_init(&myDev, &ops);
	cdev_add(&myDev, MKDEV(HELLO_MAJOR, 0), 1);

	dummy_device = platform_device_register_simple("charDev", -1, NULL, 0);
	ret = driver_register(&dummy_driver);

	ret = register_chrdev_region(MKDEV(HELLO_MAJOR, 0), HELLO_MINORS,  "hello");
	if(ret !=0)
	{
		return ret;
	}

	printk(KERN_ALERT "class_create\n");
	cls = class_create(THIS_MODULE, "charDev");
	device_create(cls, 0, MKDEV(HELLO_MAJOR, 0), 0, "charDev0");
	if(ret <0)
		return ret;
//	if(hello_major == 0)
//		hello_major = ret;
	return 0;
}
static void hello_exit(void)
{
	cdev_del(&myDev);
	device_destroy(cls, MKDEV(HELLO_MAJOR, 0));
	class_destroy(cls);
	unregister_chrdev_region(MKDEV(HELLO_MAJOR, 0), HELLO_MINORS);
	printk(KERN_ALERT "good bye\n");
	platform_device_unregister(dummy_device);
	return driver_unregister(&dummy_driver);
}

module_init(hello_init);
module_exit(hello_exit);
