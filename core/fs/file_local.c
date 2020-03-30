#include "file_interface.h"

#include <stdio.h>

/*
============
__impl_FileOpen_local
============
*/
static file_t* __impl_FileOpen_local( file_t* f, const char* fileName, uint32_t flags ) {
    char mode[8];
    int i = 0;

    f->flags = flags;
    
    // local file on the hard drive
    switch( flags & F_MODE_MASK ) {
        case F_READ:
            mode[i++] = 'r';
            break;
        case F_WRITE:
            mode[i++] = 'w';
            break;
        case F_APPEND:
            mode[i++] = 'a';
            break;
    }
    mode[i++] = 'b';
    mode[i++] = 0;
    // open the file
    if( NULL == (f->file = (void*)fopen( fileName, mode )) ) {
        return NULL;
    }
    
    return f;
}

/*
============
__impl_FileClose_local
============
*/
static ecode_t __impl_FileClose_local( file_t* f ) {
    if( fclose( (FILE*)f->file ) ) {
        return E_BADF;
    }    
    return E_OK;
}

/*
============
__impl_FileRead_local
============
*/
static ssize_t  __impl_FileRead_local( void* dst, size_t size, file_t* f ) {
    return fread( dst, 1, size, (FILE*)f->file );
}

/*
============
__impl_FileRead_local
============
*/
static ssize_t  __impl_FileWrite_local( const void* src, size_t size, file_t* f ) {
    return fwrite( src, 1, size, (FILE*)f->file );
}

/*
============
__impl_FileFlush_local
============
*/
static ecode_t __impl_FileFlush_local( file_t* f ) {
    if( fflush( (FILE*)f->file ) ) {
        return E_BADF;
    }
    return E_OK;
}

/*
============
__impl_FileTell_local
============
*/
static ssize_t __impl_FileTell_local( file_t* f ) {
    return (ssize_t)ftell( (FILE*)f->file );
}

/*
============
__impl_FileSeek_local
============
*/
static ecode_t __impl_FileSeek_local( file_t* f, ssize_t offset, uint32_t origin ) {
    int o = 0;
    switch( origin ) {
        case F_SEEK_SET:
            o = SEEK_SET;
            break;
        case F_SEEK_CUR:
            o = SEEK_CUR;
            break;
        case F_SEEK_END:
            o = SEEK_END;
            break;
        default:
            break;
    }
    
    if( fseek( f->file, offset, o ) ) {
        return E_BADF;
    }    
    return E_OK;
}

/*
============
__impl_FileEOF_local
============
*/
static bool_t  __impl_FileEOF_local( file_t* f ) {
    if( feof( (FILE*)f->file ) ) {
        return btrue;
    }
    return bfalse;
}

/*
============
__impl_FileSize_local
============
*/
static ssize_t __impl_FileSize_local( file_t* f ) {
    int64_t pos;
    int64_t size;
    
    fgetpos( (FILE*)f->file, &pos );
    FileSeek( f, 0, F_SEEK_END );
    fgetpos( (FILE*)f->file, &size );
    fsetpos( (FILE*)f->file, &pos );
    //FileSeek( f, pos, F_SEEK_SET );
    
    return size > 0 ? (size_t)size : 0;
}


// initialize interface for local file
fileInterface_t fsFileLocalInterface_g = {
    __impl_FileOpen_local,
    __impl_FileClose_local,
    __impl_FileRead_local,
    __impl_FileWrite_local,
    __impl_FileFlush_local,
    __impl_FileTell_local,
    __impl_FileSeek_local,
    __impl_FileEOF_local,
    __impl_FileSize_local
};
