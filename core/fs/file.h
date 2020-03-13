#ifndef __FILE_H__
#define __FILE_H__


#include <core/handle.h>
#include <core/types.h>
#include <core/ecode.h>


struct fileInterface_t;


enum fileFlags_t {
    F_LOCAL     =   1,
    F_MEMORY    =   2,
    F_INNER     =   3,
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
    ssize_t     offset;
    size_t      dataSize;
    struct fileInterface_t* fn;
} file_t;


// interface
typedef file_t*(*fnFileOpen)(file_t*,const char*,uint32_t);
typedef ecode_t(*fnFileClose)(file_t*);
typedef ecode_t(*fnFileFlush)(file_t*);
typedef size_t(*fnFileRead)(void*,size_t,file_t*);
typedef size_t(*fnFileWrite)(const void*,size_t,file_t*);
typedef ecode_t(*fnFileGetPos)(file_t*,int64_t*);
typedef ecode_t(*fnFileSeek)(file_t*,int64_t,uint32_t);
typedef ecode_t(*fnFileSetPos)(file_t*,const int64_t*);
typedef bool_t(*fnFileEOF)(file_t*);
typedef size_t(*fnFileSize)(file_t*);
typedef ssize_t(*fnFileTell)(file_t*);

typedef struct fileInterface_t {
    fnFileOpen      open;
    fnFileClose     close;
    fnFileFlush     flush;
    fnFileRead      read;
    fnFileWrite     write;
    fnFileGetPos    getpos;
    fnFileSeek      seek;
    fnFileSetPos    setpos;
    fnFileEOF       eof;
    fnFileSize      size;
    fnFileTell      tell;
} fileInterface_t;

// static interfaces
extern fileInterface_t fsFileLocalInterface_g;
extern fileInterface_t fsFileInZipInterface_g;



file_t*     FileOpen( const char* fileName, uint32_t flags );
ecode_t     FileClose( file_t* f );
ecode_t     FileFlush( file_t* f );
size_t      FileRead( void* dst, size_t size, file_t* f );
size_t      FileWrite( const void* src, size_t size, file_t* f );
ecode_t     FileGetPos( file_t* f, int64_t* pos );
ecode_t     FileSeek( file_t* f, int64_t offset, uint32_t origin );
ecode_t     FileSetPos( file_t* f, const int64_t* pos );
bool_t      FileEOF( file_t* f );
size_t      FileSize( file_t* f );
ssize_t     FileTell( file_t* f );


#endif //__FILE_H__