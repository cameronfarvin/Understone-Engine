#ifndef __uDynamicArray__
#define __uDynamicArray__ 1

#include <engine_tools/type_tools.h>

// Optimize for random access, "push back", "pop last"
typedef struct
{
    /* void (*push)(void* data); */
    /* void (*pop)(); */
    /* void (*at)(size_t index); */
    void (*const push)(void*);
    void (*const pop)();
    void (*const at)(size_t);

    const size_t num_elements;
    const size_t scaling_factor;
    const size_t scale_count;
    const size_t num_first_level_indices;
    const size_t datatype_size;
    const size_t estimated_maximum_elements;

    const void** const data;
    /*
      On initialization: data = (void**) malloc(scaling_factor);
      1st allocation: scaling_factor * 1 @ data[0].
      2nd allocation: scaling_factor * 2 @ data[1].
      3rd allocation: scaling_factor * 3 @ data[2].

      On Nth allocation: if N >= scaling_factor
     */
} uDynamicArray;

extern uDynamicArray* uInitializeDynamicArray(size_t size_in,
    size_t estimated_maximum_elements_in);
extern void uDestroyDynamicArray(uDynamicArray* da);

#endif // __uDynamicArray
