#ifndef __UE_ENGINE_INFO_H__
#define __UE_ENGINE_INFO_H__

#include <engine_tools/type_tools.h>
#include <engine_tools/macro_tools.h>
#include <engine_tools/debug_tools.h>

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
