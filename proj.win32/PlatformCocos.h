/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

  Version: v2017.1.4  Build: 6407
  Copyright (c) 2006-2017 Audiokinetic Inc.
*******************************************************************************/

// Platform.h
/// \file 
/// Contains platform-specific helpers for the Windows platform.

#pragma once

#ifdef SOUND_BANK_PATH
#undef SOUND_BANK_PATH
#define SOUND_BANK_PATH L"../../WwiseProject/GeneratedSoundBanks/Windows/"
#endif

#define FONT_SIZE_TITLE	    30
#define FONT_SIZE_MENU	    24

extern bool g_isLandscape;
#define SCREEN_POS_SCALER_DESCRIPTION_Y 0.90f
#define SCREEN_POS_SCALER_SELBUTTON_X 0.15f