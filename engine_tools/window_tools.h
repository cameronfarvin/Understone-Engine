#ifndef __UE_WINDOW_TOOLS_H__
#define __UE_WINDOW_TOOLS_H__

#include "type_tools.h"
#include "macro_tools.h"
#include "debug_tools.h"
#include "win_platform.h"

__UE_internal__ __UE_inline__ void
uGetWindowSize(_mut_ u32* const restrict width,
               _mut_ u32* const restrict height)
{
    uAssertMsg_v(width,  "[ window tools ] Width ptr must be non null.\n");
    uAssertMsg_v(height, "[ window tools ] Height ptr must be non null.\n");


#if _WIN32
    uWin32GetWindowSize(width, height);
#endif // _WIN32
}

#endif // __UE_WINDOW_TOOLS_H__
