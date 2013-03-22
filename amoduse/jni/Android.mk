LOCAL_PATH := $(call my-dir)
 
include $(CLEAR_VARS)
 
LOCAL_MODULE    := ioctlwrap
LOCAL_SRC_FILES := ioctlwrap.c
 
include $(BUILD_SHARED_LIBRARY)