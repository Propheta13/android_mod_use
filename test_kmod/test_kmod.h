#ifndef TEST_KMOD_H
#define TEST_KMOD_H

#include <linux/cdev.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/semaphore.h>

/*
 * Debug system macroses.
 */
#define DRIVER_STR "[TEST_KMOD] "
#define ERR_MESS(format, args...)printk(KERN_ERR DRIVER_STR format, ##args)
#define WARN_MESS(format, args...)printk(KERN_WARNING DRIVER_STR format, ##args)
#define INFO_MESS(format, args...)printk(KERN_INFO DRIVER_STR format, ##args)
#define DEBUG_MESS(format, args...)printk(KERN_DEBUG DRIVER_STR format, ##args)

/**
 * tzd_device definition.
 */
struct tkmod_device {
	/* Char device specific data */
	dev_t device_number;
	struct cdev cdev;
	struct class *class;
	int tMajor;

	struct semaphore connection_lock;
};

#endif /*TEST_KMOD_H*/
