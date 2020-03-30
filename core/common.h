#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdarg.h>

#include <core/types.h>


void ComInit( void );
void ComShutdown( void );

void LogVPrintf( const char* fmt, va_list args );
void LogPrintf( const char* fmt, ... );
void Warning( const char* fmt, ... );
void Error( const char* fmt, ... );
void FatalError( const char* fmt, ... );

bool_t  SysFileExsists( const char* fileName );

#endif //__COMMON_H__