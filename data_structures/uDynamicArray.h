#ifndef __uDynamicArray__
#define __uDynamicArray__ 1

#include <engine_tools/type_tools.h>

// Optimize for random access, "push back", "pop last",
// capping out at 2^16 elements via IEEE size_t.
typedef struct
{
    void* data;

    void (*const push)(void*);
    void (*const at)(size_t);

    const size_t num_elements;
    const size_t scaling_factor;
    const size_t datatype_size;
    const size_t max_elements;
} uDynamicArray;

void uDAPush(uDynamicArray* const da, void* const data);

void* uDAIndex(uDynamicArray* const da, const size_t index);

void uDASetScalingFactor(uDynamicArray* const da, const size_t scaling_factor_in);

#define uDAInit(type) uAPI_DAInit(sizeof(type))
uDynamicArray* uAPI_DAInit(const size_t datatype_size_in);

void uDADestroy(uDynamicArray* const da);


#endif // __uDynamicArray