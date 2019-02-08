#include <uDynamicArray.h>

#include <stdlib.h>
#include <string.h>

// [ cfarvin::DEBUG ] [ cfarvin::REMOVE ]
#include <stdio.h>

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

extern uDynamicArray* uInitializeDynamicArray(size_t size_in)
{
    printf("[ uDynamicArray ] init\n");
    uDynamicArray* da = (uDynamicArray*) malloc(sizeof(uDynamicArray));
    memset(da, 0, sizeof(uDynamicArray));

    da->push = uDynamicArrayPush;
    da->pop = uDynamicArrayPop;
    da->at = uDynamicArrayAt;
    da->num_elements = 0;
    da->scaling_factor = 2;
    da->scale_count = 1;
    da->num_first_level_indices = 2;
    da->datatype_size = size_in;

    da->data = (void**) malloc(sizeof(void**) * da->num_first_level_indices);
    for (size_t ii = 0; ii < da->num_first_level_indices; ii++)
    {
        da->data = (void*) malloc(da->scale_count * da->scaling_factor);
    }

    return da;
}

extern void uDestroyDynamicArray(uDynamicArray* da)
{
    printf("[ uDynamicArray ] destroy\n");
}
