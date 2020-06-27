#ifndef __win_platform__
#define __win_platform__ 1

#ifndef WIN32
#define WIN32
#endif // WIN32
#include <windows.h>
#include <stdio.h>
#include <stdint.h>

#include <data_structures/data_structures.h>
#include <engine_tools/macro_tools.h>
#include <engine_tools/debug_tools.h>
#include <engine_tools/event_tools.h>
#include <engine_tools/engine_info.h>
#include <engine_tools/type_tools.h>


__UE_global__ uSystemEvent win32_sys_event;
__UE_global__ POINT win32_mouse_coords;


//
// [ begin ] Prime uWin32Info
typedef struct
{
    HINSTANCE   instance;
    int         command_show;
    HWND        window;
    HDC         device_context;
    const char* class_name;
} uWin32Info;

// Forward declare creation method
__UE_internal__ const uWin32Info* const __UE_call__
uWin32CreateWin32Info();

__UE_singleton__ uWin32Info* uAPI_PRIME_WIN32_INFO = NULL;
__UE_internal__ __UE_inline__ const uWin32Info* const
uGetWin32Info()
{
    if (!uAPI_PRIME_WIN32_INFO)
    {
        *(uWin32Info**)&uAPI_PRIME_WIN32_INFO =
            (uWin32Info*)uWin32CreateWin32Info();
    }

    return uAPI_PRIME_WIN32_INFO;
}
// [ end ] Prime uWin32Info
//


__UE_internal__ __UE_inline__ void
uWin32GetWindowSize(_mut_ u32* const restrict width,
                    _mut_ u32* const restrict height)
{
    const uWin32Info* win32_info = uGetWin32Info();

    uAssertMsg_v(width,  "[ win32 ] Width ptr must be non null.\n");
    uAssertMsg_v(height, "[ win32 ] Height ptr must be non null.\n");
    uAssertMsg_v(IsWindow(win32_info->window),
                 "[ win32 ] Windows reports that the win32_info->window member is invalid.\n");

    RECT window_rect = { 0 };
    BOOL win32_rect_success = GetWindowRect(win32_info->window, &window_rect);
    if (!win32_rect_success)
    {
        uFatal("[ win32 ] [ vulkan ] Unable to determine window rect with win32 error: %d.\n", GetLastError());
    }

    uAssert(window_rect.right > window_rect.left);
    uAssert(window_rect.bottom > window_rect.top);

    *width  = window_rect.right - window_rect.left;
    *height = window_rect.bottom - window_rect.top;

    uAssert(*width);
    uAssert(*height);
}


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
        viewport.width  = (u16) LOWORD(lParam);
        viewport.height = (u16) HIWORD(lParam);
        win32_sys_event = uEventResize;
        break;
    }

    case WM_MOUSEMOVE:
    {
        GetCursorPos(&win32_mouse_coords);
        ScreenToClient(hwnd, &win32_mouse_coords);

        // Note: uMousePos has origin @ lower left == (0, 0, 0)
        mouse_pos.x = (u16) win32_mouse_coords.x;
        mouse_pos.y = (u16) (viewport.height - win32_mouse_coords.y);
        break;
    }
    }

    // Default Window Procedure
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


__UE_internal__ const uWin32Info* const __UE_call__
uWin32CreateWin32Info()
{
    if (uAPI_PRIME_WIN32_INFO)
    {
        return uAPI_PRIME_WIN32_INFO;
    }

    const char* window_class_name = uGetEngineName();

    *(uWin32Info**)&uAPI_PRIME_WIN32_INFO = (uWin32Info*)calloc(1, sizeof(uWin32Info));
    (*(uWin32Info**)&uAPI_PRIME_WIN32_INFO)->class_name    = window_class_name;
    (*(uWin32Info**)&uAPI_PRIME_WIN32_INFO)->instance      = GetModuleHandle(NULL);
    (*(uWin32Info**)&uAPI_PRIME_WIN32_INFO)->command_show  = 10;

    WNDCLASSEX window_class    = { 0 };
    window_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc   = uEngineWindowProc;
    window_class.hInstance     = uAPI_PRIME_WIN32_INFO->instance;
    window_class.lpszClassName = uAPI_PRIME_WIN32_INFO->class_name;
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
        free(*(uWin32Info**)&uAPI_PRIME_WIN32_INFO);
        uFatal("[ win32 ] Could not register window class; last error code: %d\n", GetLastError());
    }

    (*(uWin32Info**)&uAPI_PRIME_WIN32_INFO)->window = CreateWindowEx(0,
                                                                     window_class.lpszClassName,
                                                                     window_class_name,
                                                                     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                                                     CW_USEDEFAULT,
                                                                     CW_USEDEFAULT,
                                                                     CW_USEDEFAULT,
                                                                     CW_USEDEFAULT,
                                                                     0,
                                                                     0,
                                                                     uAPI_PRIME_WIN32_INFO->instance,
                                                                     0);

    if (uAPI_PRIME_WIN32_INFO->window == NULL)
    {
        free(*(uWin32Info**)&uAPI_PRIME_WIN32_INFO);
        uFatal("Windows returned null handle to client window.\n");
    }

    if (!IsWindow(uAPI_PRIME_WIN32_INFO->window))
    {
        free(*(uWin32Info**)&uAPI_PRIME_WIN32_INFO);
        uFatal("Windows reports that uAPI_PRIME_WIN32_INFO->window is invalid.\n");
    }

    ShowWindow(uAPI_PRIME_WIN32_INFO->window, uAPI_PRIME_WIN32_INFO->command_show);
    return uAPI_PRIME_WIN32_INFO;
}


__UE_internal__ __UE_inline__ const uWin32Info* const
uWin32CreateWindow()
{
    return uWin32CreateWin32Info();
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


__UE_internal__ void __UE_call__
uDestroyWin32(_mut_ uWin32Info* const restrict win32_info)
{
    uAssertMsg_v(win32_info, "Received null win32_info ptr.\n");


    if(win32_info)
    {
        free(win32_info);
    }
}

#endif // __win_platform__
