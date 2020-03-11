#ifndef __UI_TYPE_TOOLS__
#define __UI_TYPE_TOOLS__

#include <stdint.h>
#include <stddef.h>

#include <engine_tools/macro_tools.h>

// vppc = void pointer pointer const
#define VPPC_STR_LITERAL(vppc) (vppc)



//
// Fundamental Types
//
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;
typedef float    r32;
typedef double   r64;


// [ cfarvin::TODO ] Relocate
typedef struct
{
    u16 width;
    u16 height;
} VIEWPORT;
VIEWPORT viewport;


typedef enum
{
    false = 0,
    true = 1
} bool;



//
// Endian Types
//
union
{
    s16 dword;
    s8  word[2];
} uEndiannessUnion;


typedef enum
{
    uLITTLE_ENDIAN = 0,
    uBIG_ENDIAN    = 1
} uEndianness;
uEndianness uSystemEndianness;


__UE_internal__ __UE_call__ void
uDetermineSystemEndianness()
{
    uEndiannessUnion.dword = 1;
    if (uEndiannessUnion.word[0])
    {
        uSystemEndianness = uLITTLE_ENDIAN;
        return;
    }

    uSystemEndianness = uBIG_ENDIAN;
}



//
// Size Types
//
typedef struct
{
    size_t width;
    size_t height;
    size_t depth;
} uSize;



//
// Color Types
//
typedef union
{
    // Little Endian Storage
    // Access via "channel" macro
    struct
    {
        u8 A;
        u8 R;
        u8 G;
        u8 B;
    } MSB_channel;

    // Big Endian Storage
    // Access via "channel" macro
    struct
    {
        u8 B;
        u8 G;
        u8 R;
        u8 A;
    } LSB_channel;

    u32 value;
} Color32_RGB;


typedef struct
{
    r32 H; // [ 0, 360 ]
    r32 S; // [ 0, 001 ]
    r32 V; // [ 0, 001 ]
} Color32_HSV;


#endif // __UE_TYPE_TOOLS__
