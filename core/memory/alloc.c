#include <core/alloc.h>
#include <core/common.h>

#include <stdlib.h>

alloc_t defaultAllocator = { malloc, free };





/*
============
GetDefaultAllocator
============
*/
static alloc_t* GetDefaultAllocator( void ) {
    return &defaultAllocator;
}

/*
============
AllocInit
============
*/
void AllocInit( void ) {
    LogPrintf( "initialize memory allocators ...\n" );
}

/*
============
AllocShutdown
============
*/
void AllocShutdown( void ) {
    LogPrintf( "shutdown memory allocators ...\n" );
}

/*
============
GetVarStringsAllocator
============
*/
alloc_t* GetVarStringsAllocator( void ) {
    return GetDefaultAllocator();
}

/*
============
GetArchiveFileNamesStringAllocator
============
*/
alloc_t* GetArchiveFileNamesStringAllocator( void ) {
    return GetDefaultAllocator();
}

/*
============
GetVarStringsAllocator
============
*/
alloc_t* GetSystemAllocator( void ) {
    return GetDefaultAllocator();
}

/*
============
GetFsTreeAllocator
============
*/
alloc_t* GetFsTreeAllocator( void ) {
    return GetDefaultAllocator();
}


/*
============
GetFsTreeStringsAllocator
============
*/
alloc_t* GetFsTreeStringsAllocator( void ) {
    return GetDefaultAllocator(); 
}

/*
============
GetZipFileStructAllocator
============
*/
alloc_t* GetZipFileStructAllocator( void ) {
    return GetDefaultAllocator(); 
}