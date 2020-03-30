#include "fs_tree.h"

#include <core/fs.h>
#include <core/string.h>
#include <core/assert.h>
#include <core/common.h>
#include <core/memory.h>
#include <core/alloc.h>
#include <core/tree.h>
#include "zip.h"



#define MAX_FILENAME_LEN        260
#define MAX_ZIP_FILES           128

typedef struct {
    char            name[MAX_FILENAME_LEN]; // path to zip file
    archID_t        archID;     // unique identifier of the archive
} zipFileInfo_t;

// 
typedef struct {
    tree_t*         root;
    zipFileInfo_t   files[MAX_ZIP_FILES];
    int             lastZipIndex;
    alloc_t*        treeAllocator;
    alloc_t*        stringAllocator;
} fileSystemTree_t;

// tree file info flags
enum fsTreeFileFlags_t {
    FST_DIR     = 0x1       // is directory
};

// tree file info
typedef struct {
    zipFileInfo_t*  zipFileInfo;// pointer to zip file info
    char*       name;       // file or directory name
    uint16_t    nameLen;    // length of name
    uint16_t    flags;      // file flags
    uint32_t    offset;     // offset to Local file header
    uint32_t    method;     // compression method
    uint32_t    compresed;  // compressed file size
    uint32_t    uncompresed;// uncompressed file size
} fsTreeFileInfo_t;


fileSystemTree_t* fsTree_g;

/*
============
FsTreeNewNode

Allocate memory for a tree node
return values:
pointer     Returns a pointer to a new tree node
============
*/
static tree_t* FsTreeNewNode( void ) {
    return TreeNodeAllocate( fsTree_g->treeAllocator, sizeof(fsTreeFileInfo_t) );
}

/*
============
FsTreeNewString
============
*/
static char* FsTreeNewString( size_t size ) {
    return fsTree_g->stringAllocator->allocate( size );
}

/*
============
ReleaseTreeData
============
*/
static void ReleaseTreeData( void* data, void* userData ) {
    fsTreeFileInfo_t* info = (fsTreeFileInfo_t*)data;
    if( info->name != NULL ) {
        ((fnDeallocate)userData)(info->name);
    }
}

/*
============
FsTreeAppendPath

Add the file path to the tree.
return values:
NULL        If such a file already exists
pointer     If the new file was created successfully
============
*/
static tree_t* FsTreeAppendPath( const char* path ) {
    char* nextpath = (char*)path;
    char* name = (char*)path;
    size_t len;
    tree_t* cur;
    tree_t* last = fsTree_g->root;
    fsTreeFileInfo_t* info;
    tree_t* newNode;
    bool_t isdir;
    
    assert_not_null( path );
    assert( path[0] != 0 );
    
    // we go through all the subdirectories. All paths must be 
    // separated by a slash. If such a subdirectories does not 
    // exist, then create it. If the file already exists in the 
    // tree, then returns NULL
    while( *name ) {
        // next path
        while( *nextpath && *nextpath != '/' ) {
            nextpath++;
        }
        if( *nextpath == '/' ) {
            isdir = btrue;
            nextpath++;
        } else {
            isdir = bfalse;
        }
        len = nextpath - name;
        
        // find subdirectory
        cur = TreeFirstChild( last );
        while( cur ) {
            info = (fsTreeFileInfo_t*)(void*)cur->data;
            // compare names
            if( 0 == StrNCmp( info->name, name, len ) ) {
                last = cur;
                break;
            }
            cur = TreeNext( cur );
        }
        
        // if subdir not found then append subdir to the tree
        if( cur == NULL ) {
            newNode = FsTreeNewNode();
            info = (fsTreeFileInfo_t*)(void*)newNode->data;
            // copy file name
            info->name = FsTreeNewString( len + 1 );
            StrNCpy( info->name, name, len );
            info->nameLen = (uint16_t)len;
            info->name[len] = 0;
            info->flags = isdir ? FST_DIR : 0;
            
            TreeInsertLastChild( last, newNode );
            last = newNode;
        }
        
        // check for dir
        if( !isdir && cur != NULL ) {
            // file already exists
            return NULL;
        }
        
        name = nextpath;
    }
    
    return last;
}

/*
============
FsTreeFindFile
============
*/
static tree_t* FsTreeFindFile( const char* path ) {
    char* nextpath = (char*)path;
    char* name = (char*)path;
    size_t len;
    tree_t* cur;
    tree_t* last = fsTree_g->root;
    fsTreeFileInfo_t* info;
    bool_t isdir;
    
    assert_not_null( path );
    assert( path[0] != 0 );
    
    while( *name ) {
        // next path
        while( *nextpath && *nextpath != '/' ) {
            nextpath++;
        }
        if( *nextpath == '/' ) {
            isdir = btrue;
            nextpath++;
        } else {
            isdir = bfalse;
        }
        len = nextpath - name;
        
        // find subdirectory
        cur = TreeFirstChild( last );
        while( cur ) {
            info = (fsTreeFileInfo_t*)(void*)cur->data;
            // compare names
            if( 0 == StrNCmp( info->name, name, len ) ) {
                last = cur;
                break;
            }
            cur = TreeNext( cur );
        }
        
        // if subdir not found then return NULL
        if( cur == NULL ) {
            return NULL;
        }
        
        // check for dir
        if( !isdir && cur != NULL ) {
            // file is found
            return cur;
        }
        
        name = nextpath;
    }
    
    return NULL;
}

