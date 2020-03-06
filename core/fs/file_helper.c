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
    uint8_t type;
    bool_t fRead;
    bool_t fWrite;
    bool_t fAppend;
    
    // extract parameters
    type = flags & F_TYPE_MASK;
    fRead = !!(flags & F_READ);
    fWrite = !!(flags & F_WRITE);
    fAppend = !!(flags & F_APPEND);
    
    // check type
    switch( type ) {
        case F_LOCAL:
        //case F_MEMORY:
            break;
        default:
            return bfalse;
    }
    
    // check only one alternative
    if( (fRead + fWrite + fAppend) != 1 ) {
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
