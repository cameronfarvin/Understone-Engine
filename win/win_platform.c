#include <win_platform.h>
#include <assert.h>

#include <engine_tools/type_tools.h>
#include <engine_tools/event_tools.h>
#include <engine_tools/ogl_tools.h>

extern void* uWin32LoadPFNGL(const char* fn_name, const HMODULE* gl_module);

uSystemEvent
uWin32HandleEvents()
{
    win32_sys_event = uEventNone;
    MSG msg = { 0 };
    // [ cfarvin::TODO ]
    /* while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) */
    /* { */
    /*     TranslateMessage(&msg); */
    /*     DispatchMessage(&msg); */

    /*     	if (win32_sys_event == uEventResize) */
    /*     	{ */
    /*     		OutputDebugStringA("\n\nVICTORY\n\n"); */
    /*     	} */
    /* } */
    PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return win32_sys_event;
}

//
// [ cfarvin::TODO ] somehow fail from this file if setup is poopy.
//
void
uWin32CreateWindow()
{
    win32.class_name = "UE Window Class";

    WNDCLASSEX window_class = { 0 };
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = uEngineWindowProc;
    window_class.hInstance = win32.instance;
    window_class.lpszClassName = win32.class_name;
    window_class.cbSize = sizeof(WNDCLASSEX);
    /* window_class.hIcon = NULL; */
    /* window_class.hCursor = NULL; */
    /* window_class.hbrBackground = NULL; */
    /* window_class.lpszMenuName = NULL; */
    /* window_class.hIconSm = NULL; */

    if (!RegisterClassEx(&window_class))
    {
        printf("[ UE::WIN::ERROR ] Could not register window class\n");
    }

    win32.window = CreateWindowEx(0,
                                  window_class.lpszClassName,
                                  "UE",
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
        printf("[ UE::WIN::ERROR ] Windows returned null handle to client window.\n");
    }

    ShowWindow(win32.window, win32.command_show);
    uWin32HandleEvents(); // [ cfarvin::NOTE ] OpenGL Context Created here on WM_CREATE msg
}

