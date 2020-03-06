#include "assert.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>



void __impl_assert( int e, const char* expr, const char* file, int line ) {
    if( !e ){
        fprintf( stderr, "assertion failed: %s\n", expr );
        fprintf( stderr, "file: %s\n", file );
        fprintf( stderr, "line: %d\n", line );
        exit(1);
    }
}

void __impl_asserta( int e, const char* expr, const char* file, int line, const char* fmt, ... ) {
    va_list args;
    if( !e ){
        va_start( args, fmt );
        fprintf( stderr, "assertion failed: %s\n", expr );
        fprintf( stderr, "file: %s\n", file );
        fprintf( stderr, "line: %d\n", line );
        vfprintf( stderr, fmt, args );
        va_end(args);
        exit(1);
    }
}

void __impl_verify( int e, const char* expr, const char* file, int line ) {
    if( !e ){
        fprintf( stderr, "verifying failed: %s\n", expr );
        fprintf( stderr, "file: %s\n", file );
        fprintf( stderr, "line: %d\n", line );
        exit(1);
    }
}

void __impl_verifya( int e, const char* expr, const char* file, int line, const char* fmt, ... ) {
    va_list args;
    if( !e ){
        va_start( args, fmt );
        fprintf( stderr, "verifying failed: %s\n", expr );
        fprintf( stderr, "file: %s\n", file );
        fprintf( stderr, "line: %d\n", line );
        vfprintf( stderr, fmt, args );
        va_end(args);
        exit(1);
    }
}