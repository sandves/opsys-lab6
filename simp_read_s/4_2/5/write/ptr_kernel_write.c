/*****************************************************************************

DESCRIPTION Skeleton of the read-driver

*****************************************************************************/


/*--------------------  I n c l u d e   F i l e s  -------------------------*/

#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO
#include <linux/init.h>		// included for __init and __exit macros
#include <linux/fs.h>
#include <linux/uaccess.h>

/*--------------------  C o n s t a n t s  ---------------------------------*/

#define DEV_MAJOR 0		//Means to use a dynamic 
#define DEV_NAME "ptr_kernel_write"

#define SUCCESS 0

/*--------------------  T y p e s  -----------------------------------------*/

/*--------------------  V a r i a b l e s  ---------------------------------*/

static int major_number;
static char pString[16]; 
static char *anotherBuff;
static char *end;

/*--------------------  F u n c t i o n s  ---------------------------------*/

// open function - called when the "file" /dev/simp_read is opened in userspace
static int dev_open (struct inode *inode, struct file *file) 
{
	printk("ptr_kernel_write: skeleton device driver open\n");
	return 0;
}


// close function - called when the "file" /dev/simp_read is closed in userspace  
static int dev_release (struct inode *inode, struct file *file)
{
	printk("ptr_kernel_write: skeleton device driver closed\n");
	return 0;
}

static ssize_t dev_write (struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	if( copy_from_user( pString, buf, count ) )
	{
		printk("ptr_kernel_write: copy_from_user failed\n");
		return -EFAULT;
	}
	else
	{
		printk( "[ptr_kernel_write] this is the address I got: %s\n", pString );
		anotherBuff = (char*)simple_strtol( pString , &end, 16 );
		printk( "[ptr_kernel_write] ptr = %p, and the contents of this memory is: \"%s\"\n", anotherBuff, anotherBuff );
		//sprintf(pString + strlen(pString), "\n");
		return count;
	}
}

// define which file operations are supported
struct file_operations dev_fops = 
{
	.owner	=	THIS_MODULE,
	.llseek	=	NULL,
	.read		=	NULL,
	.write		=	dev_write,
	.readdir	=	NULL,
	.poll		=	NULL,
	.ioctl	=	NULL,
	.mmap		=	NULL,
	.open		=	dev_open,
	.flush	=	NULL,
	.release	=	dev_release,
	.fsync	=	NULL,
	.fasync	=	NULL,
	.lock		=	NULL,
};



// initialize module 
static int __init dev_init_module (void) 
{
	major_number = register_chrdev( 0, DEV_NAME, &dev_fops);

	if (major_number < 0) 
	{
		printk(KERN_ALERT "Registering char device failed with %d\n", major_number);
		return major_number;
	}		
	
	printk(KERN_INFO "I was assigned major number %d. To talk to\n", major_number);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEV_NAME, major_number);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");
	

	return SUCCESS;
}

// close and cleanup module
static void __exit dev_cleanup_module (void) 
{
	/* 
	 * Unregister the device 
	 */
	unregister_chrdev(major_number, DEV_NAME);

		
	printk("ptr_kernel_write: dev_cleanup_module Device Driver Removed\n");
}

module_init(dev_init_module);
module_exit(dev_cleanup_module);
MODULE_AUTHOR("Morten Mossige, University of Stavanger");
MODULE_DESCRIPTION("Sample Linux devicedriver");
MODULE_LICENSE("GPL");
