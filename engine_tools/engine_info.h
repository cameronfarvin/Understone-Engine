#ifndef __UE_ENGINE_INFO_H__
#define __UE_ENGINE_INFO_H__

#include "type_tools.h"
#include "macro_tools.h"
#include "debug_tools.h"

#define UE_ENGINE_NAME "Understone Engine"
#define UE_GAME_TITLE  "Understone"

__UE_internal__ __UE_inline__ const char*
uGetEngineName()
{
    return UE_ENGINE_NAME;
}

__UE_internal__ __UE_inline__ const char*
uGetGameTitle()
{
    return UE_GAME_TITLE;
}


#endif // __UE_ENGINE_INFO_H__
