#ifdef 	COCOS_INTEGRATION
  #include "cocos2d.h"
#else
  #include <stdio.h>  // vsprintf...
#endif // COCOS_INTEGRATION

#include "WwiseWrapper.h"
#include <AK/SoundEngine/Common/AkTypes.h>
#include <wchar.h>

#ifdef AK_IOS
  #include "SoundInput.h"
#endif // AK_IOS
#ifndef AK_OPTIMIZED
  #include <AK/Comm/AkCommunication.h> // Communication between Wwise and the game (excluded in release build)
#endif // AK_OPTIMIZED
#ifdef AK_MOTION
  #include <AK/MotionEngine/Common/AkMotionEngine.h>	// Motion Engine (required only for playback of Motion objects)
#endif // AK_MOTION

#ifdef AK_ANDROID
    #include "android/asset_manager.h"
    #define  LOG_TAG    "WwiseWrapper"
    #define __AK_OSCHAR_SNPRINTF 	snprintf
    #define  LOGAK(...)  		__android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
    void LOGAKW(AkOSChar* _Buffer)  
    {
	char szBuff[1024] = {0};
	AKPLATFORM::AkWideCharToChar((const wchar_t*)_Buffer, 1024, szBuff);
	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, (const char*)&szBuff);
    }
#elif WIN32
    #define __AK_OSCHAR_SNPRINTF    	_snwprintf
    void LOGAK(char *format, ...)
    {
        char buffer[1000];
        
        va_list argptr;
        va_start(argptr, format);
        vsprintf(buffer, format, argptr);
        va_end(argptr);
        OutputDebugStringA(buffer);
    }
    void LOGAKW(TCHAR *format, ...)
    {
        TCHAR buffer[1000];
  
        va_list argptr;
        va_start(argptr, format);
        wvsprintf(buffer, format, argptr);
        va_end(argptr);
        OutputDebugString(buffer);
    }
#elif defined(AK_APPLE) //__APPLE__
    //#define __AK_OSCHAR_SNPRINTF
    //#define  LOGAK                      CCLOG
    //#define  LOGAKW                     CCLOG
#else
    #error undefied platform
#endif

// Ak core libs
#if defined(WIN32)
#pragma comment( lib, "AkMemoryMgr")
#pragma comment( lib, "AkSoundEngine")
#pragma comment( lib, "AkStreamMgr")
#pragma comment( lib, "AkMusicEngine")
#pragma comment( lib, "dxguid.lib")	//DirectSound
#ifdef _DEBUG    
#pragma comment( lib, "CommunicationCentral")
#endif
#endif

// plugins
#define AK_PLUGINS
#include <AK/Plugin/AllPluginsRegistrationHelpers.h>	// Plug-ins
#if defined(WIN32)
#pragma comment( lib, "AkConvolutionReverbFX.lib")
#pragma comment( lib, "AkFlangerFX.lib")
#pragma comment( lib, "AkTremoloFX.lib")
#pragma comment( lib, "AuroHeadphoneFX.lib")
#pragma comment( lib, "IOSONOProximityMixer.lib")
#pragma comment( lib, "AkMotionGenerator.lib")
#pragma comment( lib, "AkSineSource.lib")
#pragma comment( lib, "AkSoundSeedWind.lib")
#pragma comment( lib, "AkStereoDelayFX.lib")
#pragma comment( lib, "AkGuitarDistortionFX.lib")
#pragma comment( lib, "AkRumble.lib")
#pragma comment( lib, "AkSilenceSource.lib")
#pragma comment( lib, "AuroPannerMixer.lib")
#pragma comment( lib, "AkPeakLimiterFX.lib")
#pragma comment( lib, "iZTrashBoxModelerFX.lib")
#pragma comment( lib, "AkSoundSeedImpactFX.lib")
#pragma comment( lib, "AkRoomVerbFX.lib")
#pragma comment( lib, "iZTrashDelayFX.lib")
#pragma comment( lib, "McDSPLimiterFX.lib")
#pragma comment( lib, "McDSPFutzBoxFX.lib")
#pragma comment( lib, "AkParametricEQFX.lib")
#pragma comment( lib, "AkToneSource.lib")
#pragma comment( lib, "AkMatrixReverbFX.lib")
#pragma comment( lib, "AkCompressorFX.lib")
#pragma comment( lib, "AkAudioInputSource.lib")
#pragma comment( lib, "AkSoundSeedWoosh.lib")
#pragma comment( lib, "CrankcaseAudioREVModelPlayerFX.lib")
#pragma comment( lib, "iZTrashMultibandDistortionFX.lib")
#pragma comment( lib, "iZHybridReverbFX.lib")
#pragma comment( lib, "AkPitchShifterFX.lib")
#pragma comment( lib, "iZTrashDynamicsFX.lib")
#pragma comment( lib, "iZTrashDistortionFX.lib")
#pragma comment( lib, "AkDelayFX.lib")
#pragma comment( lib, "AkGainFX.lib")
#pragma comment( lib, "AkVorbisDecoder.lib")
#pragma comment( lib, "AkMeterFX.lib")
#pragma comment( lib, "iZTrashFiltersFX.lib")
#pragma comment( lib, "AkSynthOne.lib")
#pragma comment( lib, "AkMP3Source.lib")
#pragma comment( lib, "AkHarmonizerFX.lib")
#pragma comment( lib, "AkTimeStretchFX.lib")
#pragma comment( lib, "AkExpanderFX.lib")
#pragma comment( lib, "Msacm32.lib") // Microsoft ACM Library
#endif

