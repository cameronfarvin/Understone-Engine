#ifndef __type_tools__
#define __type_tools__ 1

#include <stdint.h>
#include <stddef.h>

#define int8    int8_t
#define int16   int16_t
#define int32   int32_t
#define int64   int64_t
#define uint8   uint8_t
#define uint16  uint16_t
#define uint32  uint32_t
#define uint64  uint64_t
#define float32 float
#define float64 double

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
