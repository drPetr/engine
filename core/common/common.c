#include <core/common.h>


#include <stdio.h>

FILE*   log_g;

/*
============
ComInit
============
*/
void ComInit( void ) {
    log_g = stdout;
}

/*
============
ComShutdown
============
*/
void ComShutdown( void ) {
    fflush( log_g );
}

/*
============
LogVPrintf
============
*/
void LogVPrintf( const char* fmt, va_list args ) {
    vfprintf( log_g, fmt, args );
}

/*
============
LogPrintf
============
*/
void LogPrintf( const char* fmt, ... ) {
    va_list arg;
    
    va_start( arg, fmt );
    LogVPrintf( fmt, arg );
    va_end( arg );
}

/*
============
Warning
============
*/
void Warning( const char* fmt, ... ) {
    va_list arg;
    
    va_start( arg, fmt );
    LogPrintf( "warning: " );
    LogVPrintf( fmt, arg );
    va_end( arg );
}

/*
============
Error
============
*/
void Error( const char* fmt, ... ) {
    va_list arg;
    
    va_start( arg, fmt );
    LogPrintf( "error: " );
    LogVPrintf( fmt, arg );
    va_end( arg );
}

/*
============
FatalError
============
*/
void FatalError( const char* fmt, ... ) {
    va_list arg;
    
    va_start( arg, fmt );
    LogPrintf( "fatal: " );
    LogVPrintf( fmt, arg );
    va_end( arg );
}