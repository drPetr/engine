#include "archive_list.h"

/*
============
PKInfoCreate
============
*//*
void PKInfoCreate( archInfo_t* info, alloc_t* strAlloc ) {
    info->archivesOpen = 0;
    info->strAlloc = strAlloc;
}*/

/*
============
PKInfoFree
============
*//*
void PKInfoFree( archInfo_t* info ) {
    uint32_t i;
    
    for( i = 0; i < info->archivesOpen; i++ ) {
        info->strAlloc->deallocate( info->pathsToArchives[ i ] );
        fclose( info->openArchiveFiles[ i ] );
    }
    info->archivesOpen = 0;
}*/