#ifndef __type_tools__
#define __type_tools__ 1

#include <stdint.h>
#include <stddef.h>

#define s8    int_fast8_t
#define s16   int_fast16_t
#define s32   int_fast32_t
#define s64   int_fast64_t
#define u8    uint_fast8_t
#define u16   uint_fast16_t
#define u32   uint_fast32_t
#define u64   uint_fast64_t
#define r32   float
#define r64   double
#define uPI   3.141592653589793

typedef enum
    {
        false = 0,
        true = 1
    } bool;

typedef struct
{
    size_t width;
    size_t height;
    size_t depth;
} UE_SIZE;

#endif // __type_tools
