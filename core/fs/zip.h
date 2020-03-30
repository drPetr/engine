#ifndef __ZIP_H__
#define __ZIP_H__

#include <core/types.h>
#include <core/ecode.h>
#include <core/fs.h>

// dependent document
// https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT

#define ZIP_LFH_SIGNATURE   0x04034b50
#define ZIP_CDFH_SIGNATURE  0x02014b50
#define ZIP_EOCD_SIGNATURE  0x06054b50

#define ZIP_SUPPORT_VERSION 20

#define ZIP_FILENAME_MAX_LEN    1024
#define ZIP_EXTRA_FIELD_MAX_LEN 1024
#define ZIP_FILE_COMMENT_MAX_LEN    1024


enum zipCompres_t {
    ZIP_NO_COMPRESS = 0,
    ZIP_DEFLATE = 8
};

enum zipFlags_t {
    ZIP_F_ENC_FILE  = BIT(0),   // If set, indicates that the file is encrypted.
    ZIP_F_DATDESCR  = BIT(3),
    ZIP_F_ENC_CD    = BIT(13)
};

#pragma pack(push,1)

/*
    [local file header 1]
    [encryption header 1]
    [file data 1]
    [data descriptor 1]
    . 
    .
    .
    [local file header n]
    [encryption header n]
    [file data n]
    [data descriptor n]
    [archive decryption header] 
    [archive extra data record] 
    [central directory header 1]
    .
    .
    .
    [central directory header n]
    [zip64 end of central directory record]
    [zip64 end of central directory locator] 
    [end of central directory record]
*/

//
// reductions
//
// CD/cd -      central directory
// fname -      file name
// ftime -      file time
// fdate -      file date
// fcom  -      file comment
//


typedef struct {
    int             sec;
    int             min;
    int             hour;
    int             day;
    int             mon;
    int             year;
} zipDateTime_t;

// Local file header (30 bytes)
typedef struct {
    uint32_t        signature;      // local file header signature (0x04034b50)
    uint16_t        extractVer;     // version needed to extract
    uint16_t        flags;          // general purpose bit flag
    uint16_t        compress;       // compression method
    uint16_t        ftime;          // last mod file time
    uint16_t        fdate;          // last mod file date
    uint32_t        crc32;          // crc-32
    uint32_t        compressSize;   // compressed size
    uint32_t        uncompressSize; // uncompressed size
    uint16_t        fnameLen;       // file name length
    uint16_t        extraLen;       // extra field length
    // file name (variable size)
    // extra field (variable size)
} zipLocalHeader_t;


// Data descriptor (12 bytes)
typedef struct {
    uint32_t        crc32;          // crc-32
    uint32_t        compressSize;   // compressed size
    uint32_t        uncompressSize; // uncompressed size
} zipDataDescr_t;


// Central directory file header (46 bytes)
typedef struct {
    uint32_t        signature;      // central file header signature (0x02014b50)
    uint16_t        madeBy;         // version made by
    uint16_t        extractVer;     // version needed to extract
    uint16_t        flags;          // general purpose bit flag
    uint16_t        compress;       // compression method
    uint16_t        ftime;          // last mod file time
    uint16_t        fdate;          // last mod file date
    uint32_t        crc32;          // crc-32
    uint32_t        compressSize;   // compressed size
    uint32_t        uncompressSize; // uncompressed size
    uint16_t        fnameLen;       // file name length
    uint16_t        extraLen;       // extra field length
    uint16_t        fcomLen;        // file comment length
    uint16_t        diskStart;      // disk number start
    uint16_t        interAttr;      // internal file attributes
    uint32_t        exterAttr;      // external file attributes
    uint32_t        offset;         // relative offset of local header
    // file name (variable size)
    // extra field (variable size)
    // file comment (variable size)
} zipCDFileHeader_t;


// Digital signature (6 bytes)
typedef struct {
    uint32_t        signature;      // header signature (0x05054b50)
    uint16_t        dataSize;       // size of data
    // signature data (variable size)
} zipDigitalSignature_t;


// End of central directory record (22 bytes)
typedef struct {
    uint32_t        signature;      // end of central dir signature (0x06054b50)
    uint16_t        diskNum;        // number of this disk
    uint16_t        diskNumCD;      // number of the disk with the start of the central directory
    uint16_t        totalNumInDisk; // total number of entries in the central directory on this disk
    uint16_t        totalNumInCD;   // total number of entries in the central directory
    uint32_t        sizeOfCD;       // size of the central directory
    uint32_t        offset;         // offset of start of central directory with respect to the starting disk number
    uint16_t        comLen;         // ZIP file comment length
    // ZIP file comment (variable size)
} zipEndOfCD_t;

