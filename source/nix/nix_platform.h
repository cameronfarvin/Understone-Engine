#ifndef __UE_NIX_PLATFORM_H__
#define __UE_NIX_PLATFORM_H__

#include <assert.h>
#include <uDynamicArray.h>
#include <uString.h>
#include <event_tools.h>
#include <type_tools.h>
#include <stdio.h>
#include <stdlib.h>

uSystemEvent
uNixCreateWindow()
{
    uDebugPrint("TODO: Create Window: (X11 -> Weyland)");
    return uEventNone;
}

uSystemEvent
uNixDestroyWindow()
{
    uDebugPrint("TODO: Create Window: (X11 -> Weyland)");
    return uEventNone;
}

uSystemEvent
uNixHandleEvents();

#endif // __UE_NIX_PLATFORM_H__
