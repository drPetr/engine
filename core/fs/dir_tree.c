#include "dir_tree.h"

#include <core/string.h>
#include <core/common.h>



/*
============
DTNodeCreate
============
*/
static dirNode_t* DTNodeCreate( dirTree_t* dt, const char* path, uint16_t size ) {
    dirNode_t* node;
    
    node = dt->nodeAlloc->allocate( sizeof(dirNode_t) );
    TreeNodeSetToNULL( node );
    node->dirTree = dt;    // pointer to the directory tree header
    node->name = dt->strAlloc->allocate( size + 1 );   // allocate memory
    StrNCpy( node->name, path, size );  // copy path
    node->name[ size ] = 0;             // set '\0' symbol
    node->nameLen = size;               // length of 'name' string
    node->flags = 0;                    // flags set to zero
    node->crc32 = 0;                    // crc32 set to zero
    node->numOfCalls = 0;               // number of file accesses
    
    return node;
}

/*
============
DTAppendIfNotExists
============
*/
static bool_t DTAppendIfNotExists( dirNode_t* node, const char* path, dirNode_t** append ) {
    pathScanInfo_t pathInfo;
    int16_t i;
    dirNode_t* cur;
    dirNode_t* parent;
    dirNode_t* newNode;
    crc32_t crc;
    bool_t wasAppend = bfalse;
    
    // check for directory
    if( !(node->flags & DT_DIR) ) {
        Warning( "DTAppendIfNotExists(): cannot add directories to file '%s'\n", path );
        return bfalse;
    }
    
    // check path for correct
    if( 0 != PathScan( path, &pathInfo ) ) {
        Warning( "DTAppendIfNotExists(): invalid file path '%s'\n", path );
        return bfalse;
    }
    
    if( pathInfo.total == 0 ) {
        Warning( "DTAppendIfNotExists(): file path is empty\n", path );
        return bfalse;
    }
    
    crc = node->crc32;
    cur = node;
    for( i = 0; i < pathInfo.total; i++ ) {
        // update checksum
        crc = CRC32Update( crc, pathInfo.subdirs[i], pathInfo.lenghts[i] );
        
        parent = cur;
        cur = TreeFirstChild( cur );
        
        // find node by crc32 checksum
        while( cur ) {
            if( cur->crc32 == crc ) {
                // node found
                break;
            }
            cur = TreeNext( cur );
        }
        
        // if the node was not found
        // then create new node
        if( cur == NULL ) {
            newNode = DTNodeCreate( node->dirTree, pathInfo.subdirs[i], pathInfo.lenghts[i] );
            newNode->flags = (i+1 == pathInfo.fileNumber ? DT_FILE : DT_DIR);
            newNode->crc32 = crc;
            TreeInsertLastChild( parent, newNode );
            cur = newNode;
            wasAppend = btrue;
            *append = newNode;
        }
    }
    
    if( wasAppend ) {
        *append = newNode;
    }
    
    return wasAppend;
}

/*
============
DTFree_r
============
*/
static void DTFree_r( dirNode_t* node, dirTree_t* dt ) {
    if( !node ) {
        return;
    }
    
    // first view first child
    DTFree_r( TreeFirstChild( node ), dt );
    // then view next nodes
    DTFree_r( TreeNext( node ), dt );
    // after that deallocate the data
    if( node->name ) {
        dt->strAlloc->deallocate( node->name );
    }
    // finally deallocate the node
    dt->nodeAlloc->deallocate( node );
}

/*
============
DTPathTo_r
============
*/
static size_t DTPathTo_r( dirNode_t* node, char** path, size_t* maxLen, dirNode_t* stopNode ) {
    size_t size;
    size_t cpy;
    
    if( !node ) {
        return 0;
    }
    
    if( node == stopNode ) {
        return 0;
    }

    size = DTPathTo_r( TreeParent( node ), path, maxLen, stopNode );

    // copy size
    cpy = (size_t)node->nameLen;
    // clamp max copy size
    if( cpy >= *maxLen ) {
        cpy = *maxLen-1;
    }
    // copy string
    StrNCpy( *path, node->name, cpy );
    (*path)[cpy] = 0;
    
    *path += cpy;
    *maxLen -= cpy;
    
    return size + cpy;
}

