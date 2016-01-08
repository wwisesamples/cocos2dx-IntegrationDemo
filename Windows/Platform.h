// Platform.h
// Copyright (C) 2010 Audiokinetic Inc
/// \file 
/// Contains platform-specific helpers for the Windows platform.

#pragma once

#define DEMO_DEFAULT_POOL_SIZE 2*1024*1024
#define DEMO_LENGINE_DEFAULT_POOL_SIZE 2*1024*1024
#define SOUND_BANK_PATH L"../../../samples/IntegrationDemo/WwiseProject/GeneratedSoundBanks/Windows/"


// Defines for button name strings
#define UG_BUTTON1_NAME  "Enter/A"
#define UG_BUTTON2_NAME  "Escape/B"
#define UG_BUTTON3_NAME  "Q/X"
#define UG_BUTTON4_NAME  "E/Y"
#define UG_BUTTON5_NAME  "Shift/LB"
#define UG_BUTTON6_NAME  "R/RB"
#define UG_BUTTON7_NAME  "F1/START"
#define UG_BUTTON8_NAME  "BACK"
#define UG_BUTTON9_NAME  "Left Thumbstick"
#define UG_BUTTON10_NAME "Right Thumbstick"  
#define UG_BUTTON11_NAME ""
#define UG_RIGHT_STICK	 "Right Stick"
#define UG_LEFT_STICK	 "Left Stick"
#define OS_BUTTON_NAME   "Alt"
#define DIRECTIONAL_TYPE "Arrow Keys/Dpad"

#define HELP_BUTTON   UG_BUTTON7_NAME
#define ACTION_BUTTON UG_BUTTON1_NAME
#define BACK_BUTTON   UG_BUTTON2_NAME

#define INTEGRATIONDEMO_SCREEN_CHAR_WIDTH	95
#define INTEGRATIONDEMO_MICROPHONE

#define __AK_OSCHAR_SNPRINTF _snwprintf

#define CODECTYPE_STANDARD	AKCODECID_ADPCM
#if COCOS_INTEGRATION
#define FONT_SIZE_TITLE	    30
#define FONT_SIZE_MENU	    24
#endif