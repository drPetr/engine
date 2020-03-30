#include "file_helper.h"

#include <core/assert.h>
#include <stdlib.h>

/*
============
FsFileAllocate
============
*/
file_t* FsFileAllocate( void ) {
    file_t* f;
    f = malloc( sizeof(file_t) );
    return f;
}

/*
============
FsFileDeallocate
============
*/
void FsFileDeallocate( file_t* f ) {
    free( f );
}

/*
============
FsFileCheckFlags
============
*/
bool_t FsFileCheckFlags( uint32_t flags ) {
    uint32_t type;
    uint32_t mode;
    
    // extract parameters
    type = flags & F_TYPE_MASK;
    mode = flags & F_MODE_MASK;
    
    // check type
    switch( type ) {
        case F_LOCAL:
        case F_INNER:
            break;
        default:
            return bfalse;
    }
    
    // check mode
    switch( mode ) {
        case F_READ:
        case F_WRITE:
        case F_APPEND:
            break;
        default:
            return bfalse;
    }
    
    return btrue;
}

/*
============
FsFileCheckOrigin
============
*/
bool_t FsFileCheckOrigin( uint32_t origin ) {
    switch( origin ) {
        case F_SEEK_SET:
        case F_SEEK_CUR:
        case F_SEEK_END:
            break;
        default:
            return bfalse;
    }
    
    return btrue;
}
