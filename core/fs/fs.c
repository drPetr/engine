#include <core/fs.h>
#include <core/assert.h>
#include <core/var.h>

#include "file_helper.h"

#include <stdio.h>


var_t* inner_path_g;    // inner_path; use for file system

/*
============
FsInit
============
*/
void FsInit( void ) {
    inner_path_g = VarCreate( "inner_path", VAR_STR, "data/", NULL, "internal path to game archives" );
}

/*
============
FsInit
============
*/
void FsShutdown( void ) {
}

/*
============
FileOpen
============
*/
file_t* FileOpen( const char* fileName, uint32_t flags ) {
    file_t* f;
    
    assert_not_null( fileName );
    assert( FsFileCheckFlags(flags) );
    
    // allocate memory
    f = FsFileAllocate();
    
    // initialize file interface
    switch( flags & F_TYPE_MASK ) {
        case F_LOCAL:
            f->fn = &fsFileLocalInterface_g;
            break;
        case F_INNER:
            FsFileDeallocate( f );
            return NULL;
            break;
        case F_MEMORY:
            FsFileDeallocate( f );
            return NULL;
            break;
    }
    
    // if file created
    if( f->fn->open( f, fileName, flags ) ) {
        return f;
    }
    
    // else deallocate memory and return NULL pointer
    FsFileDeallocate( f );
    
    return NULL;
}


/*
============
FileClose
============
*/
ecode_t FileClose( file_t* f ) {
    ecode_t e;
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( f->fn->close );
    e = f->fn->close(f);
    FsFileDeallocate( f );
    return e;
}

/*
============
FileFlush
============
*/
ecode_t FileFlush( file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( f->fn->flush );
    return f->fn->flush( f );
}

/*
============
FileRead
============
*/
size_t FileRead( void* dst, size_t size, file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( dst );
    assert_not_null( f->fn->read );
    return f->fn->read(dst, size, f);
}

/*
============
FileWrite
============
*/
size_t FileWrite( const void* src, size_t size, file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( src );
    assert_not_null( f->fn->write );
    return f->fn->write(src, size, f);
}

/*
============
FileGetPos
============
*/
ecode_t FileGetPos( file_t* f, int64_t* pos ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( pos );
    assert_not_null( f->fn->getpos );
    return f->fn->getpos(f, pos);
}

/*
============
FileSeek
============
*/
ecode_t FileSeek( file_t* f, int64_t offset, uint32_t origin ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert( FsFileCheckOrigin(origin) );
    assert_not_null( f->fn->seek );
    return f->fn->seek(f, offset, origin);
}

/*
============
FileSetPos
============
*/
ecode_t FileSetPos( file_t* f, const int64_t* pos ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( pos );
    assert_not_null( f->fn->setpos );
    return f->fn->setpos(f, pos);
}

/*
============
FileEOF
============
*/
bool_t FileEOF( file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( f->fn->eof );
    return f->fn->eof(f);
}

/*
============
FileSize
============
*/
size_t FileSize( file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( f->fn->size );
    return f->fn->size(f);
}

/*
============
FileTell
============
*/
ssize_t FileTell( file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( f->fn->tell );
    return f->fn->tell(f);
}