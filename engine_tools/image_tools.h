#ifndef __image_tools__
#define __image_tools__ 1

#include <engine_tools/type_tools.h>


//
// BitMaps
//
/*
Bitmap files generally follow the form:
   1. Bitmap File Header
   2. Bitmap Info Header
   3. RGBQUAD Array
   4. Color-Index Array

Bitmap File Header:
typedef struct tagBITMAPFILEHEADER {
  WORD  bfType;                                               // 16 bytes
  DWORD bfSize;                                               // 32 bytes
  WORD  bfReserved1;                                          // 16 bytes
  WORD  bfReserved2;                                          // 16 bytes
  DWORD bfOffBits;                                            // 32 bytes
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;
*/
typedef struct
{
    size_t bitmap_size;
} uBitmap;

bool uLoadBitmap(const char* file_path, uBitmap* const bitmap);

#endif // __image_tools
