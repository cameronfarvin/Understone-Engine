#ifndef __UE_WINDOW_TOOLS_H__
#define __UE_WINDOW_TOOLS_H__

#include "debug_tools.h"
#include "macro_tools.h"
#include "type_tools.h"

struct uGameWindow
{
    u16  width;
    u16  height;
    bool is_minimized;
};
uGameWindow kGameWindow;

static __UE_inline__ void
uGetWindowSize(u32* const restrict width, u32* const restrict height)
{
    uAssertMsg_v(width, "[ window tools ] Width ptr must be non null.\n");
    uAssertMsg_v(height, "[ window tools ] Height ptr must be non null.\n");

    *width  = kGameWindow.width;
    *height = kGameWindow.height;
}

#endif // __UE_WINDOW_TOOLS_H__