static const AkUInt32 kMaxNumPools = 20;
static const AkUInt32 kDefaultPoolSize = 2 * 1024 * 1024;
static const AkUInt32 kLEngineDefaultPoolSize = 1 * 1024 * 1024;

#ifdef AK_WIN
  #ifdef _DEBUG
    #define MY_SOUND_BANK_PATH L"../../WwiseProject/GeneratedSoundBanks/Windows/"
  #else
    #define MY_SOUND_BANK_PATH L"Wwise/"
  #endif
#elif defined AK_ANDROID
  const AkOSChar MY_SOUND_BANK_PATH[] = AKTEXT("GeneratedSoundBanks/Android/");
#elif defined AK_APPLE
  #include "TargetConditionals.h"
  #if TARGET_OS_IPHONE
    // iOS
    #define MY_SOUND_BANK_PATH g_szBasePath //"iOS/"
  #else
    // MacOS
    #define MY_SOUND_BANK_PATH "Mac/"
  #endif
#else
  #error undefied platform
#endif

#ifdef AK_IOS
AkAudioSessionCategoryOptions GetAudioSessionCategoryOptionBitMask(bool in_bMixWithOthers, bool in_bDuckOthers, bool in_bAllowBluetooth, bool in_bDefaultToSpeaker)
{
    AkInt32 mask = 0;
    if (in_bMixWithOthers)
    {
        mask |= (1 << 0);
    }
    else
    {
        mask |= (0 << 0);
    }
    
    if (in_bDuckOthers)
    {
        mask |= (1 << 1);
    }
    else
    {
        mask |= (0 << 1);
    }
    
    if (in_bAllowBluetooth)
    {
        mask |= (1 << 2);
    }
    else
    {
        mask |= (0 << 2);
    }
    
    if (in_bDefaultToSpeaker)
    {
        mask |= (1 << 3);
    }
    else
    {
        mask |= (0 << 3);
    }
    
    return (AkAudioSessionCategoryOptions) mask;
}

static AKRESULT DemoInterruptCallback(bool in_bEnterInterruption, void* in_pCookie )
{
    // Be careful with any pause/resume events posted in this callback, along with those
    // posted in other user callbacks. The interruption and source change notifications
    // on iOS can happen on top of each other, causing some of the events to miss being
    // processed.
    // E.g., There is no need for pause/resume here because suspend/wakeup will be called
    // in the sound engine, taking care of pauses/resumes automatically.
    if (in_bEnterInterruption)
    {
        //        NSLog(@"IntegrationDemo: Interruption: User callback, before interruption happens.");
    }
    else
    {
        // This case is called after the user dismisses the interruption from phone calls or alarms.
        // However, it is not called if the interruption is from remote-controlled music app.
        // It is handled by source change callback.
        //       NSLog(@"IntegrationDemo: Interruption: User callback, after interruption is done.");
    }
    
    return AK_Success;
}
#endif

// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{
#ifndef AK_MAC
    void * AllocHook(size_t in_size)
    {
	return malloc(in_size);
    }
    void FreeHook(void * in_ptr)
    {
	free(in_ptr);
    }
#endif
#ifdef WIN32
    // Note: VirtualAllocHook() may be used by I/O pools of the default implementation
    // of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
    // - refer to the Windows SDK documentation for more details). This is NOT mandatory;
    // you may implement it with a simple malloc().
    void * VirtualAllocHook(
	void * in_pMemAddress,
	size_t in_size,
	AkUInt32 in_dwAllocationType,
	AkUInt32 in_dwProtect
	)
    {
	return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
    }

    void VirtualFreeHook(
	void * in_pMemAddress,
	size_t in_size,
	AkUInt32 in_dwFreeType
	)
    {
	VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
    }
#endif
}

namespace {
    //	Wwise		    m_wwise;
    AkMemSettings	    m_memSettings;
    AkStreamMgrSettings	    m_stmSettings;
    AkDeviceSettings	    m_deviceSettings;
    AkInitSettings	    m_initSettings;
    AkPlatformInitSettings  m_platformInitSettings;
    AkMusicSettings	    m_musicInit;
    AkOSChar		    m_strError[1024];
}

namespace WWISE {
    bool initialize(void* mgr)
    {
	Wwise::Instance().GetDefaultSettings(m_memSettings, m_stmSettings, m_deviceSettings, m_initSettings, m_platformInitSettings, m_musicInit);

#ifdef AK_ANDROID
    JavaVM* jvm = cocos2d::JniHelper::getJavaVM();
    if (!jvm)
    {
        LOGAK("<Wwise::Init> Failed to get JVM. Aborted.");
        return false;
    }
    m_platformInitSettings.pJavaVM = jvm;
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
    m_platformInitSettings.jNativeActivity = env->CallStaticObjectMethod(classID, methodID);
    Wwise::Instance().GetLowLevelIOHandler()->InitAndroidIO(m_platformInitSettings.pJavaVM, m_platformInitSettings.jNativeActivity);
#endif
	if (!Wwise::Instance().Init(m_memSettings, m_stmSettings, m_deviceSettings, m_initSettings, m_platformInitSettings, m_musicInit,
	    (AkOSChar*)MY_SOUND_BANK_PATH, m_strError, sizeof(m_strError))) {
	    LOGAKW(m_strError);
	    abort();
	}
	return true;
    }
    void terminate()
    {
	Wwise::Instance().Term();
    }
    void update()
    {
	AK::SoundEngine::RenderAudio();
    }
}

Wwise& Wwise::Instance()
{
    static Wwise wwise;
    return wwise;
}

Wwise::Wwise() {
    m_pLowLevelIO = new CAkFilePackageLowLevelIOBlocking();
}

Wwise::~Wwise()
{
    delete m_pLowLevelIO;
    m_pLowLevelIO = NULL;
}

CAkFilePackageLowLevelIOBlocking& Wwise::IOManager()
{
    return *m_pLowLevelIO;
}