/*
============
FsTreeInit
============
*/
void FsTreeInit( void ) {
    fsTree_g = GetSystemAllocator()->allocate( sizeof(fileSystemTree_t) );
    
    fsTree_g->treeAllocator = GetFsTreeAllocator();
    fsTree_g->stringAllocator = GetFsTreeStringsAllocator();
    
    fsTree_g->root = FsTreeNewNode();
    MemSet( fsTree_g->root->data, 0, sizeof(fsTreeFileInfo_t) );
    fsTree_g->lastZipIndex = 0;
}

/*
============
FsTreeShutdown

We free the memory allocated for the tree and close all zip files
============
*/
void FsTreeShutdown( void ) {
    int i;
    
    TreeRelease( fsTree_g->root, fsTree_g->treeAllocator, 
            ReleaseTreeData, fsTree_g->stringAllocator->deallocate );
    for( i = 0; i < fsTree_g->lastZipIndex; i++ ) {
        //ZipClose( fsTree_g->zipFiles[i] );
    }
    MemSet( fsTree_g, 0, sizeof(fileSystemTree_t) );
    
    GetSystemAllocator()->deallocate( fsTree_g );
}

/*
============
FsTreeAppendArchive

Add the archive with data to the directory tree and files.
If the file is not an archive, then ignore it

return values:
0            if the archive has not been added 
number       archive id
============
*/
archID_t FsTreeAppendArchive( const char* zipName ) {
    zipf_t* zip;
    tree_t* node;
    size_t added = 0;
    fsTreeFileInfo_t* info;
    char buf[MAX_FILENAME_LEN];
    archID_t id = 0;
    
    if( fsTree_g->lastZipIndex >= MAX_ZIP_FILES ) {
        Warning( "too many downloaded archives\n" );
        return 0;
    }
    
    // open zip file
    if( NULL == (zip = ZipOpen( zipName, ZIP_READ )) ) {
        Warning( "correct archive file not found '%s'\n", zipName );
        return 0;
    }
    
    LogPrintf( "loading archive '%s' ...\n", zipName );
    
    // process inner zip files
    if( 0 == ZipFirstFile( zip ) ) {
        do {
            ZipGetCurrentFileName( zip, buf, sizeof(buf) );
            node = FsTreeAppendPath( buf );
            if( node == NULL ) {
                // file already exists
                Warning( "packaged file already exists '%s'\n", buf );
            } else {
                info = (fsTreeFileInfo_t*)(void*)node->data;
                if( !(info->flags & FST_DIR) ) {
                    info->zipFileInfo = &(fsTree_g->files[ fsTree_g->lastZipIndex ]);
                    
                    info->offset = zip->cdfh.offset;    // offset to Local file header
                    info->method = zip->cdfh.compress;  // compression method
                    info->compresed = zip->cdfh.compressSize;       // compressed file size
                    info->uncompresed = zip->cdfh.uncompressSize;   // uncompressed file size
                    
                    added++;
                }
            }
        } while( 0 == ZipNextFile( zip ) );
    }
    
    // append pointer to zip file info
    if( added > 0 ) {
        StrCpy( fsTree_g->files[ fsTree_g->lastZipIndex ].name, zipName );
        id = fsTree_g->lastZipIndex + 1;
        fsTree_g->files[ fsTree_g->lastZipIndex ].archID = id;
        fsTree_g->lastZipIndex++;
    }
    
    ZipClose( zip );
    
    return id;
}

/*
============
FsTreeGetFileInfo
============
*/
archID_t FsTreeGetFileInfo( const char* filename, archFileInfo_t* info ) {
    tree_t* node;
    fsTreeFileInfo_t* fileinfo;

    assert_not_null( filename );
    assert_not_null( info );

    // find the file by name
    node = FsTreeFindFile( filename );
    if( node == NULL ) {
        // file not found
        return 0;
    }
    // get zip file info
    fileinfo = (fsTreeFileInfo_t*)node->data;

    // copy info
    info->archName = fileinfo->zipFileInfo->name;   // archive name
    info->archID = fileinfo->zipFileInfo->archID;   // archive ID
    info->offset = fileinfo->offset;            // offset to Local File Header
    info->method = fileinfo->method;            // compression method
    info->compresed = fileinfo->compresed;      // compressed file size
    info->uncompresed = fileinfo->uncompresed;  // uncompressed file size

    return info->archID;
}

/*
============
FsTreePrintLoadedArchives
============
*/
void FsTreePrintLoadedArchives( void ) {
    int i;
    
    for( i = 0; i < fsTree_g->lastZipIndex; i++ ) {
        LogPrintf( "  %s\n", fsTree_g->files[i].name );
    }
}
