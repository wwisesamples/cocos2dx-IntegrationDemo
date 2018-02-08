#include "SoundInput.h"

#include <AK/SoundEngine/Common/AkTypes.h>

#include "Platform.h"
#include "PlatformCocos.h"

#include "WwisePlatformHelper.h"



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

WwisePlatformHelper& WwisePlatformHelper::Instance()
{
    static WwisePlatformHelper wph;
    return wph;
}


bool WwisePlatformHelper::ConfigurePlatform(AkPlatformInitSettings& platformInitSettings, CAkFilePackageLowLevelIOBlocking* pLowLevelIO/*NULL*/)
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

bool WwisePlatformHelper::ConfigureSoundBankPaths(CAkFilePackageLowLevelIOBlocking* pLowLevelIO)
{
    // Set the path to the SoundBank Files.
    if (pLowLevelIO->SetBasePath(SOUND_BANK_PATH) != AK_Success) 
    {
        return false;
    }
    
    if (pLowLevelIO->AddBasePath(DOCUMENTS_PATH) != AK_Success)
    {
        return false;
    }
   
    return true;

}
