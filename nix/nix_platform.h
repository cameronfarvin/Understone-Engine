#ifndef __UE_NIX_PLATFORM_H__
#define __UE_NIX_PLATFORM_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <engine_tools/type_tools.h>
#include <engine_tools/event_tools.h>

#include <data_structures/uDynamicArray.h>
#include <data_structures/uString.h>



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
