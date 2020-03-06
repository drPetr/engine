#ifndef __FILE_HELPER_H__
#define __FILE_HELPER_H__

#include <core/fs.h>


file_t*     FsFileAllocate( void );

void        FsFileDeallocate( file_t* f );

bool_t      FsFileCheckFlags( uint32_t flags );

bool_t      FsFileCheckOrigin( uint32_t origin );

#endif //__FILE_HELPER_H__