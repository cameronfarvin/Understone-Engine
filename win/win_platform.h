#ifndef __win_platform__
#define __win_platform__ 1

#define WIN32
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include <engine_tools/debug_tools.h>
#include <engine_tools/type_tools.h>
#include <engine_tools/event_tools.h>
#include <data_structures/data_structures.h>



uSystemEvent win32_sys_event;
POINT win32_mouse_coords;
typedef struct
{
    HINSTANCE   instance;
    int         command_show;
    HWND        window;
    HDC         device_context;
    const char* class_name;
} uWin32Info;


// Note: no function decorators as to conform with Win32 specification.
LRESULT CALLBACK
uEngineWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    {
        win32_sys_event = uEventClose;
        break;
    }

    case WM_DESTROY:
    {
        win32_sys_event = uEventClose;
        PostQuitMessage(0);
        break;
    }

    case WM_LBUTTONDOWN:
    {
        uSetInputPressed(uMouse_left);
        break;
    }

    case WM_RBUTTONDOWN:
    {
        uSetInputPressed(uMouse_right);
        break;
    }

    case WM_LBUTTONUP:
    {
        uSetInputReleased(uMouse_left);
        break;
    }

    case WM_RBUTTONUP:
    {
        uSetInputReleased(uMouse_right);
        break;
    }

    case WM_SIZE:
    {
        // [ cfarvin::TODO ] scaling/ortho
        viewport.width = (u16) LOWORD(lParam);
        viewport.height = (u16) HIWORD(lParam);
        win32_sys_event = uEventResize;
        break;
    }

    case WM_MOUSEMOVE:
    {
        GetCursorPos(&win32_mouse_coords);
        ScreenToClient(hwnd, &win32_mouse_coords);

        // [ cfarvin::NOTE ] uMousePos has origin @ lower left == (0, 0, 0)
        mouse_pos.x = (u16) win32_mouse_coords.x;
        mouse_pos.y = (u16) (viewport.height - win32_mouse_coords.y);
        break;
    }
    }

    // Default Window Procedure
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


// [ cfarvin::TODO ] fail gracefully
__UE_internal__ __UE_call__ uWin32Info*
uWin32CreateWindow(const char* const restrict window_class_name)
{
    uWin32Info* win32_info   = (uWin32Info*) calloc(1, sizeof(uWin32Info));
    win32_info->class_name    = window_class_name;
    win32_info->instance      = GetModuleHandle(NULL);
    win32_info->command_show  = 10;

    WNDCLASSEX window_class    = { 0 };
    window_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc   = uEngineWindowProc;
    window_class.hInstance     = win32_info->instance;
    window_class.lpszClassName = win32_info->class_name;
    window_class.cbSize        = sizeof(WNDCLASSEX);
    /*
      window_class.hIcon = NULL;
      window_class.hCursor = NULL;
      window_class.hbrBackground = NULL;
      window_class.lpszMenuName = NULL;
      window_class.hIconSm = NULL;
    */

    if (!RegisterClassEx(&window_class))
    {
        uFatal("Could not register window class\n");
    }

    win32_info->window = CreateWindowEx(0,
                                       window_class.lpszClassName,
                                       window_class_name,
                                       WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       CW_USEDEFAULT,
                                       0,
                                       0,
                                       win32_info->instance,
                                       0);

    if (win32_info->window == NULL)
    {
        uFatal("Windows returned null handle to client window.\n");
    }

    ShowWindow(win32_info->window, win32_info->command_show);

    return win32_info;
}


__UE_internal__ __UE_inline__ uSystemEvent
uWin32HandleEvents()
{
    win32_sys_event = uEventNone;
    MSG msg = { 0 };
    PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return win32_sys_event;
}


__UE_internal__ __UE_call__ void
uDestroyWin32(_mut_ uWin32Info* const restrict win32_info)
{
    uAssertMsg_v(win32_info, "Received null win32_info ptr.\n");


    if(win32_info)
    {
        free(win32_info);
    }
}

#endif // __win_platform__
