#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/device.h>
#include <linux/netdevice.h>
#include <linux/string.h>

#include "test_kmod.h"
#include "test_version.h"
#include "test_ioctl.h"

static struct tkmod_device g_device; /**< device global variable */

static int tkmod_open(struct inode *inode, struct file  *filp)
{
	int rc;
	struct tkmod_device *dev = &g_device;

	if(down_trylock(&dev->connection_lock))
		return -EBUSY;

	rc = nonseekable_open(inode, filp);
	if (rc != 0) {
		ERR_MESS("tkmod file open error. Descriptor: 0x%p, Error: %d.", filp, rc);
		goto oerror;
	}

	DEBUG_MESS("tkmod opened. Descriptor: 0x%p.", filp);
	return 0;
	oerror:
	return rc;
}

static int tkmod_release(struct inode *inode, struct file  *filp)
{
	struct tkmod_device *dev = &g_device;
	up(&dev->connection_lock);
	DEBUG_MESS("tkmod_ 0x%p closed successfuly.", filp);
	return 0;
}

static long tkmod_ioctl(struct file  *filp, unsigned int  num, unsigned long param)
{
	struct net_device *ndev = NULL;
	char *tbuf = NULL, *pudata = (char *)param;
	int rc = 0, tsize = 0;

	switch (num) {
	case TKMOD_IOCTL_GET_VER:
		DEBUG_MESS("ITKMOD_IOCTL_GET_VER request.");
		return DRIVER_VERSION;
	case TKMOD_IOCTL_GET_DATA:
		DEBUG_MESS("TKMOD_IOCTL_GET_DATA request.");
		tbuf = kmalloc(128, GFP_KERNEL);
		ndev = first_net_device( &init_net );
		while ( ndev ) {
			INFO_MESS(
					"name = %6s\n",	ndev->name);
			memset(tbuf, 0x0, 128);
			sprintf(tbuf,
					"name = %6s\n", ndev->name);
			tsize = strnlen(tbuf, 128);
			if(copy_to_user((void __user *)pudata, (const void*)tbuf, tsize)) {
				kfree(tbuf);
				return -EFAULT;
			}
			pudata += tsize;
			ndev = next_net_device( ndev );
		}
		kfree(tbuf);
		break;
	default:
		WARN_MESS("Unknown IOCTL called.");
		rc = -EBADRQC;
		break;
	}

	if (rc != 0)
		ERR_MESS("IOCTL error %d.", rc);

	return rc;
}

/**
 * File operations structure for char device
 */
static const struct file_operations tkmod_fops = {
		.owner          = THIS_MODULE,
		.unlocked_ioctl = tkmod_ioctl,
		.open           = tkmod_open,
		.release        = tkmod_release,
		.llseek         = no_llseek
};

static int __init tkmod_init(void)
{
	int rc;
	struct tkmod_device *tDev = &g_device;

	INFO_MESS("== Module init ==");

	/* Get a major number */
	rc = alloc_chrdev_region(&tDev->device_number, 0, 1, TKMOD_MODULE_NAME);
	if (rc < 0) {
		ERR_MESS("Unable to allocate major, err: %d\n", rc);
		goto allocchrdev_failed;
	}
	tDev->tMajor = MAJOR(tDev->device_number);

	cdev_init(&tDev->cdev, &tkmod_fops);

	tDev->cdev.owner = THIS_MODULE;
	rc = cdev_add(&tDev->cdev, tDev->device_number, 1);
	if (rc < 0) {
		ERR_MESS("Unable to add char device, err: %d\n", rc);
		goto cdevadd_fail;
	}

	sema_init(&(tDev->connection_lock), 1);

	tDev->class = class_create(THIS_MODULE, TKMOD_MODULE_NAME);
	device_create(tDev->class, NULL,
			tDev->device_number,
			NULL, TKMOD_MODULE_NAME);

	return 0;

	cdevadd_fail:
	unregister_chrdev_region(tDev->device_number, 1);

	allocchrdev_failed:
	INFO_MESS("== Initialization error ==");
	return rc;
}

static void __exit tkmod_exit(void)
{
	struct tkmod_device *tDev = &g_device;
	INFO_MESS("== Module exit ==");

	device_destroy(tDev->class, tDev->device_number);
	class_destroy(tDev->class);

	cdev_del(&tDev->cdev);
	unregister_chrdev_region(tDev->device_number, 1);
}

module_init(tkmod_init);
module_exit(tkmod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Propheta13 <propheta13@gmail.com>");
MODULE_DESCRIPTION("Test module driver.");
