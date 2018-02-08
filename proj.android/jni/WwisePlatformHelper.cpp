#include "cocos2d.h"

#include <AK/SoundEngine/Common/AkTypes.h>

#include "Platform.h"
#include "PlatformCocos.h"

#include "WwisePlatformHelper.h"


WwisePlatformHelper& WwisePlatformHelper::Instance()
{
    static WwisePlatformHelper wph;
    return wph;
}


bool WwisePlatformHelper::ConfigurePlatform(AkPlatformInitSettings& platformInitSettings, CAkFilePackageLowLevelIOBlocking* pLowLevelIO/*NULL*/)
{
    JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
    if (!jvm)
    {
        LOGAK("<Wwise::Init> Failed to get JVM. Aborted.");
        return false;
    }

    platformInitSettings.pJavaVM = jvm;
    // Get activity.
    JNIEnv* env = cocos2d::JniHelper::getEnv();
    if (!env)
    {
        LOGAK("<Wwise::Init> Failed to get JNIEnv to retrieve Activity. Aborted.");
        return false;
    }

    jclass classID = env->FindClass("org/cocos2dx/cpp/AppActivity");
    if (!classID)
    {
        LOGAK("<Wwise::Init> Failed to find class AppActivity to retrieve Activity. Aborted.");
        return false;
    }

    jmethodID methodID = env->GetStaticMethodID(classID, "getInstance", "()Ljava/lang/Object;");
    if (!methodID)
    {
        LOGAK("<Wwise::Init> Failed to find method getInstance to retrieve Activity. Aborted.");
        return false;
    }

    platformInitSettings.jNativeActivity = env->CallStaticObjectMethod(classID, methodID);
    if ( pLowLevelIO->InitAndroidIO(platformInitSettings.pJavaVM, platformInitSettings.jNativeActivity) != AK_Success)
    {
        LOGAK("<Wwise::Init> Failed to initialize I/O. Aborted.");
        return false;
    };

    return true;
}

bool WwisePlatformHelper::ConfigureSoundBankPaths(CAkFilePackageLowLevelIOBlocking* pLowLevelIO)
{
    // Set the path to the SoundBank Files.
    if (pLowLevelIO->SetBasePath(SOUND_BANK_PATH) != AK_Success) 
    {
        return false;
    }

    if (pLowLevelIO->AddBasePath("/sdcard") != AK_Success)
    {
        return false;
    }
   
    return true;

}