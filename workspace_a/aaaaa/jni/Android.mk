LOCAL_PATH := $(call my-dir)  
include $(CLEAR_VARS)  
LOCAL_LDLIBS := -llog  
LOCAL_MODULE    := ndk1  
LOCAL_SRC_FILES := native.C  
include $(BUILD_SHARED_LIBRARY) 
