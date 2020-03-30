#include "file_interface.h"
#include "fs_tree.h"
#include <core/string.h>
#include <core/var.h>

#include <core/fs.h>
#include <stdio.h>


extern var_t* inner_path_g;

/*
============
__impl_FileOpen_inner
============
*/
static file_t* __impl_FileOpen_inner( file_t* f, const char* fileName, uint32_t flags ) {
    char mode[8];
    char innerFileName[1024];
    size_t len;
    int i = 0;
    
    // make relative file path
    StrCpy( innerFileName, inner_path_g->s );
    len = StrLen( innerFileName );
    if( innerFileName[len - 1] != '/' ) {
        innerFileName[len - 1] = '/';
        len++;
    }
    // skip slashes
    while( *fileName == '/' ) {
        fileName++;
    }
    // last step of make relative file path
    StrCpy( innerFileName + len, fileName );

    //
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
    if( NULL == (f->file = (void*)fopen( innerFileName, mode )) ) {
        return NULL;
    }
    
    return f;
}

/*
============
__impl_FileClose_inner
============
*/
static ecode_t __impl_FileClose_inner( file_t* f ) {
    if( fclose( (FILE*)f->file ) ) {
        return E_BADF;
    }    
    return E_OK;
}

/*
============
__impl_FileRead_inner
============
*/
static ssize_t  __impl_FileRead_inner( void* dst, size_t size, file_t* f ) {
    return fread( dst, 1, size, (FILE*)f->file );
}

/*
============
__impl_FileRead_inner
============
*/
static ssize_t  __impl_FileWrite_inner( const void* src, size_t size, file_t* f ) {
    return fwrite( src, 1, size, (FILE*)f->file );
}

/*
============
__impl_FileFlush_inner
============
*/
static ecode_t __impl_FileFlush_inner( file_t* f ) {
    if( fflush( (FILE*)f->file ) ) {
        return E_BADF;
    }
    return E_OK;
}

/*
============
__impl_FileTell_inner
============
*/
static ssize_t __impl_FileTell_inner( file_t* f ) {
    return (ssize_t)ftell( (FILE*)f->file );
}

/*
============
__impl_FileSeek_inner
============
*/
static ecode_t __impl_FileSeek_inner( file_t* f, ssize_t offset, uint32_t origin ) {
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
__impl_FileEOF_inner
============
*/
static bool_t  __impl_FileEOF_inner( file_t* f ) {
    if( feof( (FILE*)f->file ) ) {
        return btrue;
    }
    return bfalse;
}

/*
============
__impl_FileSize_inner
============
*/
static ssize_t __impl_FileSize_inner( file_t* f ) {
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
fileInterface_t fsFileInnerInterface_g = {
    __impl_FileOpen_inner,
    __impl_FileClose_inner,
    __impl_FileRead_inner,
    __impl_FileWrite_inner,
    __impl_FileFlush_inner,
    __impl_FileTell_inner,
    __impl_FileSeek_inner,
    __impl_FileEOF_inner,
    __impl_FileSize_inner
};
