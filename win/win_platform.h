#ifndef __win_platform__
#define __win_platform__ 1

// MSVC will define _WIN32 but nwo WIN32. WIN32 Needed
// for includsion of some headers.
// #define WIN32

#include <windows.h>
/* #include <wingdi.h> */

#include <GL\gl.h>
#include <GL\glext.h>
/* #include <GL\glu.h> */
/* #include <GL\wglext.h> */
#include <GL\glcorearb.h>

#include <stdio.h>
#include <stdint.h>

#include <engine_tools/event_tools.h>


typedef struct
{
    HINSTANCE instance;
    int       command_show;
    HWND      window;
    HDC       device_context;
    HGLRC     gl_context;
    LPCSTR    class_name;
} uWIN32_INFO;
uWIN32_INFO win32;

void uWin32CreateWindow();

inline void*
uWin32LoadPFNGL(const char* fn_name, const HMODULE* gl_module)
{
    void* pfngl = (void *)wglGetProcAddress(fn_name);

    if (pfngl == 0 ||
        (pfngl == (void*)0x1) ||
        (pfngl == (void*)0x2) ||
        (pfngl == (void*)0x3) ||
        (pfngl == (void*)-1))
    {
        pfngl = (void *)GetProcAddress(*gl_module, fn_name);
    }

    return pfngl;
}

uSystemEvent uWin32HandleEvents();

LRESULT CALLBACK uEngineWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// [ cfarvin::TODO ] Clean up win32 allocations on destroy
/* void win32_destroy(); */

uSystemEvent win32_sys_event;
POINT win32_mouse_coords;

#endif // __win_platform__
