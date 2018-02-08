#include "stdafx.h"

bool g_isLandscape = false;

// Alloc hook that need to be define by the game
namespace AK
{
	void * AllocHook( size_t in_size )
	{
		return malloc( in_size );
	}
	void FreeHook( void * in_ptr )
	{
		free( in_ptr );
	}
}

void LOGAKW(AkOSChar* _Buffer)  
{
    char szBuff[1024] = {0};
    AKPLATFORM::AkWideCharToChar((const wchar_t*)_Buffer, 1024, szBuff);
    __android_log_print(ANDROID_LOG_DEBUG, __FILE__, (const char*)&szBuff);
}
