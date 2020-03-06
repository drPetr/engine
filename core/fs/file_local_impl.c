#include <core/fs.h>

#include <stdio.h>

/*
============
__impl_FsFOpen_local
============
*/
static file_t* __impl_FsFOpen_local( file_t* f, const char* fileName, uint32_t flags ) {
    char mode[8];
    int i = 0;

    f->flags = flags;
    
    // local file on the hard drive
    if( flags & F_READ ) {
        mode[i++] = 'r';
    } else if( F_WRITE ) {
        mode[i++] = 'w';
    } else if( F_APPEND ) {
        mode[i++] = 'a';
    }
    if( F_BINARY ) {
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
__impl_FsFClose_local
============
*/
static ecode_t __impl_FsFClose_local( file_t* f ) {
    if( fclose( (FILE*)f->file ) ) {
        return E_BADF;
    }    
    return E_OK;
}

/*
============
__impl_FsFFlush_local
============
*/
static ecode_t __impl_FsFFlush_local( file_t* f ) {
    if( fflush( (FILE*)f->file ) ) {
        return E_BADF;
    }
    return E_OK;
}

/*
============
__impl_FsFRead_local
============
*/
static size_t  __impl_FsFRead_local( void* dst, size_t size, file_t* f ) {
    return fread( dst, 1, size, f->file );
}

/*
============
__impl_FsFRead_local
============
*/
static size_t  __impl_FsFWrite_local( const void* src, size_t size, file_t* f ) {
    return fwrite( src, 1, size, (FILE*)f->file );
}

/*
============
__impl_FsFGetPos_local
============
*/
static ecode_t __impl_FsFGetPos_local( file_t* f, int64_t* pos ) {
    if( fgetpos( (FILE*)f->file, pos ) ) {
        return E_BADF;
    }
    return E_OK;
}

/*
============
__impl_FsFSeek_local
============
*/
static ecode_t __impl_FsFSeek_local( file_t* f, int64_t offset, uint32_t origin ) {
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
__impl_FsFSetPos_local
============
*/
static ecode_t __impl_FsFSetPos_local( file_t* f, const int64_t* pos ) {
    if( fsetpos( (FILE*)f->file, pos ) ) {
        return E_BADF;
    }
    return E_OK;
}

/*
============
__impl_FsFSetPos_local
============
*/
static bool_t  __impl_FsEOF_local( file_t* f ) {
    if( feof( (FILE*)f->file ) ) {
        return btrue;
    }
    return bfalse;
}

/*
============
__impl_FsFSize_local
============
*/
size_t __impl_FsFSize_local( file_t* f ) {
    int64_t pos;
    int64_t size;
    
    FsFGetPos( f, &pos );
    FsFSeek( f, 0, F_SEEK_END );
    FsFGetPos( f, &size );
    FsFSetPos( f, &pos );
    FsFSeek( f, pos, F_SEEK_SET );
    
    return size > 0 ? (size_t)size : 0;
}


/*
============
__impl_FsFTell_local
============
*/
ssize_t __impl_FsFTell_local( file_t* f ) {
    return (ssize_t)ftell( (FILE*)f->file );
}


// initialize interface for local file
fileInterface_t fsFileLocalInterface_g = {
    __impl_FsFOpen_local,
    __impl_FsFClose_local,
    __impl_FsFFlush_local,
    __impl_FsFRead_local,
    __impl_FsFWrite_local,
    __impl_FsFGetPos_local,
    __impl_FsFSeek_local,
    __impl_FsFSetPos_local,
    __impl_FsEOF_local,
    __impl_FsFSize_local,
    __impl_FsFTell_local
};
