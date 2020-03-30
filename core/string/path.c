#ifndef __PATH_H__
#define __PATH_H__

#include <core/string.h>

enum pathScanToken_t {
    PST_OK      = 0,
    PST_DIR     = 1,
    PST_FILE    = 2,
    PST_ERROR   = 3
};

typedef struct {
    const char*     begin;
    const char*     forward;
    uint16_t        len;
    int             tk;
} pathScan_t;

/*
============
PathScanNext
============
*/
static int PathScanNext( pathScan_t* s ) {
    uint8_t isdir = 0;
#define b   (s->begin)
#define f   (s->forward)
#define l   (s->len)
#define tk  (s->tk)

    // skip slashes
    while( *f == DIR_SLASH ){
        f++;
    }
    
    // scan dir name
    b = f;
    while( *f && *f != DIR_SLASH ) {
        f++;
    }
    if( *f == DIR_SLASH ) {
        f++;
        isdir = 1;
    }
    l = f - b;
    
    if( l == 0 ) {
        return tk = PST_OK;
    } else if( isdir ){
        return tk = PST_DIR;
    }
    
    return tk = PST_FILE;
#undef b
#undef f
#undef l
#undef tk
}

/*
============
PathScan
============
*/
int PathScan( const char* path, pathScanInfo_t* info ) {
    pathScan_t scan;
    
    // init scaner
    scan.begin = path;
    scan.forward = path;
    scan.len = 0;
    scan.tk = PST_OK;
    
    info->total = 0;
    info->fileNumber = -1;
    
    // scan subdirs
    while( PathScanNext( &scan ) != PST_OK ) {
        if( scan.tk == PST_ERROR ) {
            info->total++;
            return -1;
        }
        if( info->total >= MAX_SUBDIRS ) {
            return -1;
        }
        if( scan.tk == PST_DIR ) {
            info->subdirs[ info->total ] = scan.begin;
            info->lenghts[ info->total ] = scan.len;
            info->total++;
        } else if( scan.tk == PST_FILE ) {
            info->subdirs[ info->total ] = scan.begin;
            info->lenghts[ info->total ] = scan.len;
            info->total++;
            info->fileNumber = info->total;
            return 0;
        } else {
            return -1;
        }
    }
    
    return 0;
}

#endif //__PATH_H__