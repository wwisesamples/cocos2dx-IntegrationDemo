#include "AppDelegate.h"
#include "cocos2d.h"
//#include "platform/android/jni/JniHelper.h"
//#include <jni.h>
//#include <android/log.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

void cocos_android_app_init (JNIEnv* env) {
    LOGD("cocos_android_app_init");
    struct android_app* app = (struct android_app*)env;
    AppDelegate *pAppDelegate = new AppDelegate();
}


void android_main(struct android_app* app)
{
    LOGD("android_main");
    //AppDelegate app;
    AppDelegate *pAppDelegate = new AppDelegate();
    cocos2d::Application::getInstance()->run();
}
