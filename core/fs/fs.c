#include <core/fs.h>
#include <core/assert.h>

#include "file_helper.h"

#include <stdio.h>

/*
============
FsFOpen
============
*/
file_t* FsFOpen( const char* fileName, uint32_t flags ) {
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
FsFClose
============
*/
ecode_t FsFClose( file_t* f ) {
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
FsFFlush
============
*/
ecode_t FsFFlush( file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( f->fn->flush );
    return f->fn->flush( f );
}

/*
============
FsFRead
============
*/
size_t FsFRead( void* dst, size_t size, file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( dst );
    assert_not_null( f->fn->read );
    return f->fn->read(dst, size, f);
}

/*
============
FsFWrite
============
*/
size_t FsFWrite( const void* src, size_t size, file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( src );
    assert_not_null( f->fn->write );
    return f->fn->write(src, size, f);
}

/*
============
FsFGetPos
============
*/
ecode_t FsFGetPos( file_t* f, int64_t* pos ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( pos );
    assert_not_null( f->fn->getpos );
    return f->fn->getpos(f, pos);
}

/*
============
FsFSeek
============
*/
ecode_t FsFSeek( file_t* f, int64_t offset, uint32_t origin ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert( FsFileCheckOrigin(origin) );
    assert_not_null( f->fn->seek );
    return f->fn->seek(f, offset, origin);
}

/*
============
FsFSetPos
============
*/
ecode_t FsFSetPos( file_t* f, const int64_t* pos ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( pos );
    assert_not_null( f->fn->setpos );
    return f->fn->setpos(f, pos);
}

/*
============
FsEOF
============
*/
bool_t FsEOF( file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( f->fn->eof );
    return f->fn->eof(f);
}

/*
============
FsFSize
============
*/
size_t FsFSize( file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( f->fn->size );
    return f->fn->size(f);
}

/*
============
FsFTell
============
*/
ssize_t FsFTell( file_t* f ) {
    assert_not_null( f );
    assert_not_null( f->file );
    assert_not_null( f->fn->tell );
    return f->fn->tell(f);
}