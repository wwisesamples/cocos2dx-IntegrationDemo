// Platform.h
// Copyright (C) 2011 Audiokinetic Inc
/// \file 
/// Contains platform-specific helpers for the iOS platform.

#pragma once

#ifdef SOUND_BANK_PATH
#undef SOUND_BANK_PATH
#define SOUND_BANK_PATH "../../../../../../../../WwiseProject/GeneratedSoundBanks/Mac" // AkOSChar is char. Don't use L-stirng here. CWD on Mac is the POSIX-executable dir.
#endif

#define FONT_SIZE_TITLE  30
#define FONT_SIZE_MENU   24

