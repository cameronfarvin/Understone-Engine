#ifndef __uDynamicArray__
#define __uDynamicArray__ 1

#include <engine_tools/type_tools.h>

// Optimize for random access, "push back", "pop last"
typedef struct
{
    void (*push)(void* data);
    void (*pop)();
    void (*at)(size_t index);
    size_t num_elements;
    size_t scaling_factor;
} uDynamicArray;

void uInitializeDynamicArray();

#endif // __uDynamicArray
