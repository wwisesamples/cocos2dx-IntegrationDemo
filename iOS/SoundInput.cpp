//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2012 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <AK/Plugin/AkAudioInputSourceFactory.h>
#include <AK/Tools/Common/AkAutoLock.h>
#include "SoundInput.h"
#include "SoundInputMgr.h"
#if COCOS_INTEGRATION
#include "Platform.h"
#endif
#define INVALID_DEVICE_INDEX -1

SoundInput SoundInput::ms_Instance;

AKRESULT SoundInput::AudioInputCallback(AudioBufferList* io_Data, void* in_pCookie)
{
#if ! TARGET_IPHONE_SIMULATOR
	// Prepare audio data with a ring buffer or buffer queue.
	// Avoid long processing here to minimize the chances of voice starvation (blocking).
	SoundInput* thisInput = (SoundInput*) in_pCookie;
	
	if ( ! thisInput )
	{
		return AK_Cancelled;
	}
	
	AkAutoLock <CAkLock> safeHandler( thisInput->m_streamLock );
	if( ! thisInput->m_isInputOn )
	{
		return AK_Cancelled;
	}
	
	if ( ! thisInput->EnqueueAudioData(io_Data) )
	{
		return AK_Fail;
	}
#endif // #if ! TARGET_IPHONE_SIMULATOR
    return AK_Success;

}


//---------------------------------------------------------------------------
SoundInput::SoundInput()
: m_isInputOn(false)
, m_deviceID(INVALID_DEVICE_ID)
{
	typedef AkReal32 SAMPLE_TYPE;
	const UInt32 BitsPerByte = 8;
	m_audioFormat.uSampleRate = AUDIO_SAMPLE_RATE;
	m_audioFormat.channelConfig.SetStandard( AK_SPEAKER_SETUP_MONO );
	m_audioFormat.uBitsPerSample = BitsPerByte * sizeof(SAMPLE_TYPE);
	m_audioFormat.uBlockAlign = sizeof(SAMPLE_TYPE);
	m_audioFormat.uTypeID = AK_FLOAT;
	m_audioFormat.uInterleaveID = AK_NONINTERLEAVED;
}

//---------------------------------------------------------------------------
SoundInput::~SoundInput() 
{
    InputOff();
	
	usleep(100);
	
}

//---------------------------------------------------------------------------
bool SoundInput::InputOn( AkUInt32 in_DevNumber )
{
	const AkInt32 micDeviceIndex = FindFirstInputDevice();
	if (micDeviceIndex == INVALID_DEVICE_INDEX) 
	{
		LOG_ERROR("Failed to find any audio input device.");
		return false;
	}
	
	m_deviceID = micDeviceIndex;
	
	if ( ! OpenStream(micDeviceIndex) )
	{
		return false;
	}
	
	if ( ! SoundInputMgr::Instance().RegisterInput(this, micDeviceIndex) )
	{
		return false;
	}
	
	return true;
}


bool SoundInput::OpenStream(AkUInt32 deviceIndex)
{
	AkAutoLock <CAkLock> safeHandler( m_streamLock );
	
	if ( m_isInputOn )
	{
		LOG_ERROR("Error opening input stream, one stream already open.");
		return false;
	}
		
	UInt32 nDevices = GetDeviceCount();
	if (nDevices == 0)
	{
		LOG_ERROR("Failed to find any input device.");
		return false;
	}
	
	if (deviceIndex >= nDevices) 
	{
		LOG_ERROR("Invalid device index exceeds device count!");
		return false;
	}
	
	// Allocate user stream buffer
	// NOTE: Reduce requestBufferSampleSize to reduce latency.
	AkUInt16 requestBufferSampleSize = g_uSamplesPerFrame;
	if ( ! m_ioBufferQueue.AllocateAll(requestBufferSampleSize*sizeof(AkDefaultSampleFormat)) )
	{
		LOG_ERROR("Error allocating user buffer for HAL input data.");
		DeAllocateAll();
		return false;
	}
	
	m_isInputOn = true;
	
	return true;
}

//---------------------------------------------------------------------------
void SoundInput::DeAllocateAll()
{
	m_ioBufferQueue.DeAllocateAll();
}


//---------------------------------------------------------------------------
AkUInt8 SoundInput::GetDeviceCount()
{
	return 1; // There is only one input device on iOS;
}

//---------------------------------------------------------------------------
AkInt32 SoundInput::FindFirstInputDevice()
{
	// Return default device.
	return 0;
	
}


//---------------------------------------------------------------------------
bool SoundInput::InputOff()
{
	SoundInputMgr::Instance().UnregisterInput( this, m_deviceID );

	if ( ! CloseStream() ) 
	{
		LOG_ERROR("Failed to close input stream.");		
		return false;
	}
	
	return true;
}

//---------------------------------------------------------------------------
bool SoundInput::CloseStream()
{
	AkAutoLock <CAkLock> safeHandler( m_streamLock );
	if ( ! m_isInputOn )
	{
		LOG_ERROR("Error closing an running or closed stream for device.");
		return false;
	}
	
	DeAllocateAll();
	
	m_isInputOn = false;
	
	return true;
}

#pragma mark ==== Wwise Plugin Delegates ====

//---------------------------------------------------------------------------
void SoundInput::GetFormatCallback(AkAudioFormat&  io_AudioFormat)
{
    io_AudioFormat.SetAll(
						  m_audioFormat.uSampleRate,
						  m_audioFormat.channelConfig,
						  m_audioFormat.uBitsPerSample,
						  m_audioFormat.uBlockAlign,
						  m_audioFormat.uTypeID,
						  m_audioFormat.uInterleaveID
						  );
}


//---------------------------------------------------------------------------
void SoundInput::Execute(AkAudioBuffer*	io_pBufferOut)
{
	if ( ! m_ioBufferQueue.IsAllocated() ) 
	{
		io_pBufferOut->eState = AK_Fail;
		io_pBufferOut->uValidFrames = 0;
		
		return;
	}
	
	AkUInt32 uMaxFrames = io_pBufferOut->MaxFrames();
	
	AkOSChar* AK_RESTRICT outBufferWritePos = (AkOSChar*)( io_pBufferOut->GetChannel(0) );	
	assert(io_pBufferOut->uValidFrames == 0);
	
	while (io_pBufferOut->uValidFrames != uMaxFrames) 
	{
		AkUInt16 framesLeftToWrite = (AkUInt16)uMaxFrames - io_pBufferOut->uValidFrames;
		
		AkUInt16 nTransferredBytes = m_ioBufferQueue.WriteToConsumer( (AkOSChar*) outBufferWritePos, framesLeftToWrite*m_audioFormat.uBlockAlign);
		
		// Update consumer write pointer and size
		outBufferWritePos += nTransferredBytes;
		io_pBufferOut->uValidFrames += nTransferredBytes/m_audioFormat.uBlockAlign;
		
		if ( m_ioBufferQueue.IsUnderrun() ) 
		{			
			break;
		}
		
	}

}

//---------------------------------------------------------------------------
bool SoundInput::EnqueueAudioData(const AudioBufferList *inBufferList)
{
	// copy to user buffer
	AkUInt16 bytesIn = m_ioBufferQueue.ReadFromProducer( (AkOSChar*) inBufferList->mBuffers[CHANNEL_INDEX_MONO].mData, inBufferList->mBuffers[CHANNEL_INDEX_MONO].mDataByteSize);
	if ( bytesIn != inBufferList->mBuffers[CHANNEL_INDEX_MONO].mDataByteSize )
	{
		LOG_ERROR("Buffer overrun during enqueuing.");
		return false;
	}
	
	return true;
}
