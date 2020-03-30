#ifndef __FILE_INTERFACE_H__
#define __FILE_INTERFACE_H__

#include <core/fs.h>

// interface
typedef file_t*(*fnFileOpen)(file_t*,const char*,uint32_t);
typedef ecode_t(*fnFileClose)(file_t*);
typedef ssize_t(*fnFileRead)(void*,size_t,file_t*);
typedef ssize_t(*fnFileWrite)(const void*,size_t,file_t*);
typedef ecode_t(*fnFileFlush)(file_t*);
typedef ssize_t(*fnFileTell)(file_t*);
typedef ecode_t(*fnFileSeek)(file_t*,ssize_t,uint32_t);
typedef bool_t (*fnFileEOF)(file_t*);
typedef ssize_t(*fnFileSize)(file_t*);


typedef struct fileInterface_t {
    fnFileOpen      open;
    fnFileClose     close;
    fnFileRead      read;
    fnFileWrite     write;
    fnFileFlush     flush;
    fnFileTell      tell;
    fnFileSeek      seek;
    fnFileEOF       eof;
    fnFileSize      size;
} fileInterface_t;

#endif //__FILE_INTERFACE_H__