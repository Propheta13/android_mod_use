#ifndef TEST_IOCTL_H
#define TEST_IOCTL_H

#define TKMOD_MODULE_NAME "tkmod_device"
#define TKMOD_DEV_PATH "/dev/tkmod_device"

#include <linux/ioctl.h>

/*
 * IOCTL magic number. Choosed randomly.
 */
#define TKMOD_IOCTL_MAGIC 0xE2

/*
 * IOCTL commands.
 */
#define TKMOD_IOCTL_GET_VER   _IO(TKMOD_IOCTL_MAGIC, 1)
#define TKMOD_IOCTL_GET_DATA  _IOR(TKMOD_IOCTL_MAGIC, 2, char*)

#endif /*TEST_IOCTL_H*/
