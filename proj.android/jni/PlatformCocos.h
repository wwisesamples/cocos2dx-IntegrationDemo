#pragma once
#include <AK/SoundEngine/Common/AkTypes.h>

#ifdef SOUND_BANK_PATH
#undef SOUND_BANK_PATH
#define SOUND_BANK_PATH "GeneratedSoundBanks/Android/"
#endif


#define FONT_SIZE_TITLE	    16
#define FONT_SIZE_MENU	    10

extern bool g_isLandscape;
#define SCREEN_POS_SCALER_DESCRIPTION_Y 0.90f
#define SCREEN_POS_SCALER_SELBUTTON_X 0.50f

#define MOVEMENT_STEP_SIZE 1

#define __AK_OSCHAR_SNPRINTF 	snprintf
#define  LOGAK(...)  __android_log_print(ANDROID_LOG_DEBUG, __FILE__ ,__VA_ARGS__)
void LOGAKW(AkOSChar* _Buffer);