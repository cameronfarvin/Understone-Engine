#ifndef __uDynamicArray__
#define __uDynamicArray__ 1

#include <engine_tools/type_tools.h>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


#define uDAInit(type) uAPI_uDAInit(sizeof(type))
#define uDAPush(da, data_in) uAPI_uDAPush(da, VPPC_STR_LITERAL(void** const) data_in)


// Optimize for random access, "push back", "pop last",
// capping out at 2^16 elements via IEEE size_t.
typedef struct
{
    void* volatile data;

    const size_t datatype_size;
    volatile const size_t length;
    volatile const size_t scaling_factor;
    volatile const size_t max_length;
} uDynamicArray;


// [ cfarvin::NOTE ] Note the following definition:
// #define uDAInit(type) uAPI_uDAInit(sizeof(type))
static inline uDynamicArray*
uAPI_uDAInit(const size_t datatypesize_in)
{
    uDynamicArray* da = (uDynamicArray*) calloc(1, sizeof(uDynamicArray));

    // Initialize Statics
    size_t* non_const_length = (size_t*) &(da->length);
    size_t* non_const_scaling_factor = (size_t*) &(da->scaling_factor);
    size_t* non_const_max_length = (size_t*) &(da->max_length);
    size_t* non_const_datatype_size = (size_t*) &(da->datatype_size);

    *non_const_length = 0;
    *non_const_scaling_factor = 2;
    *non_const_max_length = 2;
    *non_const_datatype_size = datatypesize_in;

    // Initialize Dynamics
    da->data = (void*) malloc(*non_const_datatype_size * da->max_length);

    return da;
}


static inline bool
uAPI_uDAPush(uDynamicArray* const da, void** const data_in)
{
    if (da && data_in)
    {
        // [ cfarvin::TODO ] Properly check for failure on realloc
        if (da->length >= da->max_length)
        {
            size_t* non_const_max_length = (size_t*) &(da->max_length);
            *non_const_max_length = da->max_length * da->scaling_factor;
            da->data = realloc(da->data, (da->datatype_size * da->max_length));
        }

        memcpy((char*)da->data + (da->length * da->datatype_size),
               data_in,
               da->datatype_size);

        size_t* non_const_length = (size_t*) &(da->length);
        *non_const_length = *non_const_length + 1;

        return true;
    }

    return false;
}


static inline void*
uDAIndex(uDynamicArray* const da, const size_t index)
{
    if (da && (index < da->length))
    {
        return (void*)((char*)da->data + (index * da->datatype_size));
    }

    return NULL;
}


static inline bool
uDAPop(uDynamicArray* const da)
{
    if (da && da->length && da->length <= da->max_length)
    {
        size_t* non_const_length = (size_t*) &(da->length);
        *non_const_length = (*non_const_length) - 1;

        return true;
    }

    return false;
}


static inline bool
uDAFitToSize(uDynamicArray* const da)
{
    if (da && (da->length > da->scaling_factor))
    {
        size_t* non_const_max_length = (size_t*) &(da->max_length);
        *non_const_max_length = da->length;
        da->data = realloc(da->data, (da->datatype_size * da->max_length));

        return true;
    }

    return false;
}


static inline bool
uDASetScalingFactor(uDynamicArray* const da, const size_t scaling_factor_in)
{
    if (da && scaling_factor_in > 1)
    {
        size_t* non_const_scaling_factor = (size_t*) &da->scaling_factor;
        *non_const_scaling_factor = scaling_factor_in;

        return true;
    }

    return false;
}


static inline bool
uDADestroy(uDynamicArray* const da)
{
    if (da && da->data)
    {
        free(da->data);
        free(da);

        return true;
    }

    return false;
}



#endif // __uDynamicArray
