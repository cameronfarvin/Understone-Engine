#ifndef __win_platform__
#define __win_platform__ 1

#include <windows.h>
#include <wingdi.h>
#include <stdio.h>
#include <stdint.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glext.h>

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

uEVENT win32_proxy_event;

void win32_createWindow();

/* void win32_destroy(); */

inline void* GetPFNGL(const char *name);

uEVENT win32_handleEvents();

LRESULT CALLBACK UninstallEngineWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // __win_platform__
