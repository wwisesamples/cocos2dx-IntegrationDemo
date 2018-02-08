#pragma once

#define SOUND_BANK_PATH g_szBasePath
#define AK_IOS_MAX_BANK_PATH 1024
extern char g_szBasePath[AK_IOS_MAX_BANK_PATH];

#define FONT_SIZE_TITLE  16
#define FONT_SIZE_MENU   10

extern bool g_isLandscape;
#define SCREEN_POS_SCALER_DESCRIPTION_Y 0.95f
#define SCREEN_POS_SCALER_SELBUTTON_X 0.50f

#define MOVEMENT_STEP_SIZE 5