#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <stdio.h>
#include <stdlib.h>

//#include "types.h"
/*
extern frgba_t clBlack;
extern frgba_t clRed;
extern frgba_t clGreen;
extern frgba_t clBlue;
extern frgba_t clYellow;
extern frgba_t clMagenta;
extern frgba_t clCyan;
extern frgba_t clWhite;
extern frgba_t clLtGrey;
extern frgba_t clMdGrey;
extern frgba_t clDkGrey;
*/
#define BITMAP_RGB      0L  // An uncompressed format.
#define BITMAP_RLE8	    1L  // A run-length encoded (RLE) format for bitmaps
                            // with 8 bpp. The compression format is a 2-byte
                            // format consisting of a count byte followed by a
                            // byte containing a color index. For more
                            // information, see Bitmap Compression.
#define BITMAP_RLE4	    2L  // An RLE format for bitmaps with 4 bpp. The
                            // compression format is a 2-byte format consisting
                            // of a count byte followed by two word-length
                            // color indexes. For more information, see Bitmap
                            // Compression.
#define BITMAP_BITFIELDS 3L // Specifies that the bitmap is not compressed and
                            // that the color table consists of three DWORD
                            // color masks that specify the red, green, and
                            // blue components, respectively, of each pixel.
                            // This is valid when used with 16- and 32-bpp
                            // bitmaps.
#define BITMAP_JPEG     4L  // Indicates that the image is a JPEG image.
#define BITMAP_PNG      5L  // Indicates that the image is a PNG image.

#define IMAGE_FMT_BYTE1     0x11
#define IMAGE_FMT_BYTE2     0x12
#define IMAGE_FMT_BYTE3     0x13
#define IMAGE_FMT_BYTE4     0x14
#define IMAGE_FMT_FLOAT3    0x21
#define IMAGE_FMT_FLOAT4    0x22

#define BITMAP_SIGNATURE         0x4d42

typedef unsigned char   byte_t;
typedef unsigned short  word_t;
typedef unsigned int    dword_t;
typedef long            long_t;

#pragma pack(push, 1)
typedef struct {
    word_t      signature;  // The file signature; must be BM.
    dword_t     size;       // The size, in bytes, of the bitmap file. 
    word_t      reserved1;  // Reserved; must be zero.
    word_t      reserved2;  // Reserved; must be zero.
    dword_t     offset;     // The offset, in bytes, from the beginning of the
                            // bitmapHeader_t structure to the bitmap bits.
} bitmapFileHeader_t;

typedef struct {
    dword_t     size;       // The number of bytes required by the structure.
    long_t      width;      // The width of the bitmap, in pixels.
    long_t      height;     // The height of the bitmap, in pixels. If biHeight
                            // is positive, the bitmap is a bottom-up DIB and
                            // its origin is the lower-left corner. If biHeight
                            // is negative, the bitmap is a top-down DIB and
                            // its origin is the upper-left corner.
    word_t      planes;     // The number of planes for the target device.
                            // This value must be set to 1.
    word_t      bitCount;   // The number of bits-per-pixel. The biBitCount
                            // member of the BITMAPINFOHEADER structure 
                            // determines the number of bits that define each
                            // pixel and the maximum number of colors in the
                            // bitmap. This member must be one of the following
                            // values.
    dword_t     compression;// 
    dword_t     imageSize;  // The size, in bytes, of the image. This may be
                            // set to zero for BI_RGB bitmaps.
    long_t      xPxPerMeter;// The horizontal resolution, in 
                            // pixels-per-meter, of the target device for the
                            // bitmap. An application can use this value to
                            // select a bitmap from a resource group that best
                            // matches the characteristics of the current
                            // device.
    long_t      yPxPerMeter;// The vertical resolution, in pixels-per-meter, of
                            // the target device for the bitmap.
    dword_t     colorUsed;  // The number of color indexes in the color table
                            // that are actually used by the bitmap. If this
                            // value is zero, the bitmap uses the maximum
                            // number of colors corresponding to the value of
                            // the biBitCount member for the compression mode
                            // specified by biCompression.
    dword_t     colorImportant; // The number of color indexes that are
                            // required for displaying the bitmap. If this
                            // value is zero, all colors are required.
} bitmapInfoHeader_t;

typedef struct {
    byte_t      blue;       // Blue color
    byte_t      green;      // Green color
    byte_t      red;        // Red color
    byte_t      reserved;   // Reserved (alpha color)
} bitmapRgbQuad_t;

typedef struct {
    byte_t      blue;       // Blue color
    byte_t      green;      // Green color
    byte_t      red;        // Red color
} rgb_t;
#pragma pack(pop)

typedef struct bitmap_s {
    struct bitmap_s* ref;   // Ref to the original image
    rgb_t*      palette;    // Pointer to the palette
    byte_t*     data;       // Pointer to the image data
    dword_t     paletteSize;// Number of elements in the palette
    dword_t     dataSize;   // Size of data in bytes
    dword_t     stride;     // Size of the row pixels
    word_t      pxfmt;      // Pixel format
    word_t      bpp;        // Bytes per pixel
    long_t      left;       // 
    long_t      top;        // 
    long_t      width;      // Image width
    long_t      height;     // Image height
} bitmap_t;



// b - back; f - front; a - alpha
#define AlphaBlend(b,f,a)   ((b)+((f)-(b))*(a))

int BitmapLoadFromFile( FILE* f, bitmap_t* bmp );

int BitmapSaveToFile( FILE* f, bitmap_t* bmp );

void BitmapCreate( bitmap_t* bmp, int width, int height, word_t pxfmt );

void BitmapFree( bitmap_t* bmp );

void BitmapResize( bitmap_t* bmp, int width, int height );

void BitmapArea( bitmap_t* dst, bitmap_t* src, int left, int top, int width, int height );

int BitmapCopy( bitmap_t* dst, bitmap_t* src, word_t pxfmt );


/*
int BitmapImageArea( bitmap_t* dst, bitmap_t* src, int left, int top, int width, int height );
*/

#endif //__BITMAP_H__