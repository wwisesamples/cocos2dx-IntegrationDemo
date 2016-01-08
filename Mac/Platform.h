// Platform.h
// Copyright (C) 2011 Audiokinetic Inc
/// \file 
/// Contains platform-specific helpers for the iOS platform.

#pragma once

#ifndef __OBJC__
#include <AK/SoundEngine/Common/AkTypes.h>
#else
typedef char AkOSChar;
#endif

#define SOUND_BANK_PATH "../../../samples/IntegrationDemo/WwiseProject/GeneratedSoundBanks/Mac/"

#define DEMO_DEFAULT_POOL_SIZE 2*1024*1024
#define DEMO_LENGINE_DEFAULT_POOL_SIZE 1*1024*1024

#define UG_BUTTON1_NAME  "Enter"
#define UG_BUTTON2_NAME  "Escape"
#define UG_BUTTON3_NAME  "Q"
#define UG_BUTTON4_NAME  "E"
#define UG_BUTTON5_NAME  "Shift"
#define UG_BUTTON6_NAME  "R"
#define UG_BUTTON7_NAME  "F1"
#define UG_BUTTON8_NAME  "BACK"
#define UG_BUTTON9_NAME  "UNDEFINED"
#define UG_BUTTON10_NAME "UNDEFINED"  
#define UG_BUTTON11_NAME "UNDEFINED"
#define UG_RIGHT_STICK	 ""
#define UG_LEFT_STICK	 "UNDEFINED"
#define OS_BUTTON_NAME   "UNDEFINED"
#define DIRECTIONAL_TYPE "Arrow Keys"


#define HELP_BUTTON		UG_BUTTON7_NAME
#define ACTION_BUTTON	UG_BUTTON1_NAME
#define BACK_BUTTON		UG_BUTTON2_NAME

#define INTEGRATIONDEMO_SCREEN_CHAR_WIDTH	95

#define __AK_OSCHAR_SNPRINTF snprintf

// Text colors
//									Red			Green		Blue		Alpha
#define DEFAULT_DRAW_COLOUR			1,			1,			1,			1	// 0xFFFFFFFF	 
#define SELECTED_DRAW_COLOUR		1,			1,			0,			1	// Yellow
#define TITLE_DRAW_COLOUR			1,			1,			1,			1	// White
#define ERROR_DRAW_COLOUR			1,			0,			0,			1	// Red

// Number of string texture that view will cache
#define TEXTURE_CACHE_COUNT 40

#define CODECTYPE_STANDARD	AKCODECID_ADPCM

#define INTEGRATIONDEMO_MICROPHONE

#define COREAUDIO_MACOSX_V10_6_AND_LATER
#define COREAUDIO_MACOSX_V10_5_AND_LATER

extern UInt32 g_uSamplesPerFrame;
#if COCOS_INTEGRATION
#define FONT_SIZE_TITLE  30
#define FONT_SIZE_MENU   24
#endif
