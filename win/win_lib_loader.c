#include "win.h"

#include <Windows.h>

/*
============
LibLoad
============
*/
HMODULE LibLoad( char* libName ) {
    return LoadLibrary( libName );
}

/*
============
LibFuncLoad
============
*/
FARPROC LibFuncLoad( HMODULE lib, char* funcName ) {
    return GetProcAddress( lib, funcName );
}