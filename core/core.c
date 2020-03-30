#include <core/core.h>

#include <core/fs.h>
#include <core/common.h>
#include <core/alloc.h>

/*
============
CoreInit

in order not to initialize each subsystem manually, 
all initialization calls must occur here
============
*/
void CoreInit( void ) {
    ComInit();
    
    LogPrintf( "core initialize ...\n" );
    
    AllocInit();
    FsInit();
}

/*
============
CoreShutdown

Kernel shutdown
============
*/
void CoreShutdown( void ) {
    FsShutdown();
    AllocShutdown();
    
    LogPrintf( "core shutdown ...\n" );
    
    ComShutdown();
}
