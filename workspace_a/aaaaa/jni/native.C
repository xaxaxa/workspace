#include <jni.h>
#include <string.h>
#include <android/log.h>
#define DEBUG_TAG "NDK_AndroidNDK1SampleActivity"
extern "C" {
JNIEXPORT void JNICALL Java_com_example_aaaaa_MainActivity_helloLog(JNIEnv * env, jobject th,
		jstring logThis) {
	jboolean isCopy;
	const char * szLogThis = env->GetStringUTFChars(logThis, &isCopy);
	__android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "NDK:LC: [%s]",
			szLogThis);
	env->ReleaseStringUTFChars(logThis, szLogThis);
}
JNIEXPORT void JNICALL Java_com_example_aaaaa_MainActivity_sssss(JNIEnv * env, jobject th,
		jint id) {

}
}