#pragma pack (pop)




typedef int(*fnZipProcCD)( file_t*,const zipCDFileHeader_t*,const char*,const char*,const char*,void*);
//                        file, cdfh, file name, extra, file comment, user data
// return values can be: 0 if all ok, or -1 if you need to interrupt the process
// other return values are erroneous


typedef int(*fnZipProc)( file_t*,const zipLocalHeader_t*,ssize_t,const char*,const char*,void*);
//                        file, lfh, offset_of_data, file name, extra, user data
// return values can be: 0 if all ok, or -1 if you need to interrupt the process
// other return values are erroneous


// use for ZipFirstFile and ZipNextFile functions
/*
typedef struct {
    file_t*         file;       // zip file
    ssize_t         offset;     // start offset data
    ssize_t         size;       // size of data
    ssize_t         fileNameLen;// length of the file name
    char            fileName[ZIP_FILENAME_MAX_LEN]; // filename
    
    ssize_t         local;      // here the local offset of the current file is stored
} zipDataInfo_t;*/



void DosTimeToZipTime( uint16_t date, uint16_t time, zipDateTime_t* tm );
// date format:
// 0-4      Day of the month (1–31)
// 5-8      Month (1 = January, 2 = February, and so on)
// 9-15     Year offset from 1980 (add 1980 to get actual year)
// time format:
// 0-4      Second divided by 2
// 5-10     Minute (0–59)
// 11-15    Hour (0–23 on a 24-hour clock) 

void ZipTimeToDosTime( const zipDateTime_t* tm, uint16_t* date, uint16_t* time );
// date format:
// 0-4      Day of the month (1–31)
// 5-8      Month (1 = January, 2 = February, and so on)
// 9-15     Year offset from 1980 (add 1980 to get actual year)
// time format:
// 0-4      Second divided by 2
// 5-10     Minute (0–59)
// 11-15    Hour (0–23 on a 24-hour clock)



ecode_t ZipProcessCD( file_t* zip, fnZipProcCD fnproc, void* userData );
// process central directory
// return values:
// E_OK        if read
// E_UNKFF     unknown file format
// E_NORDF     no data to read
// E_READ      read error
// E_NOMEM     not enough memory
// E_UNKCODE   unknown code

ecode_t ZipProcess( file_t* zip, fnZipProc fnproc, void* userData );
// process files in zip archive without use central directory
// return values:
// E_OK        if read
// E_READ      read error
// E_NOMEM     not enough memory
// E_UNKCODE   unknown code

ecode_t ZipCheck( file_t* zip );
// check the correctness of the data in the archive

/*
void* ZipFirstFile( zipDataInfo_t* dataInfo );
void* ZipNextFile( void* h );
*/










enum zipOpenMode_t {
    ZIP_READ        = 0x01,
    ZIP_WRITE       = 0x02,
    ZIP_APPEND      = 0x04
};


// zip file structure
typedef struct {
    void*               file;   // pointer to the FILE
    zipEndOfCD_t        eocd;   // end of central directory structure
    ssize_t             curOffset;  // current offset
    int                 curNum;     // current file number
    zipCDFileHeader_t   cdfh;   // current file
    uint32_t            om;     // open mode
    bool_t              stop;   
    bool_t              loaded; // zip file is loaded
} zipf_t;





zipf_t* ZipOpen( const char* zipFile, uint32_t om );
// open zip file and return pointer to the opened zip file


int     ZipClose( zipf_t* zip );
// close the zip file


int     ZipFirstFile( zipf_t* zip );
// find first file in zip archive
// return values:
// E_OK         if the file is found
// E_NOTFOUND   if the file is not found


int     ZipNextFile( zipf_t* zip );
// find next file in zip archive
// return values:
// E_OK         if the next file is found
// E_NOTFOUND   if there are no more files


ssize_t ZipGetCurrentFileName( zipf_t* zip, char* name, size_t maxSize );
// copy file name to string 'name'
// return values:
// -1           if the file is no selected
// size         copied row size


ssize_t ZipTotalFiles( zipf_t* zip );
// return the total number of files in zip
// return values:
// -1           if there was an error
// size         number of files in zip







#endif //__ZIP_H__