#include "stdafx.h"
#include <stdio.h>  
#include "PlatformCocos.h"


// Ak core libs
#pragma comment( lib, "AkMemoryMgr")
#pragma comment( lib, "AkSoundEngine")
#pragma comment( lib, "AkStreamMgr")
#pragma comment( lib, "AkMusicEngine")
#pragma comment( lib, "dxguid.lib")	//DirectSound
#ifdef _DEBUG    
#pragma comment( lib, "CommunicationCentral")
#endif

// plugins
#define AK_PLUGINS
#pragma comment( lib, "AkConvolutionReverbFX.lib")
#pragma comment( lib, "AkFlangerFX.lib")
#pragma comment( lib, "AkTremoloFX.lib")
#pragma comment( lib, "AuroHeadphoneFX.lib")
// #pragma comment( lib, "IOSONOProximityMixer.lib") // Removed since 2017.1
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
// #pragma comment( lib, "AkMP3Source.lib") // Removed since 2017.1
#pragma comment( lib, "AkHarmonizerFX.lib")
#pragma comment( lib, "AkTimeStretchFX.lib")
#pragma comment( lib, "AkExpanderFX.lib")
#pragma comment( lib, "AkRecorderFX.lib")
#pragma comment( lib, "AkReflectFX.lib")
#pragma comment( lib, "Msacm32.lib") // Microsoft ACM Library


bool g_isLandscape = true;

// Alloc hook that need to be define by the game
namespace AK
{
	void * AllocHook( size_t in_size )
	{
		return malloc( in_size );
	}
	void FreeHook( void * in_ptr )
	{
		free( in_ptr );
	}

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
}

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