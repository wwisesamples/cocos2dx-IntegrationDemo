#include "stdafx.h"

bool g_isLandscape = true;

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
