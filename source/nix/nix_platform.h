#ifndef __UE_NIX_PLATFORM_H__
#define __UE_NIX_PLATFORM_H__

#include <assert.h>
#include <data_structures/uDynamicArray.h>
#include <data_structures/uString.h>
#include <engine_tools/event_tools.h>
#include <engine_tools/type_tools.h>
#include <stdio.h>
#include <stdlib.h>

void
uNixCreateWindow()
{
    uDBGPrint("TODO: Create Window: (X11 -> Weyland)");
    return uEventNone;
}

void
uNixDestroyWindow()
{
    uDBGPrint("TODO: Create Window: (X11 -> Weyland)");
}

uSystemEvent
uNixHandleEvents();

#endif // __UE_NIX_PLATFORM_H__
