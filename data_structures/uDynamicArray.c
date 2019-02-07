#include <uDynamicArray.h>

inline void uDynamicArrayPush(void* data)
{

}

inline void uDynamicArrayPop()
{

}

inline void uDynamicArrayAt(size_t index)
{

}

inline void uInitializeDynamicArray(uDynamicArray da)
{
    da.push = &uDynamicArrayPush;
    da.pop = &uDynamicArrayPop;
    da.at = &uDynamicArrayAt;
    da.num_elements = 0;
    da.scaling_factor = 2;
}
