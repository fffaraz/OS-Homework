/*
 * In The Name Of God
 * ========================================
 * [] File Name : main.c
 *
 * [] Creation Date : 20-01-2015
 *
 * [] Last Modified : Tue 20 Jan 2015 08:42:39 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>       /* printk() */
#include <linux/fs.h>           /* everything... */
#include <linux/errno.h>        /* error codes */
#include <linux/types.h>        /* size_t */
#include <linux/fcntl.h>        /* O_ACCMODE */
#include <asm/uaccess.h>        /* copy_*_user */

#include "fifo.h"              /* local definitions */

/*
 * Parameters which can be set at load time.
 */
int fifo_major = FIFO_MAJOR;

MODULE_AUTHOR("Parham Alvani");
MODULE_LICENSE("GPL");

static char queue[MAX_BUFF];
static int head;
static int tail;

/*
 * Open and close
*/
static int fifo_open(struct inode *inode, struct file *filp)
{
	return 0;          /* success */
}

static int fifo_release(struct inode *inode, struct file *flip)
{
	return 0;
}

/*
 * Data management: read and write
*/
static ssize_t fifo_read(struct file *filp, char __user *buf,
		size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;

	if (*f_pos != 0)
		return -EINVAL;
	if (tail - head < count)
		count = tail - head;

	if (copy_to_user(buf, queue + head, count)) {
		retval = -EFAULT;
		goto out;
	}
	head += count;
	retval = count;

out:
	return retval;
}

static ssize_t fifo_write(struct file *filp, const char __user *buf,
		size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;

	if (copy_from_user(queue + tail, buf, count)) {
		retval = -EFAULT;
		goto out;
	}
	tail += count;
	retval = count;
out:
	return retval;
}

static const struct file_operations fifo_fops = {
	.owner =    THIS_MODULE,
	.read =     fifo_read,
	.write =    fifo_write,
	.open =     fifo_open,
	.release =  fifo_release
};

/*
 * Finally, the module stuff
*/

/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
*/
void __exit fifo_cleanup_module(void)
{
	/* cleanup_module is never called if registering failed */
	unregister_chrdev(fifo_major, "fifo");
}

int __init fifo_init_module(void)
{
	head = 0;
	tail = 0;


	/*
	 * Get a range of minor numbers to work with, asking for a dynamic
	 * major unless directed otherwise at load time.
	*/
	fifo_major = register_chrdev(fifo_major, "fifo", &fifo_fops);
	if (fifo_major < 0) {
		return fifo_major;
	}
	return 0;
}

module_init(fifo_init_module);
module_exit(fifo_cleanup_module);
