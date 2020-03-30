#include <core/common.h>

#include <Windows.h>
#include <shlwapi.h>

/*
============
SysFileExsists
============
*/
bool_t SysFileExsists( const char* fileName ) {
    return !!PathFileExistsA( fileName );
}