#ifndef __uDynamicArray__
#define __uDynamicArray__ 1

#include <engine_tools/type_tools.h>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Optimize for random access, "push back", "pop last",
// capping out at 2^16 elements via IEEE size_t.
typedef struct
{
    void* data;

    void (*const push)(void*);
    void (*const at)(size_t);

    volatile const size_t num_elements;
    const size_t scaling_factor;
    const size_t datatype_size;
    const size_t max_elements;
} uDynamicArray;

void uDAPush(uDynamicArray* const da, void** const data);

static inline void* uDAIndex(uDynamicArray* const da, const size_t index)
{
    if (da && (index < da->num_elements))
    {
        return (void*)((char*)da->data + (index * da->datatype_size));
    }

    return NULL;
}

static inline void uDAPop(uDynamicArray* const da)
{
    if (da && da->num_elements)
    {
        size_t* non_const_num_elements = (size_t*) &(da->num_elements);
        *non_const_num_elements = (*non_const_num_elements) - 1;
    }
}

static inline void uDAFitToSize(uDynamicArray* const da)
{
    if (da && (da->num_elements > da->scaling_factor))
    {
        size_t* non_const_max_elements = (size_t*) &(da->max_elements);
        *non_const_max_elements = da->num_elements;
        da->data = realloc(da->data, (da->datatype_size * da->max_elements));
    }
}

void uDASetScalingFactor(uDynamicArray* const da, const size_t scaling_factor_in);

#define uDAInit(type) uAPI_DAInit(sizeof(type))
uDynamicArray* uAPI_DAInit(const size_t datatype_size_in);

void uDADestroy(uDynamicArray* const da);


#endif // __uDynamicArray
