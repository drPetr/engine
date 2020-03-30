#ifndef __FS_H__
#define __FS_H__

#include <core/types.h>
#include <core/ecode.h>



// open modes
enum openMode_t {
    F_READ      =   0x01,       // open for read
    F_WRITE     =   0x02,       // open for write
    F_APPEND    =   0x04,       // open for append
    F_MODE_MASK =   0x0f,       // open mode mask
    
    F_LOCAL     =   0x10,       // local file
    F_INNER     =   0x20,       // inner file
    F_TYPE_MASK =   0xf0        // file type mask
};

// seek origin
enum fileOrigin_t {
    F_SEEK_SET  =   1,
    F_SEEK_CUR  =   2,
    F_SEEK_END  =   3
};

// file interface structure
struct fileInterface_t;

// file structure
typedef struct file_t {
    void*               file;       // file pointer
    struct fileInterface_t* fn;     // file interface
    uint32_t            flags;      // file flags
    
                        // for relative files
    ssize_t             offset;     // relative file offset
    size_t              fileSize;   // file offset in relative file
} file_t;



/*
================================================================

                            file system

================================================================
*/

void        FsInit( void );
// initialize file system

void        FsShutdown( void );
// finalize file system





/*
================================================================

                        file functions

================================================================
*/

file_t*     FileOpen( const char* fileName, uint32_t flags );
// open the file

ecode_t     FileClose( file_t* file );
// close the file

ssize_t     FileRead( void* dst, size_t size, file_t* file );
// read data from file

ssize_t     FileWrite( const void* src, size_t size, file_t* file );
// write data to file

ecode_t     FileFlush( file_t* file );
// flush file streams

ssize_t     FileTell( file_t* f );
// get file pointer position

ecode_t     FileSeek( file_t* file, ssize_t offset, uint32_t origin );
// seek to file position

bool_t      FileEOF( file_t* f );
// returns whether the end of the file has been reached

ssize_t     FileSize( file_t* f );
// returns size of the file





#endif //__FS_H__