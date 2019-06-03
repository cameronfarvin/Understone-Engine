#ifndef __uString__
#define __uString__ 1

#include <string.h>
#include <engine_tools/type_tools.h>

typedef struct
{
    char* volatile data;
    const size_t length;
} uString;

static inline uString*
uStringInit(const char* str)
{
    if (str)
    {
        uString* uStr = (uString*) calloc(1, sizeof(uString));
        size_t* non_const_length = (size_t*) &(uStr->length);
        *non_const_length = strlen(str);

        uStr->data = (char*) malloc(sizeof(char) * *non_const_length);
        memcpy(uStr->data, str, *non_const_length);

        return uStr;
    }

    return NULL;
}

static inline bool
uStringDestroy(uString* const uStr)
{
    if(uStr && uStr->data)
    {
        free(uStr->data);
        free(uStr);
        return true;
    }

    return false;
}

// [ cfarvin::TODO ]
/* static inline uDynamicArray* */
/* uStringSubstringIndices(uStringArray* const uSA, const char* sub_string) */
/* {} */


#endif // __uString
