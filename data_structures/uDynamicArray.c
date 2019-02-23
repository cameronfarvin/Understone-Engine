#include <uDynamicArray.h>

void
uDAPush(uDynamicArray* const da, void** const data_in)
{
    if (!da)
    {
        return;
    }

    if (da->num_elements >= da->max_elements)
    {
        size_t* non_const_max_elements = (size_t*) &(da->max_elements);
        *non_const_max_elements = da->max_elements * da->scaling_factor;
        da->data = realloc(da->data, (da->datatype_size * da->max_elements));
    }

    memcpy((char*)da->data + (da->num_elements * da->datatype_size),
           data_in,
           da->datatype_size);

    size_t* non_const_num_elements = (size_t*) &(da->num_elements);
    *non_const_num_elements = *non_const_num_elements + 1;
}

// [ cfarvin::NOTE ] C99+ inlining.
// Definition provided in uDynmaicArray.h
extern void
uDAPop(uDynamicArray* const da);

// [ cfarvin::NOTE ] C99+ inlining.
// Definition provided in uDynmaicArray.h
extern void*
uDAIndex(uDynamicArray* const da, const size_t index);

// [ cfarvin::NOTE ] C99+ inlining.
// Definition provided in uDynmaicArray.h
extern void
uDAFitToSize(uDynamicArray* const da);

// [ cfarvin::NOTE ] The following is defined in the header:
// #define uDAInit(type) uAPI_uDAInit(sizeof(type))
uDynamicArray*
uAPI_DAInit(const size_t datatype_size_in)
{
    uDynamicArray* da = (uDynamicArray*) calloc(1, sizeof(uDynamicArray));

    // Initialize Statics
    size_t* non_const_num_elements = (size_t*) &(da->num_elements);
    size_t* non_const_scaling_factor = (size_t*) &(da->scaling_factor);
    size_t* non_const_max_elements = (size_t*) &(da->max_elements);
    size_t* non_const_datatype_size = (size_t*) &(da->datatype_size);

    *non_const_num_elements = 0;
    *non_const_scaling_factor = 2;
    *non_const_max_elements = 2;
    *non_const_datatype_size = datatype_size_in;

    assert(da->num_elements == 0);
    assert(da->scaling_factor == 2);
    assert(da->max_elements == 2);
    assert(da->datatype_size == datatype_size_in);

    // Initialize Dynamics
    (da->data) = (void*) malloc(sizeof(datatype_size_in) * da->max_elements);

    assert(da);
    return da;
}

void
uDADestroy(uDynamicArray* const da)
{
    if (da && da->data)
    {
        free(da->data);
        free(da);
    }
}

void uDASetScalingFactor(uDynamicArray* const da, const size_t scaling_factor_in)
{
    assert(da);

    size_t* non_const_scaling_factor = (size_t*) &da->scaling_factor;
    * non_const_scaling_factor = scaling_factor_in;
}
