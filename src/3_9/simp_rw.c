/*****************************************************************************

DESCRIPTION Skeleton of the rw-driver

*****************************************************************************/


/*--------------------  I n c l u d e   F i l e s  -------------------------*/

#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO
#include <linux/init.h>		// included for __init and __exit macros
#include <linux/fs.h>
#include <linux/uaccess.h>

/*--------------------  C o n s t a n t s  ---------------------------------*/

#define DEV_MAJOR 0		//Means to use a dynamic 
#define DEV_NAME "simp_rw"

#define SUCCESS 0

/*--------------------  T y p e s  -----------------------------------------*/

/*--------------------  V a r i a b l e s  ---------------------------------*/

static int major_number;
static bool msgWritten;
static char msg[50];

/*--------------------  F u n c t i o n s  ---------------------------------*/

// open function - called when the "file" /dev/simp_rw is opened in userspace
static int dev_open (struct inode *inode, struct file *file) 
{
	msgWritten = false;
	printk("simplkm_rw: skeleton device driver open\n");
	return 0;
}


// close function - called when the "file" /dev/simp_rw is closed in userspace  
static int dev_release (struct inode *inode, struct file *file)
{
	printk("simplkm_rw: skeleton device driver closed\n");
	return 0;
}

// read function called when from /dev/simp_rw is read
static ssize_t dev_read (struct file *file, char *buf, size_t count, loff_t *ppos) 
{
	if(msgWritten) {
		return 0;
	} 
	msgWritten = true;
	int bufLen=0;

	bufLen = strlen( msg );
	
	printk("simplkm_rw: %s, buflen=%d\n", msg, bufLen );
			
	if( copy_to_user( buf, msg,  bufLen ) )
	{
		printk("simplkm_rw: copy_to_user failed\n" );
		return -EFAULT;
	}
	else {
		sprintf(msg, "");
		return bufLen;
	}
}

static ssize_t dev_write (struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	sprintf(msg, "");
	int bufLen=0;

	if( copy_from_user( msg, buf, count ) )
	{
		printk("simplkm_rw: copy_from_user failed\n");
		return -EFAULT;
	}
	else
	{
		printk(KERN_INFO "%s\n", msg);
		sprintf(msg + strlen(msg), "\n");
		bufLen = strlen( msg );
		return bufLen;
	}
}

// define which file operations are supported
struct file_operations dev_fops = 
{
	.owner	=	THIS_MODULE,
	.llseek	=	NULL,
	.read		=	dev_read,
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

		
	printk("simplkm_rw: dev_cleanup_module Device Driver Removed\n");
}

module_init(dev_init_module);
module_exit(dev_cleanup_module);
MODULE_AUTHOR("Stian Sandve, University of Stavanger");
MODULE_DESCRIPTION("Sample Linux devicedriver");
MODULE_LICENSE("GPL");
