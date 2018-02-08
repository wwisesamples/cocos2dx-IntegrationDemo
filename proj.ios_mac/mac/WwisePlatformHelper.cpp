#include <AK/SoundEngine/Common/AkTypes.h>

// #include "Platform.h"
#include "PlatformCocos.h"

#include "WwisePlatformHelper.h"


WwisePlatformHelper& WwisePlatformHelper::Instance()
{
    static WwisePlatformHelper wph;
    return wph;
}


bool WwisePlatformHelper::ConfigurePlatform(AkPlatformInitSettings& platformInitSettings, CAkFilePackageLowLevelIOBlocking* pLowLevelIO/*NULL*/)
{
    // Add your code here.

    return true;
}

bool WwisePlatformHelper::ConfigureSoundBankPaths(CAkFilePackageLowLevelIOBlocking* pLowLevelIO)
{
    // Set the path to the SoundBank Files.
    if (pLowLevelIO->SetBasePath(SOUND_BANK_PATH) != AK_Success) 
    {
        return false;
    }
   
    return true;

}