bool Wwise::Init(   AkMemSettings&          in_memSettings,
		    AkStreamMgrSettings&    in_stmSettings,
		    AkDeviceSettings&       in_deviceSettings,
		    AkInitSettings&         in_initSettings,
		    AkPlatformInitSettings& in_platformInitSettings,
		    AkMusicSettings&        in_musicInit,
		    AkOSChar*               in_soundBankPath,
		    AkOSChar*               in_szErrorBuffer,
		    unsigned int            in_unErrorBufferCharCount)
{
    bool bSuccess;
    AKRESULT hr;
    
#if  defined(AK_IOS)
    // Allow other applications to play sounds while the integration demo executes.
    in_platformInitSettings.audioSession.eCategory = AkAudioSessionCategoryPlayAndRecord;
    //	platformInitSettings.audioSession.eCategory = AkAudioSessionCategorySoloAmbient;
    //	platformInitSettings.audioSession.eCategory = AkAudioSessionCategoryAmbient;
    
    if (in_platformInitSettings.audioSession.eCategory == AkAudioSessionCategoryPlayAndRecord)
    {
        g_bEnableMicDemo = true;
        in_platformInitSettings.audioSession.eCategoryOptions = GetAudioSessionCategoryOptionBitMask(true, false, true, true);
    }
    else
    {
        g_bEnableMicDemo = false;
        in_platformInitSettings.audioSession.eCategoryOptions = GetAudioSessionCategoryOptionBitMask(true, true, false, false);
    }
    
    // Optimization.
    in_platformInitSettings.uSampleRate = AUDIO_SAMPLE_RATE;
    g_uSamplesPerFrame = in_initSettings.uNumSamplesPerFrame;
    
    in_platformInitSettings.audioCallbacks.inputCallback = SoundInput::AudioInputCallback;
    in_platformInitSettings.audioCallbacks.inputCallbackCookie = (void*)&SoundInput::Instance();
    
    in_platformInitSettings.audioCallbacks.interruptionCallback = DemoInterruptCallback;
#endif
    
    // Initialize Wwise
    bSuccess = InitWwise(in_memSettings, in_stmSettings, in_deviceSettings, in_initSettings, in_platformInitSettings, in_musicInit, in_szErrorBuffer, in_unErrorBufferCharCount);
    if (!bSuccess) {
	goto cleanup;
    }

    // Set the path to the SoundBank Files.
    hr = m_pLowLevelIO->SetBasePath(in_soundBankPath);

    // Set global language. Low-level I/O devices can use this string to find language-specific assets.
    if (AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)")) != AK_Success)
    {
	goto cleanup;
    }

    AkBankID bankID;
    if (AK::SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID) != AK_Success)
    {
	LOGAK("<Wwise::Init> Cannot load Init.bnk! error");

#ifdef 	COCOS_INTEGRATION
	cocos2d::MessageBox("Cannot load Init.bnk!", "Error");
#endif
	return false;
    }

    return true;

cleanup:
    Term();
    return false;
}

void Wwise::Term()
{
    TermWwise();
}

void Wwise::GetDefaultSettings(	AkMemSettings&          out_memSettings,
				AkStreamMgrSettings&    out_stmSettings,
				AkDeviceSettings&       out_deviceSettings,
				AkInitSettings&         out_initSettings,
				AkPlatformInitSettings& out_platformInitSettings,
				AkMusicSettings&        out_musicInit)
{
    out_memSettings.uMaxNumPools = kMaxNumPools;
    AK::StreamMgr::GetDefaultSettings(out_stmSettings);

    AK::StreamMgr::GetDefaultDeviceSettings(out_deviceSettings);

    AK::SoundEngine::GetDefaultInitSettings(out_initSettings);
    out_initSettings.uDefaultPoolSize = kDefaultPoolSize;

    AK::SoundEngine::GetDefaultPlatformInitSettings(out_platformInitSettings);
    out_platformInitSettings.uLEngineDefaultPoolSize = kLEngineDefaultPoolSize;

    AK::MusicEngine::GetDefaultInitSettings(out_musicInit);
}

const bool Wwise::GetCommunicationEnabled() {
#if !defined AK_OPTIMIZED && !defined INTEGRATIONDEMO_DISABLECOMM
    return true;
#else
    return false;
#endif
}

