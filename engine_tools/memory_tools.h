#ifndef __memory_tools__
#define __memory_tools__ 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

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
    memory_arena->data     = malloc(arena_bytes);
    memory_arena->offset   = 0;
    u16* non_const_arena_size = (u16*) &(memory_arena->arena_size);
    *non_const_arena_size = arena_bytes;

    return memory_arena;
}

#define uMAPush( arena, new_data )                      \
    uMAPush_API( arena, new_data, sizeof(new_data) )
static inline void
uMAPush_API(uMemoryArena* memory_arena,
            void* new_data,
            u16 new_data_size)
{
    if (!(memory_arena &&
          new_data &&
          new_data_size &&
          ((memory_arena->offset + new_data_size) <= (memory_arena->arena_size))))
    {
        uError_v("Attempted illegal arena push!\n");
        assert(0);
    }

    memcpy((memory_arena->data + memory_arena->offset),
           new_data,
           (size_t) new_data_size);
}

static inline void
uDestroyMemoryArena(uMemoryArena* memory_arena)
{
    if (!(memory_arena && memory_arena->data))
    {
        return;
    }

    free(memory_arena->data);
    free(memory_arena);
}

#endif // __memory_tools
