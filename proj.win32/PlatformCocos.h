#pragma once

#include "windows.h"

#ifdef SOUND_BANK_PATH
#undef SOUND_BANK_PATH
#define SOUND_BANK_PATH L"../../WwiseProject/GeneratedSoundBanks/Windows/"
#endif

#define FONT_SIZE_TITLE	    30
#define FONT_SIZE_MENU	    24

extern bool g_isLandscape;
#define SCREEN_POS_SCALER_DESCRIPTION_Y 0.90f
#define SCREEN_POS_SCALER_SELBUTTON_X 0.15f

#define MOVEMENT_STEP_SIZE 5

void LOGAK(char *format, ...);
void LOGAKW(TCHAR *format, ...);