#include <wchar.h>

#include "cocos2d.h"

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Plugin/AllPluginsFactories.h>

#include "SoundInput.h"

#ifndef AK_OPTIMIZED
  #include <AK/Comm/AkCommunication.h> // Communication between Wwise and the game (excluded in release build)
#endif // AK_OPTIMIZED
#ifdef AK_MOTION
  #include <AK/MotionEngine/Common/AkMotionEngine.h>	// Motion Engine (required only for playback of Motion objects)
#endif // AK_MOTION

#include "Platform.h"
#include "PlatformCocos.h"

#include "WwiseWrapper.h"


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


Wwise& Wwise::Instance()
{
    static Wwise wwise;
    return wwise;
}

Wwise::Wwise() 
{
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

bool Wwise::Init()
{
    AkMemSettings       memSettings;
    AkStreamMgrSettings     stmSettings;
    AkDeviceSettings        deviceSettings;
    AkInitSettings      initSettings;
    AkPlatformInitSettings  platformInitSettings;
    AkMusicSettings     musicInit;
    AkOSChar            strError[1024];

    GetDefaultSettings(memSettings, stmSettings, deviceSettings, initSettings, platformInitSettings, musicInit);

    if (!ConfigurePlatform(platformInitSettings))
    {
        return false;
    }

    bool bSuccess = false;
    
    bSuccess = InitWwise(memSettings, stmSettings, deviceSettings, initSettings, platformInitSettings, musicInit, strError, sizeof(strError));
    if (!bSuccess) {
        LOGAKW(strError);
        goto cleanup;
    }

    // Set the path to the SoundBank Files.
    if (m_pLowLevelIO->SetBasePath(SOUND_BANK_PATH) != AK_Success) 
    {
        goto cleanup;
    }

    //
    // TODO: AddBasePath() for mobile
    //

    // Set global language. Low-level I/O devices can use this string to find language-specific assets.
    if (AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)")) != AK_Success)
    {
        goto cleanup;
    }

    AkBankID bankID;
    if (AK::SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID) != AK_Success)
    {
        LOGAK("<Wwise::Init> Cannot load Init.bnk! error");
        cocos2d::MessageBox("Cannot load Init.bnk!", "Error");
        goto cleanup;
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

void Wwise::Update()
{
    AK::SoundEngine::RenderAudio();
}


//
// Private Methods
//

void Wwise::GetDefaultSettings( AkMemSettings&          out_memSettings,
                AkStreamMgrSettings&    out_stmSettings,
                AkDeviceSettings&       out_deviceSettings,
                AkInitSettings&         out_initSettings,
                AkPlatformInitSettings& out_platformInitSettings,
                AkMusicSettings&        out_musicInit)
{
    out_memSettings.uMaxNumPools = 20;
    AK::StreamMgr::GetDefaultSettings(out_stmSettings);

    AK::StreamMgr::GetDefaultDeviceSettings(out_deviceSettings);

    AK::SoundEngine::GetDefaultInitSettings(out_initSettings);
    out_initSettings.uDefaultPoolSize = 2 * 1024 * 1024;

    AK::SoundEngine::GetDefaultPlatformInitSettings(out_platformInitSettings);
    out_platformInitSettings.uLEngineDefaultPoolSize = 1 * 1024 * 1024;

    AK::MusicEngine::GetDefaultInitSettings(out_musicInit);
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
	AkCommSettings commSettings;
    AK::Comm::GetDefaultInitSettings( commSettings );
    AKPLATFORM::SafeStrCpy(commSettings.szAppNetworkName, "Cocos2d-x Integration Demo", AK_COMM_SETTINGS_MAX_STRING_SIZE);
    res = AK::Comm::Init( commSettings );
	if (res != AK_Success)
	{
	    __AK_OSCHAR_SNPRINTF(in_szErrorBuffer, in_unErrorBufferCharCount, AKTEXT("AK::Comm::Init() returned AKRESULT %d. Communication between the Wwise authoring application and the game will not be possible."), res);
	    LOGAKW(in_szErrorBuffer);
	}
#endif

    AK::SoundEngine::RegisterGameObj(LISTENER_ID, "Listener (Default)");
    AK::SoundEngine::SetDefaultListeners(&LISTENER_ID, 1);
    
    return true;
}

void Wwise::TermWwise()
{
//#ifndef AK_OPTIMIZED
#if defined(NDK_DEBUG) || defined(AK_DEBUG) || defined(_DEBUG)
    // Terminate communications between Wwise and the game
	AK::Comm::Term();
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

bool Wwise::ConfigurePlatform(AkPlatformInitSettings& platformInitSettings)
{
    // Allow other applications to play sounds while the integration demo executes.
    platformInitSettings.audioSession.eCategory = AkAudioSessionCategoryPlayAndRecord;
    //  platformInitSettings.audioSession.eCategory = AkAudioSessionCategorySoloAmbient;
    //  platformInitSettings.audioSession.eCategory = AkAudioSessionCategoryAmbient;
    
    if (platformInitSettings.audioSession.eCategory == AkAudioSessionCategoryPlayAndRecord)
    {
        g_bEnableMicDemo = true;
        platformInitSettings.audioSession.eCategoryOptions = GetAudioSessionCategoryOptionBitMask(true, false, true, true);
    }
    else
    {
        g_bEnableMicDemo = false;
        platformInitSettings.audioSession.eCategoryOptions = GetAudioSessionCategoryOptionBitMask(true, true, false, false);
    }
    
    // Optimization.
    platformInitSettings.uSampleRate = AUDIO_SAMPLE_RATE;
    
    platformInitSettings.audioCallbacks.inputCallback = SoundInput::AudioInputCallback;
    platformInitSettings.audioCallbacks.inputCallbackCookie = (void*)&SoundInput::Instance();
    
    platformInitSettings.audioCallbacks.interruptionCallback = DemoInterruptCallback;

    return true;
}
