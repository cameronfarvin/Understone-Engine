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
    size_t scale_count;
    size_t num_first_level_indices;
    size_t datatype_size;

    void** data;
    /*
      On initialization: data = (void**) malloc(scaling_factor);
      1st allocation: scaling_factor * 1 @ data[0].
      2nd allocation: scaling_factor * 2 @ data[1].
      3rd allocation: scaling_factor * 3 @ data[2].

      On Nth allocation: if N >= scaling_factor
     */
} uDynamicArray;

extern uDynamicArray* uInitializeDynamicArray(size_t size_in);
extern void uDestroyDynamicArray(uDynamicArray* da);

#endif // __uDynamicArray
