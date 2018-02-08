#pragma once

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/AkSoundEngine.h>	// Sound engine
#include "AkFilePackageLowLevelIOBlocking.h"



class WwisePlatformHelper
{
public:
    static WwisePlatformHelper& Instance();

    bool ConfigurePlatform(AkPlatformInitSettings& platformInitSettings, CAkFilePackageLowLevelIOBlocking* pLowLevelIO=NULL);
    
    bool ConfigureSoundBankPaths(CAkFilePackageLowLevelIOBlocking* pLowLevelIO);
    
private:
    WwisePlatformHelper() {}
    WwisePlatformHelper( WwisePlatformHelper const& ){};
    WwisePlatformHelper& operator=( WwisePlatformHelper const& );
    ~WwisePlatformHelper() {}


};
