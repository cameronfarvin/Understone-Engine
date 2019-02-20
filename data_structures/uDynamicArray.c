#include <uDynamicArray.h>

#include <stdlib.h>
#include <string.h>

// [ cfarvin::DEBUG ] [ cfarvin::REMOVE ]
#include <stdio.h>
#include <assert.h>

#ifndef uPUSH_DATA
#define uPUSH_DATA memcpy((non_const_data + (da->num_elements * da->datatype_size)), \
                          data, da->datatype_size)
#endif // uPUSH_DATA


void
uDAPush(uDynamicArray* const da, void* const data)
{
    printf("[ uDynamicArray ] push\n");
    // size_t* non_const_num_elements = (size_t*) &(da->num_elements);
    /* *(((char*)da->data) + (0 * da->datatype_size)) = 11; */
    /* *(((char*)da->data) + (1 * da->datatype_size)) = 21; */
    /* *(((char*)da->data) + (2 * da->datatype_size)) = 33; */

    /* printf("[ DEBUG ] da->data[0]: %d\n", *(((char*)da->data) + (0 * da->datatype_size))); */
    /* printf("[ DEBUG ] da->data[1]: %d\n", *(((char*)da->data) + (1 * da->datatype_size))); */
    /* printf("[ DEBUG ] da->data[2]: %d\n", *(((char*)da->data) + (2 * da->datatype_size))); */

    if (da->num_elements >= da->max_elements)
    {
        printf("RESIZING\n");
        realloc(da->data, (da->max_elements * 2));
        size_t* non_const_max_elements = (size_t*) &(da->data);
        *non_const_max_elements = da->max_elements * 2;
    }

    printf("PUSH\n");
    size_t* non_const_num_elements = (size_t*) &(da->num_elements);
    char* index_ptr = ( ((char*)da->data) + (++(*non_const_num_elements) * da->datatype_size) );
    memcpy(index_ptr, data, da->datatype_size);
}

/* void */
/* uDAPop(uDynamicArray* const da) */
/* { */
/*     printf("[ uDynamicArray ] pop\n"); */
/* } */

void*
uDAIndex(uDynamicArray* const da, const size_t index)
{
    printf("[ uDynamicArray ] index\n");
    assert(index <= da->num_elements);
    return (da + index);
}

// [ cfarvin::NOTE ] The following is defined in the header:
// #define uDAInit(type) uAPI_uDAInit(sizeof(type))
uDynamicArray*
uAPI_DAInit(const size_t datatype_size_in)
{
    /*
      [ cfarvin::NOTE ] [ cfarvin::TODO ]
      Test use of differentiable divergent function to map into sub-array
    */
    printf("[ uDynamicArray ] init\n");
    uDynamicArray* da = (uDynamicArray*) calloc(1, sizeof(uDynamicArray));

    //
    // Initialize Statics
    //
    size_t* non_const_num_elements = (size_t*) &(da->num_elements);
    size_t* non_const_scaling_factor = (size_t*) &(da->scaling_factor);
    size_t* non_const_max_elements = (size_t*) &(da->max_elements);
    size_t* non_const_datatype_size = (size_t*) &(da->datatype_size);

    *non_const_num_elements = 0;
    *non_const_scaling_factor = 2;
    *non_const_max_elements = 2;
    *non_const_datatype_size = datatype_size_in;

    // [ cfarvin::DEBUG ] [ cfarvin::REMOVE ]
    assert(da->num_elements == 0);
    assert(da->scaling_factor == 2);
    assert(da->max_elements == 2);
    assert(da->datatype_size == datatype_size_in);

    //
    // Initialize Dynamics
    //
    da->data = (void*) malloc(sizeof(datatype_size_in) * da->max_elements);

    return da;
}

/* void */
/* uDADestroy(uDynamicArray* const da) */
/* { */
/*     printf("[ uDynamicArray ] destroy\n"); */
/* } */
