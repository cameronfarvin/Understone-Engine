#ifndef __type_tools__
#define __type_tools__ 1

#include <stdint.h>
#include <stddef.h>


//
// Data Types
//
#define s8    int8_t
#define s16   int16_t
#define s32   int32_t
#define s64   int64_t
#define u8    uint8_t
#define u16   uint16_t
#define u32   uint32_t
#define u64   uint64_t
#define r32   float
#define r64   double
#define uPI   3.141592653589793
/* #define s8    int_fast8_t */
/* #define s16   int_fast16_t */
/* #define s32   int_fast32_t */
/* #define s64   int_fast64_t */
/* #define u8    uint_fast8_t */
/* #define u16   uint_fast16_t */
/* #define u32   uint_fast32_t */
/* #define u64   uint_fast64_t */

// vppc = void pointer pointer const
#define VPPC_STR_LITERAL(vppc) (vppc)

typedef enum
    {
        false = 0,
        true = 1
    } bool;


//
// Endian Types
//
union {
    s16 dword;
    s8  word[2];
} uEndiannessUnion;

typedef enum
    {
        uLITTLE_ENDIAN = 0,
        uBIG_ENDIAN    = 1
    } uEndianness;

void uDetermineSystemEndianness();
uEndianness uSystemEndianness;

//
// Size Types
//
typedef struct
{
    size_t width;
    size_t height;
    size_t depth;
} uSize;

#endif // __type_tools
