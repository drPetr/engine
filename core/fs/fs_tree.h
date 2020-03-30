#ifndef __FS_TREE_H__
#define __FS_TREE_H__

#include <core/types.h>

typedef uint32_t    archID_t;

typedef struct {
    const char* archName;   // file to open
    archID_t    archID;     // unique identifier of the archive
    uint32_t    offset;     // (offset to Local File Header)if the file is in 
                            // the archive, then offset is used
    uint32_t    method;     // compression method
    uint32_t    compresed;  // compressed file size
    uint32_t    uncompresed;// uncompressed file size 
} archFileInfo_t;



void    FsTreeInit( void );
// initialize fs tree

void    FsTreeShutdown( void );
// finalize fs tree



archID_t FsTreeAppendArchive( const char* zipName );
// append zip archive to the tree
// return values:
// 0            if the archive has not been added 
// number       archive id

archID_t FsTreeGetFileInfo( const char* filename, archFileInfo_t* info );
// find file in fs tree
// return values:
// 0            if file not found
// number       id of the archive in which the file

void FsTreePrintLoadedArchives( void );
// print all loaded archives in to


#endif //__FS_TREE_H__