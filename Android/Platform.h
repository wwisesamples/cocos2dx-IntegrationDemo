// Platform.h
// Copyright (C) 2010 Audiokinetic Inc
/// \file 
/// Contains platform-specific helpers for the Windows platform.

#pragma once

#define DEMO_DEFAULT_POOL_SIZE 2*1024*1024
#define DEMO_LENGINE_DEFAULT_POOL_SIZE 2*1024*1024

// WG-21707
// Soundbanks will be automatically deployed to your Android device as part of the APK, if you 
// place the generated soundbanks under $WWISESDK/samples/IntegrationDemo/WwiseProject/GeneratedSoundBanks/Android
// to $WWISESDK/samples/IntegrationDemo/assets/$SOUND_BANK_PATH
// NOTE: SOUND_BANK_PATH needs the trailing path separator.
#define SOUND_BANK_PATH "GeneratedSoundBanks/Android/"

// Defines for button name strings
#define UG_BUTTON1_NAME  "Enter"
#define UG_BUTTON2_NAME  "Space"
#define UG_BUTTON3_NAME  "Z"
#define UG_BUTTON4_NAME  "X"
#define UG_BUTTON5_NAME  "C"
#define UG_BUTTON6_NAME  "V"
#define UG_BUTTON7_NAME  "B"
#define UG_BUTTON8_NAME  "N"
#define UG_BUTTON9_NAME  "UNDEFINED"
#define UG_BUTTON10_NAME "UNDEFINED"   
#define UG_BUTTON11_NAME "UNDEFINED"
#define UG_RIGHT_STICK	 ""
#define UG_LEFT_STICK	 "UNDEFINED"
#define OS_BUTTON_NAME   "UNDEFINED"
#define DIRECTIONAL_TYPE "A,W,D,S"

#define HELP_BUTTON   UG_BUTTON7_NAME
#define ACTION_BUTTON UG_BUTTON1_NAME
#define BACK_BUTTON   UG_BUTTON2_NAME

#define INTEGRATIONDEMO_SCREEN_CHAR_WIDTH	60
#define INTEGRATIONDEMO_BGM

#define __AK_OSCHAR_SNPRINTF snprintf

#define CODECTYPE_STANDARD	AKCODECID_ADPCM

#if defined(AK_ANDROID)
#define FONT_SIZE_TITLE	    16
#define FONT_SIZE_MENU	    10
#endif
