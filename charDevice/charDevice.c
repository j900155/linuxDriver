#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
MODULE_LICENSE("GPL");

#define  HELLO_MAJOR 61
#define  HELLO_MINORS 2
char data[20];
ssize_t char_read(struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	int ret = 0;
	if(count > 20)
	{
		ret = copy_to_user(buf, data, 20);
	}
	else
	{
		ret = copy_to_user(buf, data, count);
	}
	printk("char_read\n");
	return ret;
}

ssize_t char_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
	int ret = 0;
	if(count > 20)
	{
		ret = copy_from_user(data,  buf, 20);
	}
	else
	{
		ret = copy_from_user(data, buf, count);
	}
	printk("char_write %s\n", data);
	return ret;
}

static int char_open(struct inode *inode, struct file *file)
{
	printk("char_open\n");
	return 0;
}
static struct file_operations ops = {
	.read = char_read,
	.write = char_write,
	.open = char_open,
	.owner = THIS_MODULE
};
static struct cdev myDev;
static int hello_init(void)
{
	int ret;
	printk(KERN_ALERT "hello world\n");
	//struct cdev *hello_dev = cdev_alloc();
	//hello_dev->ops = &ops; 
	
	//cdev_init(hello_dev, &ops);
	ret = register_chrdev_region(MKDEV(HELLO_MAJOR, 0), HELLO_MINORS,  "hello");
	if(ret !=0)
	{
		return ret;
	}
	printk(KERN_ALERT "hello world\n");
	cdev_init(&myDev, &ops);
	cdev_add(&myDev, MKDEV(HELLO_MAJOR, 0), 1);
	if(ret <0)
		return ret;
//	if(hello_major == 0)
//		hello_major = ret;
	return 0;
}
static void hello_exit(void)
{
	cdev_del(&myDev);
	unregister_chrdev_region(MKDEV(HELLO_MAJOR, 0), HELLO_MINORS);
	printk(KERN_ALERT "good bye\n");
}

module_init(hello_init);
module_exit(hello_exit);
