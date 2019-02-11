#include <uDynamicArray.h>

#include <stdlib.h>
#include <string.h>

// [ cfarvin::DEBUG ] [ cfarvin::REMOVE ]
#include <stdio.h>
#include <assert.h>

static inline void uDynamicArrayPush(void* data)
{
    printf("[ uDynamicArray ] push\n");
}

static inline void uDynamicArrayPop()
{
    printf("[ uDynamicArray ] pop\n");
}

static inline void uDynamicArrayAt(size_t index)
{
    printf("[ uDynamicArray ] index\n");
}

extern uDynamicArray* uInitializeDynamicArray(size_t datatype_size_in,
                                              size_t estimated_maximum_elements_in)
{
    printf("[ uDynamicArray ] init\n");
    uDynamicArray* da = (uDynamicArray*) calloc(1, sizeof(uDynamicArray));

    //
    // Initialize Statics
    //

    void (**non_const_push_fn)(void*) = (void (**)(void*)) &(da->push);
    void (**non_const_pop_fn)() = (void (**)()) &(da->pop);
    void (**non_const_at_fn)(size_t) = (void (**)(size_t)) &(da->at);
    size_t* non_const_num_elements = (size_t*) &(da->num_elements);
    size_t* non_const_scaling_factor = (size_t*) &(da->scaling_factor);
    size_t* non_const_scale_count = (size_t*) &(da->scale_count);
    size_t* non_const_num_first_level_indices = (size_t*) &(da->num_first_level_indices);
    size_t* non_const_datatype_size = (size_t*) &(da->datatype_size);
    size_t* non_const_estimated_maximum_elements = (size_t*) &(da->estimated_maximum_elements);

    *non_const_push_fn = uDynamicArrayPush;
    *non_const_pop_fn = uDynamicArrayPop;
    *non_const_at_fn = uDynamicArrayAt;
    *non_const_num_elements = 0;
    *non_const_scaling_factor = 2;
    *non_const_scale_count = 1;
    *non_const_num_first_level_indices = 2;
    *non_const_datatype_size = datatype_size_in;
    *non_const_estimated_maximum_elements = estimated_maximum_elements_in;

    // [ cfarvin::DEBUG ] [ cfarvin::REMOVE ]
    assert(da->push == uDynamicArrayPush);
    assert(da->pop == uDynamicArrayPop);
    assert(da->at == uDynamicArrayAt);
    assert(da->num_elements == 0);
    assert(da->scaling_factor == 2);
    assert(da->scale_count == 1);
    assert(da->num_first_level_indices == 2);
    assert(da->datatype_size == datatype_size_in);
    assert(da->estimated_maximum_elements == estimated_maximum_elements_in);

    //
    // Initialize Dynamics
    //

    void** non_const_const_data = (void**) &(da->data);
    *non_const_const_data = (void**) malloc(sizeof(void**) * da->num_first_level_indices);
    for (size_t ii = 0; ii < da->num_first_level_indices; ii++)
    {
        (non_const_const_data)[ii] = (void*) malloc(da->scale_count * da->scaling_factor);
        (non_const_const_data)[ii] = NULL;
    }

    return da;
}

extern void uDestroyDynamicArray(uDynamicArray* da)
{
    printf("[ uDynamicArray ] destroy\n");
}