bool Wwise::InitWwise(	AkMemSettings&          in_memSettings,
			AkStreamMgrSettings&    in_stmSettings,
			AkDeviceSettings&       in_deviceSettings,
			AkInitSettings&         in_initSettings,
			AkPlatformInitSettings& in_platformInitSettings,
			AkMusicSettings&        in_musicInit,
			AkOSChar*               in_szErrorBuffer,
			unsigned int            in_unErrorBufferCharCount)
{
    //
    // Create and initialize an instance of the default memory manager. Note
    // that you can override the default memory manager with your own. Refer
    // to the SDK documentation for more information.
    //

    AKRESULT res = AK::MemoryMgr::Init(&in_memSettings);
    if (res != AK_Success)
    {
	__AK_OSCHAR_SNPRINTF(in_szErrorBuffer, in_unErrorBufferCharCount, AKTEXT("AK::MemoryMgr::Init() returned AKRESULT %d"), res);
	LOGAKW(in_szErrorBuffer);
	return false;
    }

    //
    // Create and initialize an instance of the default streaming manager. Note
    // that you can override the default streaming manager with your own. Refer
    // to the SDK documentation for more information.
    //

    // Customize the Stream Manager settings here.

    if (!AK::StreamMgr::Create(in_stmSettings))
    {
	AKPLATFORM::SafeStrCpy(in_szErrorBuffer, AKTEXT("AK::StreamMgr::Create() failed"), in_unErrorBufferCharCount);
	LOGAKW(in_szErrorBuffer);
	return false;
    }

    //
    // Create a streaming device with blocking low-level I/O handshaking.
    // Note that you can override the default low-level I/O module with your own. Refer
    // to the SDK documentation for more information.
    //

    // CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
    // in the Stream Manager, and registers itself as the File Location Resolver.
    res = m_pLowLevelIO->Init(in_deviceSettings);
    if (res != AK_Success)
    {
	__AK_OSCHAR_SNPRINTF(in_szErrorBuffer, in_unErrorBufferCharCount, AKTEXT("m_lowLevelIO.Init() returned AKRESULT %d"), res);
	LOGAKW(in_szErrorBuffer);
	return false;
    }

    //
    // Create the Sound Engine
    // Using default initialization parameters
    //

    res = AK::SoundEngine::Init(&in_initSettings, &in_platformInitSettings);
    if (res != AK_Success)
    {
	__AK_OSCHAR_SNPRINTF(in_szErrorBuffer, in_unErrorBufferCharCount, AKTEXT("AK::SoundEngine::Init() returned AKRESULT %d"), res);
	LOGAKW(in_szErrorBuffer);
	return false;
    }

    //
    // Initialize the music engine
    // Using default initialization parameters
    //

    res = AK::MusicEngine::Init(&in_musicInit);
    if (res != AK_Success)
    {
	__AK_OSCHAR_SNPRINTF(in_szErrorBuffer, in_unErrorBufferCharCount, AKTEXT("AK::MusicEngine::Init() returned AKRESULT %d"), res);
	LOGAKW(in_szErrorBuffer);
	return false;
    }

#if defined(NDK_DEBUG) || defined(AK_DEBUG) || defined(_DEBUG)
    //
    // Initialize communications (not in release build!)
    //
    if (GetCommunicationEnabled()) {
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	res = AK::Comm::Init(commSettings);
	if (res != AK_Success)
	{
	    __AK_OSCHAR_SNPRINTF(in_szErrorBuffer, in_unErrorBufferCharCount, AKTEXT("AK::Comm::Init() returned AKRESULT %d. Communication between the Wwise authoring application and the game will not be possible."), res);
	    LOGAKW(in_szErrorBuffer);
	}
    }
#endif

#ifdef AK_PLUGINS
    //
    // Register plugins
    /// Note: This a convenience method for rapid prototyping.
    /// To reduce executable code size register/link only the plug-ins required by your game
    res = AK::SoundEngine::RegisterAllPlugins();
    if (res != AK_Success)
    {
	__AK_OSCHAR_SNPRINTF(in_szErrorBuffer, in_unErrorBufferCharCount, AKTEXT("AK::SoundEngine::RegisterAllPlugins() returned AKRESULT %d."), res);
    }
#endif
    
    return true;
}

void Wwise::TermWwise()
{
//#ifndef AK_OPTIMIZED
#if defined(NDK_DEBUG) || defined(AK_DEBUG) || defined(_DEBUG)
    // Terminate communications between Wwise and the game
    if (GetCommunicationEnabled()) {
	AK::Comm::Term();
    }
#endif
    // Terminate the music engine
    AK::MusicEngine::Term();

    // Terminate the sound engine
    if (AK::SoundEngine::IsInitialized())
    {
	AK::SoundEngine::Term();
    }

    // Terminate the streaming device and streaming manager
    // CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device 
    // that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
    if (AK::IAkStreamMgr::Get())
    {
	m_pLowLevelIO->Term();
	AK::IAkStreamMgr::Get()->Destroy();
    }

    // Terminate the Memory Manager
    if (AK::MemoryMgr::IsInitialized())
    {
	AK::MemoryMgr::Term();
    }
}
