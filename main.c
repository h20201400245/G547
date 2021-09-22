#include<linux/kernel.h>
#include<linux/version.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/module.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/device.h>
#include<linux/time.h>
#include<linux/random.h>
#include "chardev.h"
#define driver_name "/dev/imu_char"



static dev_t get_major;
static struct cdev c_dev;
static struct class *cls;

static int device_open(struct inode *inode, struct file *file)
{
printk (KERN_INFO "%s opened\n",driver_name);
return 0;

}

static int device_release(struct inode *inode, struct file *file)
{
printk(KERN_INFO "%s close\n",driver_name);
return 0;

}



void  my_read(struct file *f,char __user *buffer, size_t length, loff_t *off)
{

       printk(KERN_INFO "char reading...\n");
}

void  my_write(struct file *f,char __user *buffer, size_t length, loff_t *off)
{

       printk(KERN_INFO "char writing...\n");
}


/*declaring ioctl*/
static long device_ioctl(struct file *file,unsigned int ioctl_num,unsigned long ioctl_param)
{
 int value;
//char *temp;
//char ch;

switch (ioctl_num)
{
case gy_xaxis:
value=(ioctl_num%3)+66;
break;

case gy_yaxis:
value=(ioctl_num%3)+66;
break;

case gy_zaxis:
value=(ioctl_num%3)+66;
break;

case acc_xaxis:
value=(ioctl_num%3)+66;
break;

case acc_yaxis:
value=(ioctl_num%3)+66;
break;

case acc_zaxis:
value=(ioctl_num%3)+66;
break;

case mag_xaxis:
value=(ioctl_num%3)+66;
break;

case mag_yaxis:
value=(ioctl_num%3)+66;
break;

case mag_zaxis:
value=(ioctl_num%3)+66;
break;

case bar_pressure:
value=(ioctl_num%3)+66;
break;

default:
return -1;
}
printk(KERN_INFO "%s has param=%d value=%d\n",driver_name, ioctl_num, value);
return value;


}

static struct file_operations f_ops=
{
	.owner= THIS_MODULE,
	.open= device_open,
	.release= device_release,
	.read= my_read,
	.write= my_write,
        .unlocked_ioctl=device_ioctl


};

static int __init my_init(void)
{
	printk(KERN_INFO "char registered\n");
        
	if(alloc_chrdev_region(&get_major,0,10,"DD assignment")<0)
	{
		return -1;
	}
         printk(KERN_INFO "Major=%d\n",MAJOR(get_major));
	if ((cls=class_create(THIS_MODULE,"chardrv"))==NULL)
	{
		unregister_chrdev_region(get_major,10);
		return -1;
	}
	if (device_create(cls,NULL,get_major,NULL,"imu_char")==NULL)
	{
		class_destroy(cls);
		unregister_chrdev_region(get_major,10);
		return -1;
	}

	cdev_init(&c_dev,&f_ops);
	if(cdev_add(&c_dev,get_major,10)==-1)
	{
		device_destroy(cls,get_major);
		class_destroy(cls);
		unregister_chrdev_region(get_major,10);
	}
	return 0;
}

static void __exit my_exit(void)
{
	cdev_del(&c_dev);
	device_destroy(cls,get_major);
	class_destroy(cls);
	unregister_chrdev_region(get_major,10);
	printk(KERN_INFO "char unregistered\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SUNIL");



