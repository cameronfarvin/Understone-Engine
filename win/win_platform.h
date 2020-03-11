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
    HINSTANCE instance;
    int       command_show;
    HWND      window;
    HDC       device_context;
    HGLRC     gl_context;
    LPCSTR   class_name;
} uWIN32_INFO;
uWIN32_INFO win32;


LRESULT CALLBACK
uEngineWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
	case WM_CLOSE:
	{
            //OutputDebugStringA("WM_CLOSE\n");
            win32_sys_event = uEventClose;
            break;
	}

	case WM_DESTROY:
	{
            //OutputDebugStringA("WM_DESTROY\n");
            win32_sys_event = uEventClose;
            PostQuitMessage(0);
            break;
	}

	case WM_CREATE:
	{
            //OutputDebugStringA("WM_CREATE\n");
            PIXELFORMATDESCRIPTOR pixel_format_desc = { 0 };
            pixel_format_desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
            pixel_format_desc.nVersion = 1;
            pixel_format_desc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pixel_format_desc.iPixelType = PFD_TYPE_RGBA;
            pixel_format_desc.cColorBits = 32;
            pixel_format_desc.cRedBits = 0;
            pixel_format_desc.cRedShift = 0;
            pixel_format_desc.cGreenBits = 0;
            pixel_format_desc.cGreenShift = 0;
            pixel_format_desc.cBlueBits = 0;
            pixel_format_desc.cBlueShift = 0;
            pixel_format_desc.cAlphaBits = 0;
            pixel_format_desc.cAlphaShift = 0;
            pixel_format_desc.cAccumBits = 0;
            pixel_format_desc.cAccumRedBits = 0;
            pixel_format_desc.cAccumGreenBits = 0;
            pixel_format_desc.cAccumBlueBits = 0;
            pixel_format_desc.cAccumAlphaBits = 0;
            pixel_format_desc.cDepthBits = 24;
            pixel_format_desc.cStencilBits = 8;
            pixel_format_desc.cAuxBuffers = 0;
            pixel_format_desc.iLayerType = PFD_MAIN_PLANE; // now ignored by WINAPI
            pixel_format_desc.bReserved = 0;
            pixel_format_desc.dwLayerMask = 0;
            pixel_format_desc.dwVisibleMask = 0;
            pixel_format_desc.dwDamageMask = 0;

            win32.device_context = GetDC(hwnd);
            if (!win32.device_context)
            {
                uError_v("Could not obtain a valid device context\n");
                assert(false);
            }

            int pixel_format = ChoosePixelFormat(win32.device_context, &pixel_format_desc);
            if (!pixel_format)
            {
                uError_v("Could not obtain a valid pixel format\n");
                assert(false);
            }
            SetPixelFormat(win32.device_context, pixel_format, &pixel_format_desc);
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
            ScreenToClient(win32.window, &win32_mouse_coords);
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
void
uWin32CreateWindow()
{
    win32.class_name = (const LPCSTR) "UE Window Class";

    WNDCLASSEX window_class = { 0 };
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = uEngineWindowProc;
    window_class.hInstance = win32.instance;
    window_class.lpszClassName = (const LPCSTR) win32.class_name;
    window_class.cbSize = sizeof(WNDCLASSEX);
    /* window_class.hIcon = NULL; */
    /* window_class.hCursor = NULL; */
    /* window_class.hbrBackground = NULL; */
    /* window_class.lpszMenuName = NULL; */
    /* window_class.hIconSm = NULL; */

    if (!RegisterClassEx(&window_class))
    {
        uError_v("Could not register window class\n");
    }

    win32.window = CreateWindowEx(0,
                                  window_class.lpszClassName,
                                  (const LPCSTR)"UE",
                                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  0,
                                  0,
                                  win32.instance,
                                  0);

    if (win32.window == NULL)
    {
        uError_v("Windows returned null handle to client window.\n");
    }

    ShowWindow(win32.window, win32.command_show);
    /* uWin32HandleEvents(); // [ cfarvin::NOTE ] OpenGL Context Created here on WM_CREATE msg */
}


uSystemEvent
uWin32HandleEvents()
{
    win32_sys_event = uEventNone;
    MSG msg = { 0 };
    PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return win32_sys_event;
}


// [ cfarvin::TODO ] Clean up win32 allocations on destroy
/* void win32_destroy(); */

#endif // __win_platform__
