#ifndef __memory_tools__
#define __memory_tools__ 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Arena Allocation
typedef struct
{
    void*        data;
    void*        data_ptr;
    const size_t arena_size;
} uMemoryArena;


static inline uMemoryArena*
uInitMemoryArena(size_t arena_bytes)
{
    if (arena_bytes)
    {
        uMemoryArena* memory_arena = (uMemoryArena*)malloc(sizeof(uMemoryArena));
        memory_arena->data     = malloc(arena_bytes);
        memory_arena->data_ptr = memory_arena->data;
        size_t* non_const_arena_size = (size_t*) &(memory_arena->arena_size);
        *non_const_arena_size = arena_bytes;

        return memory_arena;
    }

    return NULL;
}

#define uPushMemoryArena( arena, new_data )                     \
    uPushMemoryArena_API( arena, new_data, sizeof(new_data) )
static inline void
uPushMemoryArena_API(uMemoryArena* memory_arena,
                     void* new_data,
                     size_t new_data_size)
{
    if (memory_arena &&
        memory_arena->data &&
        new_data &&
        new_data_size)
    {
        if ( ((size_t*)memory_arena->data_ptr + new_data_size) >
             ((size_t*)memory_arena->data + memory_arena->arena_size) )
        {
            puts("[ ERROR ][ DEBUG ] Attempted arena overallocation!\n");
            assert(0);
        }

        memcpy(memory_arena->data_ptr, new_data, new_data_size);
    }
    else
    {
        puts("[ ERROR ][ DEBUG ] Attempted illegal arena push!\n");
        assert(0);
    }
}

static inline void
uDestroyMemoryArena(uMemoryArena* memory_arena)
{
    if (memory_arena)
    {
        if (memory_arena->data)
        {
            free(memory_arena->data);
        }

        free(memory_arena);
    }
}

#endif // __memory_tools
