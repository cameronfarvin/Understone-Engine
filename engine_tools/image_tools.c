#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _linux_
#include <unistd.h>
#endif

#include <assert.h>

#include <engine_tools/image_tools.h>

u8
uReadNextByte(uImage* const img)
{
    if (img->img_cursor < img->img_end)
    {
        return *img->img_cursor++;
    }

    uError_v("uReadNextByte(): Out of bounds.\n");

    // [ cfarvin::DEBUG ]
    assert(false);
    return 0; // [ cfarvin::NOTE ] silence msvc
}

u16
uRead16AsLE(uImage* const img)
{
    u16 tmp = uReadNextByte(img);
    return tmp + (uReadNextByte(img) << 8);
}

u32
uRead32AsLE(uImage* const img)
{
    u32 tmp = uRead16AsLE(img);
    return tmp + (uRead16AsLE(img) << 16);
}

bool
uLoadBitmap(const char* file_path, uImage* const img)
{
    if (!file_path || !strlen(file_path))
    {
        uError_v("uLoadBitmap(): Invalid file path.\n");
        return false;
    }

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable: 4996 )
#endif
    FILE* file = fopen(file_path, "rb");
#ifdef _WIN32
#pragma warning( pop)
#endif

    if (!file)
    {
        uError_v("uLoadBitmap(): Could not open file at location %s.\n", file_path);
        fclose(file);
        return false;
    }

    fseek(file, 0, SEEK_END);
    size_t original_bitmap_size = ftell(file);

    if (!original_bitmap_size)
    {
        uError_v("uLoadBitmap(): Invalid bitmap file.\n");
        return false;
    }

    // [ cfarvin::NOTE ] [ cfarvin::TODO ]
    // Need to intergrate with an Arena for more than this single image
    // in the future. This is a single test case
    if (!imageArena)
    {
        imageArena = uMAInit((u16)40000);
    }

    // [ cfarvin::TODO ] Ensure that conversion does not result in loss of data
    // for very very large assets. Extremely unlikely.

    // [ cfarvin::NOTE ] Replaced with Arena
    /* u8* mem_file = (u8*) malloc(original_bitmap_size + 1); */
    if (imageArena->arena_size < original_bitmap_size)
    {
        uError_v("Image arena not large enough for this image\n");
        return false;
    }

    // [ cfarvin::NOTE ] Removed.
    /* mem_file[original_bitmap_size] = '\0'; */
    fseek(file, 0, SEEK_SET);

    img->img_start = img->img_cursor = uMANext(imageArena);
    img->img_end = img->img_start + original_bitmap_size;

    size_t items_read = fread(uMANext(imageArena),
                              (size_t) original_bitmap_size,
                              1,
                              file);
    if (items_read != 1)
    {
        uError_v("uLoadBitmap(): File read error.\n");
        fclose(file);
        return false;
    }
    fclose(file);

    //
    // Parse BITMAPFILEHEADER
    //
    if (uReadNextByte(img) != 'B')
    {
        uError_v("uLoadBitmap(): Invalid bitmap header type.\n");
        return false;
    }

    if (uReadNextByte(img) != 'M')
    {
        uError_v("uLoadBitmap(): Invalid bitmap header type.\n");
        return false;
    }

    u32 bitmap_size = uRead32AsLE(img);

    uDBGPrint("bitmap_size: %d\n", bitmap_size);

    if (!bitmap_size)
    {
        uError_v("[ UE::IMAGE_TOOLS::WARNING ] uLoadBitmap(): Possible bad bitmap, indicates 0 size.\n");
    }

    if (uRead16AsLE(img) != 0)
    {
        uError_v("uLoadBitmap(): Invalid bitmap reserved bytes.\n");
        return false;
    }

    if (uRead16AsLE(img) != 0)
    {
        uError_v("uLoadBitmap(): Invalid bitmap reserved bytes.\n");
        return false;
    }

    // The offset, in bytes, from the beginning of the
    // BITMAPFILEHEADER structure to the bitmap bits.
    u32 bitmap_bits_offset = uRead32AsLE(img);
    u8* bitmap_bits_begin  = img->img_start + bitmap_bits_offset;

    //
    // Parse one of the following:
    //   BITMAPCOREHEADER:   12  bytes
    //   BITMAPINFOHEADER:   40  bytes
    //   BITMAPV4HEADER:     108 bytes
    //   BITMAPV5HEADER:     124 bytes
    //
    // The following are *not* currently supported:
    //   BITMAPV3INFOHEADER: 56 bytes
    //   OS22XBITMAPHEADER:  64 bytes
    //   BITMAPV2INFOHEADER: 52 bytes
    //
    u32 bitmap_info_header_size = uRead32AsLE(img);

    uDBGPrint("bitmap_info_header_size: %d\n", bitmap_info_header_size);
    if (bitmap_info_header_size > 124)
    {
        uError_v("uLoadBitmap(): Invalid bitmap header read.\n");
        return false;
    }

    if (bitmap_info_header_size != 12  &&
        bitmap_info_header_size != 40  &&
        bitmap_info_header_size != 108 &&
        bitmap_info_header_size != 124)
    {
        uError_v("uLoadBitmap(): Unsupported bitmap format.\n");
        return false;
    }

    //
    //
    // [ cfarvin::TODO ] Possibly remove this?
    //
    //
    /* typedef enum */
    /* { */
    /*     BOTTOM_UP, // origin is in the lower left corner */
    /*     TOP_DOWN   // origin is in the upper left corner */
    /* } BitmapDirection; */

    typedef struct tagCIEXYZ {
        s32 ciexyzX;
        s32 ciexyzY;
        s32 ciexyzZ;
    } CIEXYZ;

    typedef struct tagICEXYZTRIPLE {
        CIEXYZ ciexyzRed;
        CIEXYZ ciexyzGreen;
        CIEXYZ ciexyzBlue;
    } CIEXYZTRIPLE;

    /* BitmapDirection bitmap_direction; */

    // [ cfarvin:: TODO ]
    // BYTE ORDER UNIQUE TO: BITMAPCOREHEADER
    /* u32 bitmap_size          = 0; */
    /* u16 bitmap_width         = 0; */
    /* u16 bitmap_height        = 0; */
    /* u16 bitmap_planes        = 0; */
    /* u16 bitmap_bitCount      = 0; */

    // [ cfarvin::TODO ]
    // BYTE ORDER COMMON TO: BITMAPCOREHEADER, BITMAPINFOHEADER,
    // BITMAPV4HEADER, BITMAPV5HEADER
    /* u32 bitmap_size          = 0; */ // already read
    s32 bitmap_width         = 0;
    s32 bitmap_height        = 0;
    u16 bitmap_planes        = 0;
    u16 bitmap_bitCount      = 0;
    u32 bitmap_compression   = 0;
    u32 bitmap_sizeImage     = 0;
    s32 bitmap_xPelsPerMeter = 0;
    s32 bitmap_yPelsPerMeter = 0;
    u32 bitmap_clrUsed       = 0;
    u32 bitmap_clrImportant  = 0;

    // [ cfarvin::TODO ]
    // BYTE ORDER COMMON TO: BITMAPV4HEADER, BITMAPV5HEADER
    u32          bitmap_redMask    = 0;
    u32          bitmap_greenMask  = 0;
    u32          bitmap_blueMask   = 0;
    u32          bitmap_alphaMask  = 0;
    u32          bitmap_CSType     = 0;
    CIEXYZTRIPLE bitmap_endpoints  = { 0 };
    u32          bitmap_gammaRed   = 0;
    u32          bitmap_gammaGreen = 0;
    u32          bitmap_gammaBlue  = 0;

    /* // [ cfarvin::TODO ] */
    /* // BYTE ORDER UNIQUE TO BITMAPV5HEADER */
    u32 bitmap_intent      = 0;
    u32 bitmap_profileData = 0;
    u32 bitmap_profileSize = 0;
    u32 bitmap_reserved    = 0;

    uDBGPrint("Final (non-sequential) bitmap header type printed is type found\n");
    if (bitmap_info_header_size == 12) // BITMAPCOREHEADER ONLY
    {
        uDBGPrint("BITMAP HEADER TYPE: BITMAPCOREHEADER\n");
        bitmap_width = (s32) uRead16AsLE(img);
        bitmap_height = (s32) uRead16AsLE(img);
        bitmap_planes = uRead16AsLE(img);
        assert(bitmap_planes == 1);
        bitmap_bitCount = uRead16AsLE(img);
    }
    else // EVERYONE ELSE
    {
        uDBGPrint("BITMAP HEADER TYPE: BITMAPINFOHEADER\n");
        bitmap_width = (s32) uRead32AsLE(img);
        uDBGPrint("bitmap_width: %d\n", bitmap_width);
        bitmap_height = (s32) uRead32AsLE(img);
        uDBGPrint("bitmap_height: %d\n", bitmap_height);
        bitmap_planes = uRead16AsLE(img);
        uDBGPrint("bitmap_planes: %d\n", bitmap_planes);
        assert(bitmap_planes == 1);

        bitmap_bitCount = uRead16AsLE(img);
        uDBGPrint("bitmap_bitCount: %d\n", bitmap_bitCount);
        bitmap_compression = uRead32AsLE(img);
        assert((bitmap_compression == uBI_RGB)       ||
               (bitmap_compression == uBI_RLE8)      || // [ cfarvin::TODO ]
               (bitmap_compression == uBI_RLE4)      || // [ cfarvin::TODO ]
               (bitmap_compression == uBI_BITFIELDS) || // [ cfarvin::TODO ]
               (bitmap_compression == uBI_JPEG)      || // [ cfarvin::TODO ]
               (bitmap_compression == uBI_PNG)       || // [ cfarvin::TODO ]
               (bitmap_compression == uBI_CMYK)      || // [ cfarvin::TODO ]
               (bitmap_compression == uBI_CMYKRLE8)  || // [ cfarvin::TODO ]
               (bitmap_compression == uBI_CMYKRLE4));
        uDBGPrint("bitmap_compression: %d\n", bitmap_compression);
        bitmap_sizeImage = uRead32AsLE(img);
        uDBGPrint("bitmap_sizeImage: %d\n", bitmap_sizeImage);
        bitmap_xPelsPerMeter = (s32) uRead32AsLE(img);
        uDBGPrint("bitmap_xPelsPerMeter: %d\n", bitmap_xPelsPerMeter);
        bitmap_yPelsPerMeter = (s32) uRead32AsLE(img);
        uDBGPrint("bitmap_yPelsPerMeter: %d\n", bitmap_yPelsPerMeter);
        bitmap_clrUsed = uRead32AsLE(img);
        uDBGPrint("bitmap_clrUsed: %d\n", bitmap_clrUsed);
        bitmap_clrImportant = uRead32AsLE(img);
        uDBGPrint("bitmap_clrImportant: %d\n", bitmap_clrImportant);

        // [ cfarvin::UNTESTED ]
        // BITMAPV4HEADER && BITMAPV5HEADER
        if (bitmap_info_header_size >= 108)
        {
            uDBGPrint("BITMAP HEADER TYPE: BITMAPV4HEADER\n");
            bitmap_redMask = uRead32AsLE(img);
            uDBGPrint("bitmap_redMask: %d\n", bitmap_redMask);
            bitmap_greenMask = uRead32AsLE(img);
            uDBGPrint("bitmap_greenMask: %d\n", bitmap_greenMask);
            bitmap_blueMask = uRead32AsLE(img);
            uDBGPrint("bitmap_blueMask: %d\n", bitmap_blueMask);
            bitmap_alphaMask = uRead32AsLE(img);
            uDBGPrint("bitmap_alphaMask: %d\n", bitmap_alphaMask);
            bitmap_CSType = uRead32AsLE(img);
            uDBGPrint("bitmap_CSType: %d\n", bitmap_CSType);

            // [ cfarvin::VERIFY_CORRECTES ] [ cfarvin::UNTESTED ]
            bitmap_endpoints.ciexyzRed.ciexyzX   = (s32) uRead32AsLE(img);
            uDBGPrint("bitmap_endpoints.ciexyzRed.ciexyzX: %d\n",
                      bitmap_endpoints.ciexyzRed.ciexyzX);
            bitmap_endpoints.ciexyzRed.ciexyzY   = (s32) uRead32AsLE(img);
            uDBGPrint("bitmap_endpoints.ciexyzRed.ciexyzY: %d\n",
                      bitmap_endpoints.ciexyzRed.ciexyzY);
            bitmap_endpoints.ciexyzRed.ciexyzZ   = (s32) uRead32AsLE(img);
            uDBGPrint("bitmap_endpoints.ciexyzRed.ciexyzZ: %d\n",
                      bitmap_endpoints.ciexyzRed.ciexyzZ);
            bitmap_endpoints.ciexyzGreen.ciexyzX = (s32) uRead32AsLE(img);
            uDBGPrint("bitmap_endpoints.ciexyzGreen.ciexyZ: %d\n",
                      bitmap_endpoints.ciexyzGreen.ciexyzX);
            bitmap_endpoints.ciexyzGreen.ciexyzY = (s32) uRead32AsLE(img);
            uDBGPrint("bitmap_endpoints.ciexyzGreen.ciexyzY: %d\n",
                      bitmap_endpoints.ciexyzGreen.ciexyzY);
            bitmap_endpoints.ciexyzGreen.ciexyzZ = (s32) uRead32AsLE(img);
            uDBGPrint("bitmap_endpoints.ciexyzGreen.ciexyzZ: %d\n",
                      bitmap_endpoints.ciexyzGreen.ciexyzZ);
            bitmap_endpoints.ciexyzBlue.ciexyzX  = (s32) uRead32AsLE(img);
            uDBGPrint("bitmap_endpoints.ciexyzBlue.ciexyzX: %d\n",
                      bitmap_endpoints.ciexyzBlue.ciexyzX);
            bitmap_endpoints.ciexyzBlue.ciexyzY  = (s32) uRead32AsLE(img);
            uDBGPrint("bitmap_endpoints.ciexyzBlue.ciexyzY: %d\n",
                      bitmap_endpoints.ciexyzBlue.ciexyzY);
            bitmap_endpoints.ciexyzBlue.ciexyzZ  = (s32) uRead32AsLE(img);
            uDBGPrint("bitmap_endpoints.ciexyzBlue.ciexyZ: %d\n",
                      bitmap_endpoints.ciexyzBlue.ciexyzZ);

            /* uDBGPrint("bitmap_clrImportant: %d\n", bitmap_clrImportant);         */
            bitmap_gammaRed = uRead32AsLE(img);
            uDBGPrint("bitmap_gammaRed: %d\n", bitmap_gammaRed);
            bitmap_gammaGreen = uRead32AsLE(img);
            uDBGPrint("bitmap_gammaGreen: %d\n", bitmap_gammaGreen);
            bitmap_gammaBlue = uRead32AsLE(img);
            uDBGPrint("bitmap_gammaBlue: %d\n", bitmap_gammaBlue);

            // [ cfarvin::UNTESTED ]
            if (bitmap_info_header_size >= 124) // BITMAPV5HEADER ONLY
            {
                uDBGPrint("BITMAP HEADER TYPE: BITMAPV5HEADER\n");
                bitmap_intent = uRead32AsLE(img);
                uDBGPrint("bitmap_intent: %d\n", bitmap_intent);
                bitmap_profileData = uRead32AsLE(img);
                uDBGPrint("bitmap_profileData: %d\n", bitmap_profileData);
                bitmap_profileSize = uRead32AsLE(img);
                uDBGPrint("bitmap_profileSize: %d\n", bitmap_profileSize);
                bitmap_reserved = uRead32AsLE(img);
                uDBGPrint("bitmap_reserved: %d\n", bitmap_reserved);
            }
        }
    }

    if (bitmap_height > 0)
    {
        /* bitmap_direction = BOTTOM_UP; // origin in bottom left corner */
    }
    else
    {
        /* bitmap_direction = TOP_DOWN; // origin in top right corner */

        // Does not apply to the BITMAPCOREHEADER, applies to everyone else
        if ( (bitmap_info_header_size > 12) &&
             ((bitmap_compression != uBI_RGB) ||
              (bitmap_compression != uBI_BITFIELDS)) )
        {
            uError_v("uLoadBitmap(): Unsupported bitmap compression.\n");
            return false;
        }
    }

    if (!bitmap_width)
    {
        uError_v("uLoadBitmap(): Invliad bitmap height.\n");
        return false;
    }

    img->img_height = bitmap_height;
    img->img_width = bitmap_width;

    // [ cfarvin::TODO ]
    // If bV4Compression is BI_JPEG or BI_PNG, bV4Height specifies the height of the
    // JPEG or PNG image in pixels.

    //
    // [ cfarvin::TODO ] [ cfarvin::UNTESTED ] Parse RGBQuad
    //
    u8 intensityRed         = 0;
    u8 intensityGreen       = 0;
    u8 intensityBlue        = 0;
    u8 bitmap_reserved_quad = 0;

    // [ cfarvin::TODO ] What is a reasonable maximum for the padding needed between the RGBQuad
    // (if an RGBQuad exists), and the bitmap bits? Going w/ s16, though s8 is probably fine.
    s16 distance_to_bitmap_bits = (s16) (bitmap_bits_begin - img->img_cursor); // negative == error
    if (distance_to_bitmap_bits < 0)
    {
        uError_v("uLoadBitmap(): INTERNAL ERROR. Surpassed bitmap bits.\n");
        return false;
    }

    //
    // [ cfarvin::NOTE ] [ cfarvin::TODO ] [ cfarvin::UPDATE ]
    // This was seen in Microsoft documentation. It may or may not invalidate the information
    // given in the comments directly below:
    //
    // The bmiColors color table is used for optimizing colors used on palette-based devices,
    // and must contain the number of entries specified by the biClrUsed member of the BITMAPINFOHEADER.
    //

    // Zero indicates we are "at the gates" of the bitmap bits.
    // If greater than 0 but less than 4, either a mistake was made in parsing, or it is an
    // improperly formatted bitmap.
    if (distance_to_bitmap_bits)
    {
        if (distance_to_bitmap_bits < 4)
        {
            uError_v("uLoadBitmap(): Invalid bitmap RGBQuad format.\n");
            return false;
        }

        intensityRed   = uReadNextByte(img);
        intensityGreen = uReadNextByte(img);
        intensityBlue  = uReadNextByte(img);
        bitmap_reserved_quad = uReadNextByte(img);

        if (bitmap_reserved_quad != 0)
        {
            uError_v("uLoadBitmap(): Unsupported bitmap color intensities.\n");
            return false;
        }

        uDBGPrint("r: %d, g: %d, b:%d\n",
                  intensityRed,
                  intensityGreen,
                  intensityBlue);

        // [ cfarvin::TODO ] Continue parsing or use color intensities
    }


    /* size_t DEBUG_COUNT_PIXELS = 0; */
    /* u8 bitcount_modifier = 0; */
    /* if (bitmap_bitCount <= 8) */
    /* { */
    /*     bitcount_modifier = 8; */
    /* } */
    /* else if (bitmap_bitCount <= 16) */
    /* { */
    /*     bitcount_modifier = 16; */
    /* } */
    /* else */
    /* { */
    /*     bitcount_modifier = 32; */
    /* } */

    // [ cfarvin::TODO ] It would be less than great if img->img_pixels
    // already had been allocated. We are assuming that the user is
    // passing a new image object, not reusing one this is probably a
    // bad assumption in the long term; fine in the short term.
    // img->img_pixels = (u8*) malloc(bitmap_height * bitmap_width * bitcount_modifier);


    // [ cfarvin::REMOVED ]
    // Once we're ready to start using the loader, need to load the bits in via
    // arena rather than malloc
    /* img->img_pixels = (u8*) malloc(bitmap_height * bitmap_width * 3); */

/* #define uBITMAP_FILL_PIXELS                                     \ */
/*     for (ssize_t x = 0; x < bitmap_width; x++)                  \ */
/*     {                                                           \ */
/*         *(img->img_pixels + (y * x) + x) = uReadNextByte(img);  \ */
/*         DEBUG_COUNT_PIXELS++;                                   \ */
/*     }                                                           \ */


/*     if (bitmap_direction == BOTTOM_UP) */
/*     { */
/*         uError_v("Populating color bits: BOTTOM_UP...\n."); */
/*         for (ssize_t y = bitmap_height; y > 0; y--) */
/*         { */
/*             uBITMAP_FILL_PIXELS; */
/*         } */
/*     } */
/*     else */
/*     { */
/*         uError_v("Populating color bits: TOP_DOWN...\n."); */
/*         for (ssize_t y = 0; y > bitmap_height; y++) */
/*         { */
/*             uBITMAP_FILL_PIXELS; */
/*         } */
/*     } */

/* #undef uBITMAP_FILL_PIXELS */

    memcpy(img->img_pixels, bitmap_bits_begin, (bitmap_height * bitmap_width * 3));
    uDBGPrint("[ SUCCES ]\n");
    return true;
}
