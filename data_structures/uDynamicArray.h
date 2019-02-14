#ifndef __uDynamicArray__
#define __uDynamicArray__ 1


#include <engine_tools/type_tools.h>

// Optimize for random access, "push back", "pop last"
typedef struct
{
    const size_t num_elements;
    const size_t max_elements;
    const size_t scaling_factor;
    const size_t num_first_level_indices;
    const size_t datatype_size;
    void* data;
} uDynamicArray;

void uDAPush(uDynamicArray* const da, void* const data);

void uDAPop(uDynamicArray* const da);

void* uDAIndex(uDynamicArray* const da, const size_t index);

#define uDAInit(type) uAPI_uDAInit(sizeof(type))
uDynamicArray* uAPI_uDAInit(size_t datatype_size_in);

void uDADestroy(uDynamicArray* const da);


#endif // __uDynamicArray
