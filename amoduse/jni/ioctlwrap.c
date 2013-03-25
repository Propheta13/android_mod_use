#include <stdlib.h>
#include <fcntl.h>

#include "ioctlwrap.h"
#include "../../test_kmod/test_ioctl.h"


const char string[] = "Driver open failed.";
#define BUF_SIZE 4096

JNIEXPORT jstring JNICALL Java_com_propheta13_amoduse_IoctlWrapper_getKData
  (JNIEnv *env, jclass jcl)
{
	char *info_buf = NULL;
	int dfd = 0, rc = 0;

	dfd = open(TKMOD_DEV_PATH, O_RDONLY);

	if(dfd < 0)
	{
		jstring RetString = (*env)->NewStringUTF(env, string);
		goto exit;
	}

	info_buf = malloc(BUF_SIZE);
	rc = ioctl(dfd, TKMOD_IOCTL_GET_DATA, info_buf);
	if(rc < 0)
	{
		strerror_r(rc, info_buf, BUF_SIZE);
	}
	jstring RetString = (*env)->NewStringUTF(env, info_buf);
	free(info_buf);

	close(dfd);

exit:
	return RetString;
}
