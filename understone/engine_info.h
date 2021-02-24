#ifndef __UE_ENGINE_INFO_H__
#define __UE_ENGINE_INFO_H__

#include "debug_tools.h"
#include "macro_tools.h"
#include "type_tools.h"

#define UE_ENGINE_NAME "Understone Engine"

__UE_inline__ static const char*
uGetEngineName()
{
    return UE_ENGINE_NAME;
}

#endif // __UE_ENGINE_INFO_H__
