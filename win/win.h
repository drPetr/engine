#ifndef __WIN_H__
#define __WIN_H__

#include <Windows.h>


HMODULE     LibLoad( char* libName );
FARPROC     LibFuncLoad( HMODULE lib, char* funcName );


#endif //__WIN_H__