#ifndef __win_platform__
#define __win_platform__ 1

#include "debug_tools.h"
#include "engine_info.h"
#include "event_tools.h"
#include "macro_tools.h"
#include "type_tools.h"
#include "window_tools.h"

#include <stdint.h>
#include <stdio.h>
#include <windows.h>

uSystemEvent kWin32SystemEvent      = {};
POINT        kWin32MouseCoordinates = {};

typedef struct
{
    HINSTANCE   instance;
    int         command_show;
    HWND        window;
    HDC         device_context;
    const char* class_name;
} uWin32Info;

uWin32Info
uCreateWin32Info(uGameWindow* const restrict game_window,
                 LRESULT                     CALLBACK (*WindowProc)(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam))
{
    const char* window_class_name = uGetEngineName();
    uWin32Info  win32_info        = {};

    win32_info.command_show = 10;

    WNDCLASSEX window_class  = {};
    window_class.style       = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = WindowProc;
    window_class.hInstance = win32_info.instance = GetModuleHandle(NULL);
    window_class.lpszClassName = win32_info.class_name = window_class_name;
    window_class.cbSize                                = sizeof(WNDCLASSEX);
    /*
      window_class.hIcon = NULL;
      window_class.hCursor = NULL;
      window_class.hbrBackground = NULL;
      window_class.lpszMenuName = NULL;
      window_class.hIconSm = NULL;
    */

    if (!RegisterClassEx(&window_class))
    {
        uFatal("[ win32 ] Could not register window class; last error code: %lu\n", GetLastError());
    }

    win32_info.window = CreateWindowEx(0,
                                       window_class.lpszClassName,
                                       ( const char* )window_class_name,
                                       WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       0,
                                       0,
                                       win32_info.instance,
                                       0);

    if (win32_info.window == NULL)
    {
        uFatal("Windows returned null handle to client window.\n");
    }

    if (!IsWindow(win32_info.window))
    {
        uFatal("Windows reports that uAPI_PRIME_WIN32_INFO->window is invalid.\n");
    }

    ShowWindow(win32_info.window, win32_info.command_show);
    game_window->is_minimized = false;
    return win32_info;
}

__UE_inline__ uSystemEvent
uWin32HandleEvents()
{
    kWin32SystemEvent = uEventNone;
    MSG msg           = {};
    PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return kWin32SystemEvent;
}
#endif // __win_platform__
