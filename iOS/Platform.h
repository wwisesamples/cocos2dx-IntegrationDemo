// Platform.h
// Copyright (C) 2010 Audiokinetic Inc
/// \file 
/// Contains platform-specific helpers for the iOS platform.

#pragma once

#include <MacTypes.h>

#ifndef __OBJC__
#include <AK/SoundEngine/Common/AkTypes.h>
#else
typedef char AkOSChar;
#endif

#define SYSTEM_VERSION_EQUAL_TO(v)                  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedSame)
#define SYSTEM_VERSION_GREATER_THAN(v)              ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedDescending)
#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(v)     ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedDescending)

#define SOUND_BANK_PATH g_szBasePath
#define DEMO_DEFAULT_POOL_SIZE 2*1024*1024
#define DEMO_LENGINE_DEFAULT_POOL_SIZE 1*1024*1024
#define AK_IOS_MAX_BANK_PATH 1024
extern char g_szBasePath[AK_IOS_MAX_BANK_PATH];

// Defines for button name strings
#define UG_BUTTON1_NAME  "A"
#define UG_BUTTON2_NAME  "B"
#define UG_BUTTON3_NAME  "X"
#define UG_BUTTON4_NAME  "Y"
#define UG_BUTTON5_NAME  "UNDEFINED"
#define UG_BUTTON6_NAME  "UNDEFINED"
#define UG_BUTTON7_NAME  "Start"
#define UG_BUTTON8_NAME  "Select"
#define UG_BUTTON9_NAME  "UNDEFINED"
#define UG_BUTTON10_NAME "UNDEFINED"  
#define UG_BUTTON11_NAME "UNDEFINED"
#define OS_BUTTON_NAME   "UNDEFINED"
#define UG_RIGHT_STICK	 "Right"
#define UG_LEFT_STICK	 "Left"
#define DIRECTIONAL_TYPE "Dpad"
#define HELP_BUTTON   UG_BUTTON7_NAME
#define ACTION_BUTTON UG_BUTTON1_NAME
#define BACK_BUTTON   UG_BUTTON2_NAME

#define INTEGRATIONDEMO_SCREEN_CHAR_WIDTH	95

#define __AK_OSCHAR_SNPRINTF snprintf

// Defines specific to iOS UI
// Interface colors

//									Red			Green		Blue		Alpha
#define JOYSTICK_BACKGROUND_COLOR	0.04296875,	0.37890625,	0.640625,	1
#define JOYSTICK_COLOR				0,			0.703125,	0.328125,	1
#define	GAMEPAD_TEXT_COLOR			0.140625,	0.09765625,	0.6953125,	1

//									Red			Green		Blue		Alpha
#define DEFAULT_DRAW_COLOUR			1,			1,			1,			1	// 0xFFFFFFFF	 
#define SELECTED_DRAW_COLOUR		1,			1,			0,			1	// Yellow
#define TITLE_DRAW_COLOUR			1,			1,			1,			1	// White
#define ERROR_DRAW_COLOUR			1,			0,			0,			1	// Red

// Number of string texture that view will cache
#define TEXTURE_CACHE_COUNT 40

#define CODECTYPE_STANDARD	AKCODECID_ADPCM

#if ! TARGET_IPHONE_SIMULATOR
	#define INTEGRATIONDEMO_MICROPHONE
#endif // #if ! TARGET_IPHONE_SIMULATOR

#define INTEGRATIONDEMO_BGM

#define AUDIO_SAMPLE_RATE 32000

extern UInt32 g_uSamplesPerFrame;

extern bool g_bEnableMicDemo;

#if COCOS_INTEGRATION
#define FONT_SIZE_TITLE  16
#define FONT_SIZE_MENU   10
#endif
