#ifndef __FS_H__
#define __FS_H__

#include <core/handle.h>
#include <core/types.h>
#include <core/ecode.h>


struct fileInterface_t;


enum fileFlags_t {
    F_LOCAL     =   1,
    F_MEMORY    =   2,
    F_TYPE_MASK =   0x0f,
    F_READ      =   0x10,
    F_WRITE     =   0x20,
    F_APPEND    =   0x40,
    F_BINARY    =   0x80
};

// 
enum fileOrigin_t {
    F_SEEK_SET  =   0x10,
    F_SEEK_CUR  =   0x20,
    F_SEEK_END  =   0x40
};

// file structure
typedef struct {
    void*       file;
    uint32_t    flags;
    struct fileInterface_t* fn;
} file_t;


// interface
typedef file_t*(*fnFsFOpen)(file_t*,const char*,uint32_t);
typedef ecode_t(*fnFsFClose)(file_t*);
typedef ecode_t(*fnFsFFlush)(file_t*);
typedef size_t(*fnFsFRead)(void*,size_t,file_t*);
typedef size_t(*fnFsFWrite)(const void*,size_t,file_t*);
typedef ecode_t(*fnFsFGetPos)(file_t*,int64_t*);
typedef ecode_t(*fnFsFSeek)(file_t*,int64_t,uint32_t);
typedef ecode_t(*fnFsFSetPos)(file_t*,const int64_t*);
typedef bool_t(*fnFsEOF)(file_t*);
typedef size_t(*fnFsFSize)(file_t*);
typedef ssize_t(*fnFsFTell)(file_t*);

typedef struct fileInterface_t {
    fnFsFOpen       open;
    fnFsFClose      close;
    fnFsFFlush      flush;
    fnFsFRead       read;
    fnFsFWrite      write;
    fnFsFGetPos     getpos;
    fnFsFSeek       seek;
    fnFsFSetPos     setpos;
    fnFsEOF         eof;
    fnFsFSize       size;
    fnFsFTell       tell;
} fileInterface_t;

// static interfaces
extern fileInterface_t fsFileLocalInterface_g;



file_t*     FsFOpen( const char* fileName, uint32_t flags );
ecode_t     FsFClose( file_t* f );
ecode_t     FsFFlush( file_t* f );
size_t      FsFRead( void* dst, size_t size, file_t* f );
size_t      FsFWrite( const void* src, size_t size, file_t* f );
ecode_t     FsFGetPos( file_t* f, int64_t* pos );
ecode_t     FsFSeek( file_t* f, int64_t offset, uint32_t origin );
ecode_t     FsFSetPos( file_t* f, const int64_t* pos );
bool_t      FsEOF( file_t* f );
size_t      FsFSize( file_t* f );
ssize_t     FsFTell( file_t* f );



#endif //__FS_H__