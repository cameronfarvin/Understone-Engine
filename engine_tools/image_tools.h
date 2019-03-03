#ifndef __image_tools__
#define __image_tools__ 1

#include <engine_tools/type_tools.h>


typedef struct
{
    size_t img_size;
    /* size_t img_height; */
    /* size_t img_width; */
    u8* img_start;
    u8* img_end;
    u8* img_cursor;
    uEndianness endian;
    u8* img_pixels;
} uImage;

u8  uReadNextByte(uImage* const img);
u16 uRead16AsLE(uImage* const img);
u32 uRead32AsLE(uImage* const img);

//
// BitMaps
//

typedef enum
    {
        BI_RGB = 0x0000,
        BI_RLE8 = 0x0001,
        BI_RLE4 = 0x0002,
        BI_BITFIELDS = 0x0003,
        BI_JPEG = 0x0004,
        BI_PNG = 0x0005,
        BI_CMYK = 0x000B,
        BI_CMYKRLE8 = 0x000C,
        BI_CMYKRLE4 = 0x000D
    } BitmapCompression;

bool uLoadBitmap(const char* file_path, uImage* const img);

#endif // __image_tools
