#include <core/fs.h>

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
    if( flags & F_READ ) {
        mode[i++] = 'r';
    } else if( flags & F_WRITE ) {
        mode[i++] = 'w';
    } else if( flags & F_APPEND ) {
        mode[i++] = 'a';
    }
    if( flags & F_BINARY ) {
        mode[i++] = 'b';
    }
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
__impl_FileRead_local
============
*/
static size_t  __impl_FileRead_local( void* dst, size_t size, file_t* f ) {
    return fread( dst, 1, size, f->file );
}

/*
============
__impl_FileRead_local
============
*/
static size_t  __impl_FileWrite_local( const void* src, size_t size, file_t* f ) {
    return fwrite( src, 1, size, (FILE*)f->file );
}

/*
============
__impl_FileGetPos_local
============
*/
static ecode_t __impl_FileGetPos_local( file_t* f, int64_t* pos ) {
    if( fgetpos( (FILE*)f->file, pos ) ) {
        return E_BADF;
    }
    return E_OK;
}

/*
============
__impl_FileSeek_local
============
*/
static ecode_t __impl_FileSeek_local( file_t* f, int64_t offset, uint32_t origin ) {
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
__impl_FileSetPos_local
============
*/
static ecode_t __impl_FileSetPos_local( file_t* f, const int64_t* pos ) {
    if( fsetpos( (FILE*)f->file, pos ) ) {
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
size_t __impl_FileSize_local( file_t* f ) {
    int64_t pos;
    int64_t size;
    
    FileGetPos( f, &pos );
    FileSeek( f, 0, F_SEEK_END );
    FileGetPos( f, &size );
    FileSetPos( f, &pos );
    FileSeek( f, pos, F_SEEK_SET );
    
    return size > 0 ? (size_t)size : 0;
}


/*
============
__impl_FileTell_local
============
*/
ssize_t __impl_FileTell_local( file_t* f ) {
    return (ssize_t)ftell( (FILE*)f->file );
}


// initialize interface for local file
fileInterface_t fsFileLocalInterface_g = {
    __impl_FileOpen_local,
    __impl_FileClose_local,
    __impl_FileFlush_local,
    __impl_FileRead_local,
    __impl_FileWrite_local,
    __impl_FileGetPos_local,
    __impl_FileSeek_local,
    __impl_FileSetPos_local,
    __impl_FileEOF_local,
    __impl_FileSize_local,
    __impl_FileTell_local
};