/*
============
DTCreateRoot
============
*/
dirNode_t* DTCreateRoot( dirTree_t* dt, alloc_t* nodeAlloc, alloc_t* strAlloc, const char* rootName ) {
    size_t len;
    
    dt->nodeAlloc = nodeAlloc;
    dt->strAlloc = strAlloc;
    len = StrLen(rootName);
    dt->root = DTNodeCreate( dt, rootName, (uint16_t)len );
    
    dt->root->flags = DT_DIR | DT_ROOT;
    // calculate checksum
    dt->root->crc32 = CRC32Update( 0, rootName, len );
    
    return dt->root;
}

/*
============
DTFree
============
*/
void DTFree( dirNode_t* node ) {
    DTFree_r( node->dirTree->root, node->dirTree );
}

/*
============
DTAppend
============
*/
dirNode_t* DTAppend( dirNode_t* dir, const char* fileName ) {
    dirNode_t* append = NULL;
    if( DTAppendIfNotExists( dir, fileName, &append ) ) {
        return append;
    }
    return NULL;
}

/*
============
DTAppendToRoot
============
*/
dirNode_t* DTAppendToRoot( dirNode_t* dir, const char* fileName ) {
    dirNode_t* append = NULL;
    if( DTAppendIfNotExists( dir->dirTree->root, fileName, &append ) ) {
        return append;
    }
    return NULL;
}

/*
============
DTAttachArchiveInfo
============
*/
void DTAttachArchiveInfo( dirNode_t* dir, uint32_t archID, uint32_t offset ) {
    dir->flags |= DT_ARCH;
    dir->archID = archID;
    dir->offset = offset;
}

/*
============
DTAttachLocalInfo
============
*/
void DTAttachLocalInfo( dirNode_t* dir );

/*
============
DTDetachArchiveInfo
============
*/
void DTDetachArchiveInfo( dirNode_t* dir ) {
    dir->flags &= ~DT_ARCH;
}

/*
============
DTDetachLocalInfo
============
*/
void DTDetachLocalInfo( dirNode_t* dir ) {
    dir->flags &= ~DT_LOCAL;
}

/*
============
DTPathToLocal
============
*/
size_t DTPathToLocal( dirNode_t* node, char* path, size_t maxLen ) {
    return DTPathTo_r( node, &path, &maxLen, NULL );
}

/*
============
DTArchivePath
============
*/
size_t DTArchivePath( dirNode_t* node, char* path, size_t maxLen ) {
    return DTPathTo_r( node, &path, &maxLen, node->dirTree->root );
}

/*
============
DTFindFromRoot
============
*/
dirNode_t* DTFindFromRoot( dirNode_t* root, const char* fileName ) {
    return DTFindIn( root->dirTree->root, fileName );
}

/*
============
DTFindIn
============
*/
dirNode_t* DTFindIn( dirNode_t* node, const char* fileName ) {
    pathScanInfo_t pathInfo;
    int16_t i;
    dirNode_t* cur;
    crc32_t crc;
    
    // check path for correct
    if( 0 != PathScan( fileName, &pathInfo ) ) {
        return NULL;
    }
    
    if( pathInfo.total == 0 ) {
        return NULL;
    }
    
    crc = node->crc32;
    cur = node;
    for( i = 0; i < pathInfo.total; i++ ) {
        // update checksum
        crc = CRC32Update( crc, pathInfo.subdirs[i], pathInfo.lenghts[i] );
        
        cur = TreeFirstChild( cur );
        
        // find node by crc32 checksum
        while( cur ) {
            if( cur->crc32 == crc ) {
                // node found
                break;
            }
            cur = TreeNext( cur );
        }
        
        // if current node is NULL then return NULL
        if( cur == NULL ) {
            return NULL;
        }
    }
    
    cur->numOfCalls++;
    return cur;
}







void DTPrintSubtree( dirNode_t* node, char* str, size_t len ) {
    size_t oldLen = len;
        
    if( node == NULL ) {
        return;
    }
    
    len += node->nameLen;
    StrNCpy( str + oldLen, node->name, node->nameLen );
    str[len] = 0;
    
    LogPrintf( "  %c%c%c-%c%c", 
        (node->flags & DT_ROOT ? 'r' : '-'), 
        (node->flags & DT_DIR ? 'd' : '-'),
        (node->flags & DT_FILE ? 'f' : '-'),
        (node->flags & DT_LOCAL ? 'l' : '-'),
        (node->flags & DT_ARCH ? 'a' : '-') );
    
    LogPrintf( "   %s\n", str );
    
    DTPrintSubtree( TreeFirstChild( node ), str, len );
    DTPrintSubtree( TreeNext( node ), str, oldLen );
}