#include <core/fs.h>
#include <core/assert.h>
#include <core/var.h>
#include <core/tree.h>
#include <core/alloc.h>
#include <core/common.h>
#include <core/string.h>

#include "file_helper.h"
#include "file_interface.h"
#include "fs_tree.h"
#include "dir_tree.h"
#include "archive_list.h"

#include <stdio.h>


var_t* inner_path_g;    // inner_path; use for file system
extern fileInterface_t fsFileLocalInterface_g;
extern fileInterface_t fsFileInnerInterface_g;
dirTree_t   dir;        // inner directory tree
dirNode_t*  root = NULL;       // pointer to root directory tree
//archInfo_t  archList;

/*
============
PathToLocal
============
*/
static char* PathToLocal( char* newPath, const char* path ) {
    size_t len;
    len = StrLen( inner_path_g->s );
    
    StrCpy( newPath, inner_path_g->s );
    StrCpy( newPath + len, path );
    return newPath;
}

/*
============
FsInit
============
*/
void FsInit( void ) {
    LogPrintf( "file system initialize ...\n" );
    inner_path_g = VarCreate( "inner_path", VAR_STR, "data/", NULL, "internal path to game archives" );
    //FsTreeInit();
    
    root = DTCreateRoot( &dir, GetFsTreeAllocator(), GetFsTreeStringsAllocator(), inner_path_g->s );
    //PKInfoCreate( &archList, GetArchiveFileNamesStringAllocator() );
}

/*
============
FsInit
============
*/
void FsShutdown( void ) {
    LogPrintf( "file system shutdown ...\n" );
    //FsTreeShutdown();
    DTFree( root );
    root = NULL;
    //PKInfoFree( &archList );
}

/*
============
FileOpen
============
*/
file_t* FileOpen( const char* fileName, uint32_t flags ) {
    file_t* f;
    const char* forOpen = fileName;
    char path[260];
    
    assert_not_null( forOpen );
    assert( FsFileCheckFlags(flags) );
    
    // allocate memory
    f = FsFileAllocate();
    
    // initialize file interface
    switch( flags & F_TYPE_MASK ) {
        case F_LOCAL:
            f->fn = &fsFileLocalInterface_g;
            break;
        case F_INNER:
            PathToLocal( path, fileName );
            if( SysFileExsists( path ) ) {
                f->fn = &fsFileLocalInterface_g;
                forOpen = path;
            } else {
                f->fn = &fsFileInnerInterface_g;
            }
            break;
    }
    
    // if file created
    if( f->fn->open( f, forOpen, flags ) ) {
        return f;
    }
    
    // else deallocate memory and return NULL pointer
    FsFileDeallocate( f );
    
    return NULL;
}


/*
============
FileClose
============
*/
ecode_t FileClose( file_t* file ) {
    ecode_t e;
    assert_not_null( file );
    assert_not_null( file->file );
    assert_not_null( file->fn->close );
    e = file->fn->close( file );
    FsFileDeallocate( file );
    return e;
}

/*
============
FileRead
============
*/
ssize_t FileRead( void* dst, size_t size, file_t* file ) {
    assert_not_null( file );
    assert_not_null( file->file );
    assert_not_null( dst );
    assert_not_null( file->fn->read );
    return file->fn->read(dst, size, file);
}

/*
============
FileWrite
============
*/
ssize_t FileWrite( const void* src, size_t size, file_t* file ) {
    assert_not_null( file );
    assert_not_null( file->file );
    assert_not_null( src );
    assert_not_null( file->fn->write );
    return file->fn->write( src, size, file );
}

/*
============
FileFlush
============
*/
ecode_t FileFlush( file_t* file ) {
    assert_not_null( file );
    assert_not_null( file->file );
    assert_not_null( file->fn->flush );
    return file->fn->flush( file );
}

/*
============
FileTell
============
*/
ssize_t FileTell( file_t* file ) {
    assert_not_null( file );
    assert_not_null( file->file );
    assert_not_null( file->fn->tell );
    return file->fn->tell( file );
}

/*
============
FileSeek
============
*/
ecode_t FileSeek( file_t* file, ssize_t offset, uint32_t origin ) {
    assert_not_null( file );
    assert_not_null( file->file );
    assert( FsFileCheckOrigin(origin) );
    assert_not_null( file->fn->seek );
    return file->fn->seek( file, offset, origin );
}

/*
============
FileEOF
============
*/
bool_t FileEOF( file_t* file ) {
    assert_not_null( file );
    assert_not_null( file->file );
    assert_not_null( file->fn->eof );
    return file->fn->eof( file );
}

/*
============
FileSize
============
*/
ssize_t FileSize( file_t* file ) {
    assert_not_null( file );
    assert_not_null( file->file );
    assert_not_null( file->fn->size );
    return file->fn->size( file );
}
