#ifndef __DIR_TREE_H__
#define __DIR_TREE_H__

#include <core/tree.h>
#include <core/alloc.h>
#include <core/types.h>
#include <core/hash.h>


/*
================================================================
Implementing an Internal Directory Tree

Directory names always end in a slash '/'
================================================================
*/



// directory tree flags
enum dirTreeFlags_t{
    DT_DIR      = 0x01,         // is directory
    DT_FILE     = 0x02,         // is file
    DT_ROOT     = 0x04,         // is root directory
    DT_LOCAL    = 0x08,         // there is a local file
    DT_ARCH     = 0x10          // there is an archive file
};


struct dirTree_t;

typedef struct {
    tree_t              treeNode;
    struct dirTree_t*   dirTree;    // pointer to the directory tree header
    char*               name;       // file or directory name
    uint16_t            nameLen;    // length of 'name' string
    uint16_t            flags;      // flags
    crc32_t             crc32;      // hash from the root of the directory to the 
                                    // name of the current directory or file
    uint32_t            archID;     // archive number (if flag DT_ARCH is set)
    uint32_t            offset;     // offset in the archive (if flag DT_ARCH is set)
    size_t              numOfCalls; // number of file accesses
} dirNode_t;

typedef struct dirTree_t {
    dirNode_t*          root;       // name of the directory in which the files are stored
    alloc_t*            nodeAlloc;  // node allocator
    alloc_t*            strAlloc;   // string allocator
} dirTree_t;



dirNode_t*  DTCreateRoot( dirTree_t* dt, alloc_t* nodeAlloc, alloc_t* strAlloc, const char* rootName );

void        DTFree( dirNode_t* node );

dirNode_t*  DTAppend( dirNode_t* dir, const char* fileName );

dirNode_t*  DTAppendToRoot( dirNode_t* dir, const char* fileName );

void        DTAttachArchiveInfo( dirNode_t* dir, uint32_t archID, uint32_t offset );

void        DTAttachLocalInfo( dirNode_t* dir );

void        DTDetachArchiveInfo( dirNode_t* dir );

void        DTDetachLocalInfo( dirNode_t* dir );

size_t      DTPathToLocal( dirNode_t* node, char* path, size_t maxLen );

size_t      DTArchivePath( dirNode_t* node, char* path, size_t maxLen );

dirNode_t*  DTFindFromRoot( dirNode_t* root, const char* fileName );

dirNode_t*  DTFindIn( dirNode_t* node, const char* fileName );


#define     DTIsFile(d)             (!!(((dirNode_t*)(void*)(d))->flags & DT_FILE))
#define     DTIsDir(d)              (!!(((dirNode_t*)(void*)(d))->flags & DT_DIR))
#define     DTIsRoot(d)             (!!(((dirNode_t*)(void*)(d))->flags & DT_ROOT))
#define     DTIsInArchive(d)        (!!(((dirNode_t*)(void*)(d))->flags & DT_ARCH))
#define     DTIsInLocal(d)          (!!(((dirNode_t*)(void*)(d))->flags & DT_LOCAL))
#define     DTArchiveInfoOffset(d)  (((dirNode_t*)(void*)(d))->offset)
#define     DTArchiveInfoID(d)      (((dirNode_t*)(void*)(d))->archID)



void DTPrintSubtree( dirNode_t* node, char* str, size_t len );



#endif //__DIR_TREE_H__