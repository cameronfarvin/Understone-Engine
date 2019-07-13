#ifndef __memory_tools__
#define __memory_tools__ 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <engine_tools/type_tools.h>
#include <engine_tools/debug_tools.h>

// Optimize for 64 byte cache line size,
// 32bit word alignment
typedef struct
{
    intptr_t*   data;
    u16         offset;
    const u16   arena_size;
} uMemoryArena;


static inline uMemoryArena*
uMAInit(u16 arena_bytes)
{
    if (!arena_bytes)
    {

        return NULL;
    }

    uMemoryArena* memory_arena = (uMemoryArena*)malloc(sizeof(uMemoryArena));
    memory_arena->data         = malloc(arena_bytes);
    memory_arena->offset       = 0;
    u16* non_const_arena_size  = (u16*) &(memory_arena->arena_size);
    *non_const_arena_size      = arena_bytes;

    return memory_arena;
}

static inline void*
uMANext(uMemoryArena* memory_arena)
{
    if(!(memory_arena && memory_arena->data && memory_arena->arena_size))
    {
        return NULL;
    }

    return (void*)(memory_arena->data + memory_arena->offset);
}


#define uMAAllocate(arena, type, num_bytes)     \
    (type*)uMAAllocate_API__(arena, num_bytes)
static inline void* uMAAllocate_API__(uMemoryArena* memory_arena, u16 num_bytes)
{
    if (!(num_bytes && memory_arena && memory_arena->data))
    {
        return NULL;
    }

    void* return_ptr = (memory_arena->data + memory_arena->offset);
    memory_arena->offset += num_bytes;
    return return_ptr;
}


//
// [ cfarvin::NOTE ] [ cfarvin::TODO ] Difficult to store lvalue of all types w/o using
// void*. May require user to create an lvalue for targets before pushes,
// otherwise there would not be need for dynamic memory allocation in the first
// place.
//
// Was thinking something along the lines of uMAPushRawData().
//
#define uMAPushData( arena, new_data, type )                            \
    (type*)uMAPushData_API( arena, (type*)&(new_data), sizeof(type) )
static inline void*
uMAPushData_API(uMemoryArena* memory_arena,
                void* new_data,
                u16 new_data_size)
{
    if (!(memory_arena &&
          new_data &&
          new_data_size))
    {
        return NULL;
    }

    if((memory_arena->offset + new_data_size) > (memory_arena->arena_size))
    {
        uError("[ DATA ] Attempted an illegal push to uMemoryArena (overshot arena size)\n");
        return (memory_arena->data + memory_arena->offset);
    }

    intptr_t* dest_ptr = memory_arena->data + memory_arena->offset;
    memcpy(dest_ptr,
           new_data,
           (size_t) new_data_size);
    memory_arena->offset += new_data_size;
    return (void*)dest_ptr;
}


#define uMAPushArray( arena, new_data, type, num_bytes )        \
    (type*)uMAPushArray_API( arena, new_data, num_bytes )
static inline void*
uMAPushArray_API(uMemoryArena* memory_arena,
                 void* new_data,
                 u16 new_data_size)
{
    if (!(memory_arena &&
          new_data &&
          new_data_size))
    {
        return NULL;
    }

    if((memory_arena->offset + new_data_size) > (memory_arena->arena_size))
    {
        uError("[ ARRAY ] Attempted an illegal push to uMemoryArena (overshot arena size)\n");
        return (memory_arena->data + memory_arena->offset);
    }


    intptr_t* dest_ptr = memory_arena->data + memory_arena->offset;
    memcpy(dest_ptr,
           new_data,
           (size_t) new_data_size);
    memory_arena->offset += new_data_size;
    return (void*)(dest_ptr);
}


static inline bool
uMADestroy(uMemoryArena* memory_arena)
{
    if (!(memory_arena && memory_arena->data))
    {
        // [ cfarvin::NOTE ] [ cfarvin::TODO ]Was printing during tests in which
        // this was *supposed* to fail.
        /* uError("Invlid memory arena free\n"); */
        return false;
    }

    free(memory_arena->data);
    free(memory_arena);
    return true;
}

#endif // __memory_tools