LRESULT CALLBACK
uEngineWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        /* LRESULT result = { 0 }; */
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
                printf("[ UE::WIN::ERROR ] Could not obtain a valid device context\n");
                assert(false);
            }

            int pixel_format = ChoosePixelFormat(win32.device_context, &pixel_format_desc);
            if (!pixel_format)
            {
                printf("[ UE::WIN::ERROR ] Could not obtain a valid pixel format\n");
                assert(false);
            }
            SetPixelFormat(win32.device_context, pixel_format, &pixel_format_desc);

            win32.gl_context = wglCreateContext(win32.device_context);
            if (!win32.gl_context)
            {
                printf("[ UE::WIN::ERROR ] Could not obtain a GL rendering context\n");
                assert(false);
            }

            if (!wglMakeCurrent(win32.device_context, win32.gl_context))
            {
                printf("[ UE::WIN::ERROR ] Could not make the GL rendering context current\n");
                assert(false);
            }

            // Load all OpenGL functions
            const HMODULE gl_module = LoadLibraryA("opengl32.dll");
            if (!gl_module)
            {
                printf("[ UE::WIN::ERROR ] Could not load opengl32.dll\n");
                assert(false);
            }

            glUseProgram = (PFNGLUSEPROGRAMPROC) uWin32LoadPFNGL("glUseProgram", &gl_module);
            assert(glUseProgram);
            glGetShaderiv = (PFNGLGETSHADERIVPROC) uWin32LoadPFNGL("glGetShaderiv", &gl_module);
            assert(glGetShaderiv);
            glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) uWin32LoadPFNGL("glGetShaderInfoLog", &gl_module);
            assert(glGetShaderInfoLog);
            glGetProgramiv = (PFNGLGETPROGRAMIVPROC)uWin32LoadPFNGL("glGetProgramiv", &gl_module);
            assert(glGetProgramiv);
            glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) uWin32LoadPFNGL("glGetProgramInfoLog", &gl_module);
            assert(glGetProgramInfoLog);
            glCreateShader = (PFNGLCREATESHADERPROC) uWin32LoadPFNGL("glCreateShader", &gl_module);
            assert(glCreateShader);
            glCreateProgram = (PFNGLCREATEPROGRAMPROC) uWin32LoadPFNGL("glCreateProgram", &gl_module);
            assert(glCreateProgram);
            glShaderSource = (PFNGLSHADERSOURCEPROC) uWin32LoadPFNGL("glShaderSource", &gl_module);
            assert(glShaderSource);
            glCompileShader = (PFNGLCOMPILESHADERPROC) uWin32LoadPFNGL("glCompileShader", &gl_module);
            assert(glCompileShader);
            glAttachShader = (PFNGLATTACHSHADERPROC) uWin32LoadPFNGL("glAttachShader", &gl_module);
            assert(glAttachShader);
            glLinkProgram = (PFNGLLINKPROGRAMPROC) uWin32LoadPFNGL("glLinkProgram", &gl_module);
            assert(glLinkProgram);
            glDeleteShader = (PFNGLDELETESHADERPROC) uWin32LoadPFNGL("glDeleteShader", &gl_module);
            assert(glDeleteShader);
            glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) uWin32LoadPFNGL("glGetAttribLocation", &gl_module);
            assert(glGetAttribLocation);
            glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) uWin32LoadPFNGL("glGetUniformLocation", &gl_module);
            assert(glGetUniformLocation);
            glUniform1f = (PFNGLUNIFORM1FPROC) uWin32LoadPFNGL("glUniform1f", &gl_module);
            assert(glUniform1f);
            glUniform2f = (PFNGLUNIFORM2FPROC)uWin32LoadPFNGL("glUniform2f", &gl_module);
            assert(glUniform2f);
            glUniform3f = (PFNGLUNIFORM3FPROC) uWin32LoadPFNGL("glUniform3f", &gl_module);
            assert(glUniform3f);
            glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) uWin32LoadPFNGL("glGenVertexArrays", &gl_module);
            assert(glGenVertexArrays);
            glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) uWin32LoadPFNGL("glBindVertexArray", &gl_module);
            assert(glBindVertexArray);
            glGenBuffers = (PFNGLGENBUFFERSPROC) uWin32LoadPFNGL("glGenBuffers", &gl_module);
            assert(glGenBuffers);
            glBindBuffer = (PFNGLBINDBUFFERPROC) uWin32LoadPFNGL("glBindBuffer", &gl_module);
            assert(glBindBuffer);
            glBufferData = (PFNGLBUFFERDATAPROC) uWin32LoadPFNGL("glBufferData", &gl_module);
            assert(glBufferData);
            glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) uWin32LoadPFNGL("glVertexAttribPointer", &gl_module);
            assert(glVertexAttribPointer);
            glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) uWin32LoadPFNGL("glEnableVertexAttribArray", &gl_module);
            assert(glEnableVertexAttribArray);
            glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) uWin32LoadPFNGL("glGenFramebuffers", &gl_module);
            assert(glGenFramebuffers);
            glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) uWin32LoadPFNGL("glBindFramebuffer", &gl_module);
            assert(glBindFramebuffer);
            glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) uWin32LoadPFNGL("glFramebufferTexture2D", &gl_module);
            assert(glFramebufferTexture2D);
            glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) uWin32LoadPFNGL("glCheckFramebufferStatus", &gl_module);
            assert(glCheckFramebufferStatus);
            glActiveTexture = (PFNGLACTIVETEXTUREPROC) uWin32LoadPFNGL("glActiveTexture", &gl_module);
            assert(glActiveTexture);
            glIsShader = (PFNGLISSHADERPROC) uWin32LoadPFNGL("glIsShader", &gl_module);
            assert(glIsShader);
            glIsProgram = (PFNGLISPROGRAMPROC) uWin32LoadPFNGL("glIsProgram", &gl_module);
            assert(glIsProgram);
            glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) uWin32LoadPFNGL("glUniformMatrix4fv", &gl_module);
            assert(glUniformMatrix4fv);

            /* glClearColor(1.0f, 0.0f, 0.0f, 1.0f); */
            /* glClear(GL_COLOR_BUFFER_BIT); */
            /* SwapBuffers(UEhdc); */
            /* ReleaseDC(hwnd, UEhdc); */

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
            // [ cfarvin::TODO ] scaling/glortho
            viewport.width = (u16) LOWORD(lParam);
            viewport.height = (u16) HIWORD(lParam);
            win32_sys_event = uEventResize;
            break;
	}

	/* case WM_PAINT: */
	/* { */
        /*     break; */
	/* } */

        /* case WM_KEYDOWN */
        /* { */
        /*     switch(wParam) */
        /*     { */
        /*         case VK_LEFT: */
        /*         { */
        /*             win32_sys_event =  */
        /*         } */
        /*     } */
        /* } */

        case WM_MOUSEMOVE:
        {
            GetCursorPos(&win32_mouse_coords);
            ScreenToClient(win32.window, &win32_mouse_coords);
            // [ cfarvin::NOTE ] uMousePos has origin @ lower left == (0, 0, 0)
            mouse_pos.x = (u16) win32_mouse_coords.x;
            mouse_pos.y = (u16) (viewport.height - win32_mouse_coords.y);
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
