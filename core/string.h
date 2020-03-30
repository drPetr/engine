#ifndef __STRING_H__
#define __STRING_H__

#include <core/string/str_base.h>
#include <core/types.h>

#ifdef MAX_PATH
#undef MAX_PATH
#endif

#define MAX_PATH        260
#define DIR_SLASH       '/'
#define MAX_SUBDIRS     32

typedef struct {
    const char*     subdirs[MAX_SUBDIRS];   // subdirectories
    uint16_t        lenghts[MAX_SUBDIRS];
    uint16_t        total;
    int16_t         fileNumber;
} pathScanInfo_t;



int PathScan( const char* path, pathScanInfo_t* info );
// fills the directory structure
// return values:
// 0        if the path is scanned
// -1       if there was an error


#endif //__STRING_H__