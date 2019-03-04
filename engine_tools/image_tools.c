#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <engine_tools/image_tools.h>

u8
uReadNextByte(uImage* const img)
{
    if (img->img_cursor < img->img_end)
    {
        return *img->img_cursor++;
    }

    printf("[ UE::IMAGE_TOOLS::ERROR ] uReadNextByte(): Out of bounds.\n");
    assert(false);
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
        printf("[ UE::IMAGE_TOOLS::ERROR ] uLoadBitmap(): Invalid file path.\n");
        return false;
    }

    FILE* file = fopen(file_path, "rb");
    if (!file)
    {
        printf("[ UE::IMAGE_TOOLS::ERROR ] uLoadBitmap(): Could not open file.\n");
        fclose(file);
        return false;
    }

    fseek(file, 0, SEEK_END);
    img->img_size = ftell(file);

    if (!img->img_size)
    {
        printf("[ UE::IMAGE_TOOLS::ERROR ] uLoadBitmap(): Invalid bitmap file.\n");
        return false;
    }

    // [ cfarvin::NOTE ] [ cfarvin::TODO ]
    // For now, let's assume that the user is passing reasonable file sizes, and that it
    // is safe to read the entire file into memory.
    u8* mem_file = (u8*) malloc(img->img_size + 1);
    mem_file[img->img_size] = '\0';
    fseek(file, 0, SEEK_SET);

    img->img_start = img->img_cursor = mem_file;
    img->img_end = mem_file + img->img_size;

    size_t items_read = fread(mem_file, img->img_size, 1, file);
    if (items_read != 1)
    {
        printf("[ UE::IMAGE_TOOLS::ERROR ] uLoadBitmap(): File read error.\n");
        fclose(file);
        return false;
    }
    fclose(file);

    //
    // Parse BITMAPFILEHEADER
    //
    if (uReadNextByte(img) != 'B')
    {
        printf("[ UE::IMAGE_TOOLS::ERROR ] uLoadBitmap(): Invalid bitmap header type.\n");
        return false;
    }

    if (uReadNextByte(img) != 'M')
    {
        printf("[ UE::IMAGE_TOOLS::ERROR ] uLoadBitmap(): Invalid bitmap header type.\n");
        return false;
    }

    u32 bitmap_size = uRead32AsLE(img);
    printf("\t[ debug ] bitmap_size: %d\n", bitmap_size);
    if (!bitmap_size)
    {
        printf("[ UE::IMAGE_TOOLS::WARNING ] uLoadBitmap(): Possible bad bitmap, indicates 0 size.\n");
    }

    if (uRead16AsLE(img) != 0)
    {
        printf("[ UE::IMAGE_TOOLS::ERROR ] uLoadBitmap(): Invalid bitmap reserved bytes.\n");
        return false;
    }

    if (uRead16AsLE(img) != 0)
    {
        printf("[ UE::IMAGE_TOOLS::ERROR ] uLoadBitmap(): Invalid bitmap reserved bytes.\n");
        return false;
    }

    uRead32AsLE(img); // progress cursor

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

    printf("\t[ debug ] bitmap_info_header_size: %d\n", bitmap_info_header_size);
    if (bitmap_info_header_size > 124)
    {
        printf("[ UE::IMAGE_TOOLS::ERROR ] uLoadBitmap(): Invalid bitmap header read.\n");
        return false;
    }

    if (bitmap_info_header_size != 12  &&
        bitmap_info_header_size != 40  &&
        bitmap_info_header_size != 108 &&
        bitmap_info_header_size != 124)
    {
        printf("[ UE::IMAGE_TOOLS::ERROR ] uLoadBitmap(): Unsupported bitmap format.\n");
        return false;
    }

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
    CIEXYZTRIPLE bitmap_endpoints  = {};
    u32          bitmap_gammaRed   = 0;
    u32          bitmap_gammaGreen = 0;
    u32          bitmap_gammaBlue  = 0;

    /* // [ cfarvin::TODO ] */
    /* // BYTE ORDER UNIQUE TO BITMAPV5HEADER */
    u32 bitmap_intent      = 0;
    u32 bitmap_profileData = 0;
    u32 bitmap_profileSize = 0;
    u32 bitmap_reserved    = 0;

    if (bitmap_info_header_size == 12) // BITMAPCOREHEADER ONLY
    {
        bitmap_width = (s32) uRead16AsLE(img);
        bitmap_height = (s32) uRead16AsLE(img);
        bitmap_planes = uRead16AsLE(img);
        assert(bitmap_planes == 1);
        bitmap_bitCount = uRead16AsLE(img);
    }
    else // EVERYONE ELSE
    {
        bitmap_width = (s32) uRead32AsLE(img);
        printf("\t[ debug ] bitmap_width: %d\n", bitmap_width);
        bitmap_height = (s32) uRead32AsLE(img);
        printf("\t[ debug ] bitmap_height: %d\n", bitmap_height);
        bitmap_planes = uRead16AsLE(img);
        printf("\t[ debug ] bitmap_planes: %d\n", bitmap_planes);
        assert(bitmap_planes == 1);

        bitmap_bitCount = uRead16AsLE(img);
        printf("\t[ debug ] bitmap_bitCount: %d\n", bitmap_bitCount);
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
        printf("\t[ debug ] bitmap_compression: %d\n", bitmap_compression);
        bitmap_sizeImage = uRead32AsLE(img);
        printf("\t[ debug ] bitmap_sizeImage: %d\n", bitmap_sizeImage);
        bitmap_xPelsPerMeter = (s32) uRead32AsLE(img);
        printf("\t[ debug ] bitmap_xPelsPerMeter: %d\n", bitmap_xPelsPerMeter);
        bitmap_yPelsPerMeter = (s32) uRead32AsLE(img);
        printf("\t[ debug ] bitmap_yPelsPerMeter: %d\n", bitmap_yPelsPerMeter);
        bitmap_clrUsed = uRead32AsLE(img);
        printf("\t[ debug ] bitmap_clrUsed: %d\n", bitmap_clrUsed);
        bitmap_clrImportant = uRead32AsLE(img);
        printf("\t[ debug ] bitmap_clrImportant: %d\n", bitmap_clrImportant);

        // [ cfarvin::UNTESTED ]
        // BITMAPV4HEADER && BITMAPV5HEADER
        if (bitmap_info_header_size >= 108)
        {
            bitmap_redMask = uRead32AsLE(img);
            printf("\t[ debug ] bitmap_redMask: %d\n", bitmap_redMask);
            bitmap_greenMask = uRead32AsLE(img);
            printf("\t[ debug ] bitmap_greenMask: %d\n", bitmap_greenMask);
            bitmap_blueMask = uRead32AsLE(img);
            printf("\t[ debug ] bitmap_blueMask: %d\n", bitmap_blueMask);
            bitmap_alphaMask = uRead32AsLE(img);
            printf("\t[ debug ] bitmap_alphaMask: %d\n", bitmap_alphaMask);
            bitmap_CSType = uRead32AsLE(img);
            printf("\t[ debug ] bitmap_CSType: %d\n", bitmap_CSType);

            // [ cfarvin::VERIFY_CORRECTES ] [ cfarvin::UNTESTED ]
            bitmap_endpoints.ciexyzRed.ciexyzX   = (s32) uRead32AsLE(img);
            printf("\t[ debug ] bitmap_endpoints.ciexyzRed.ciexyzX: %d\n",
                   bitmap_endpoints.ciexyzRed.ciexyzX);
            bitmap_endpoints.ciexyzRed.ciexyzY   = (s32) uRead32AsLE(img);
            printf("\t[ debug ] bitmap_endpoints.ciexyzRed.ciexyzY: %d\n",
                   bitmap_endpoints.ciexyzRed.ciexyzY);
            bitmap_endpoints.ciexyzRed.ciexyzZ   = (s32) uRead32AsLE(img);
            printf("\t[ debug ] bitmap_endpoints.ciexyzRed.ciexyzZ: %d\n",
                   bitmap_endpoints.ciexyzRed.ciexyzZ);
            bitmap_endpoints.ciexyzGreen.ciexyzX = (s32) uRead32AsLE(img);
            printf("\t[ debug ] bitmap_endpoints.ciexyzGreen.ciexyZ: %d\n",
                   bitmap_endpoints.ciexyzGreen.ciexyzX);
            bitmap_endpoints.ciexyzGreen.ciexyzY = (s32) uRead32AsLE(img);
            printf("\t[ debug ] bitmap_endpoints.ciexyzGreen.ciexyzY: %d\n",
                   bitmap_endpoints.ciexyzGreen.ciexyzY);
            bitmap_endpoints.ciexyzGreen.ciexyzZ = (s32) uRead32AsLE(img);
            printf("\t[ debug ] bitmap_endpoints.ciexyzGreen.ciexyzZ: %d\n",
                   bitmap_endpoints.ciexyzGreen.ciexyzZ);
            bitmap_endpoints.ciexyzBlue.ciexyzX  = (s32) uRead32AsLE(img);
            printf("\t[ debug ] bitmap_endpoints.ciexyzBlue.ciexyzX: %d\n",
                   bitmap_endpoints.ciexyzBlue.ciexyzX);
            bitmap_endpoints.ciexyzBlue.ciexyzY  = (s32) uRead32AsLE(img);
            printf("\t[ debug ] bitmap_endpoints.ciexyzBlue.ciexyzY: %d\n",
                   bitmap_endpoints.ciexyzBlue.ciexyzY);
            bitmap_endpoints.ciexyzBlue.ciexyzZ  = (s32) uRead32AsLE(img);
            printf("\t[ debug ] bitmap_endpoints.ciexyzBlue.ciexyZ: %d\n",
                   bitmap_endpoints.ciexyzBlue.ciexyzZ);

            /* printf("\t[ debug ] bitmap_clrImportant: %d\n", bitmap_clrImportant);         */
            bitmap_gammaRed = uRead32AsLE(img);
            printf("\t[ debug ] bitmap_gammaRed: %d\n", bitmap_gammaRed);
            bitmap_gammaGreen = uRead32AsLE(img);
            printf("\t[ debug ] bitmap_gammaGreen: %d\n", bitmap_gammaGreen);
            bitmap_gammaBlue = uRead32AsLE(img);
            printf("\t[ debug ] bitmap_gammaBlue: %d\n", bitmap_gammaBlue);

            // [ cfarvin::UNTESTED ]
            if (bitmap_info_header_size >= 124) // BITMAPV5HEADER ONLY
            {
                bitmap_intent = uRead32AsLE(img);
                printf("\t[ debug ] bitmap_intent: %d\n", bitmap_intent);
                bitmap_profileData = uRead32AsLE(img);
                printf("\t[ debug ] bitmap_profileData: %d\n", bitmap_profileData);
                bitmap_profileSize = uRead32AsLE(img);
                printf("\t[ debug ] bitmap_profileSize: %d\n", bitmap_profileSize);
                bitmap_reserved = uRead32AsLE(img);
                printf("\t[ debug ] bitmap_reserved: %d\n", bitmap_reserved);
            }
        }
    }

    printf("\t[ DEBUG::SUCCES ]\n");
    return true;
}
