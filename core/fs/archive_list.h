#ifndef __ARCH_LIST_H__
#define __ARCH_LIST_H__

#include <core/types.h>
#include <core/string.h>
#include <core/alloc.h>

#include <stdio.h>

#define MAX_ARCHIVES                    64
#define MAX_OPEN_ARCHIVE_STREAMS        (MAX_ARCHIVES * 2)


/*
typedef uint32_t archID_t;

typedef struct {
    char*           archPath;       // path to archive
    FILE*           file;           // 
    uint32_t        numOpenFiles;   // number open files
    archID_t        archID;         // archive identificator
} archFileInfos_t;

typedef struct {
    archID_t        id;
    bool_t          noClosingFile;
    FILE*           file;
} archStream_t;

typedef struct {
    archFileInfos_t  filesInfo[ MAX_ARCHIVES ];
    archStream_t    streams[ MAX_OPEN_ARCHIVE_STREAMS ];
    uint32_t        archives;       // number of archives
    uint32_t        openStreams;    // 
    alloc_t*        strAlloc;       // string allocator
} archInfo_t;



void        PKInfoCreate( archInfo_t* info, alloc_t* strAlloc );

void        PKInfoFree( archInfo_t* info );

archID_t    PKAppendArchive( archInfo_t* info, const char* archFileName );

archStream_t* PKOpenStream( archID_t id );

void        PKCloseStream( archFileInfos_t* arch );
*/





#endif //__ARCH_LIST_H__