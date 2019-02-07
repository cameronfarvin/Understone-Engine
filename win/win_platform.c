#include <win_platform.h>
#include <assert.h>

#include <engine_tools/type_tools.h>
#include <engine_tools/event_tools.h>
#include <engine_tools/ogl_tools.h>

uEVENT
uWin32HandleEvents()
{
    win32_proxy_event = uEVENT_NONE;

    MSG msg = { 0 };
    /* while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) */
    /* { */
    /*     TranslateMessage(&msg); */
    /*     DispatchMessage(&msg); */

    /*     	if (win32_proxy_event == uEVENT_RESIZE) */
    /*     	{ */
    /*     		OutputDebugStringA("\n\nVICTORY\n\n"); */
    /*     	} */
    /* } */
    PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return win32_proxy_event;
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

    //RegisterClassEx(&window_class); // [ cfarvin::NOTE ] Apparently we thought it was a good idea to register the class twice...

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
        LRESULT result = { 0 };
	case WM_CLOSE:
	{
            //OutputDebugStringA("WM_CLOSE\n");
            printf("WM_CLOSE\n");
            win32_proxy_event = uEVENT_CLOSE;
            break;
	}

	case WM_DESTROY:
	{
            //OutputDebugStringA("WM_DESTROY\n");
            printf("WM_DESTROY\n");
            win32_proxy_event = uEVENT_CLOSE;
            PostQuitMessage(0);
            break;
	}

	case WM_CREATE:
	{
            //OutputDebugStringA("WM_CREATE\n");
            printf("WM_CREATE\n");
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

            glUseProgram = uWin32LoadPFNGL("glUseProgram", &gl_module);
            glGetShaderiv = uWin32LoadPFNGL("glGetShaderiv", &gl_module);
            glGetShaderInfoLog = uWin32LoadPFNGL("glGetShaderInfoLog", &gl_module);
            glGetProgramiv = uWin32LoadPFNGL("glGetProgramiv", &gl_module);
            glGetProgramInfoLog = uWin32LoadPFNGL("glGetProgramInfoLog", &gl_module);
            glCreateShader = uWin32LoadPFNGL("glCreateShader", &gl_module);
            glCreateProgram = uWin32LoadPFNGL("glCreateProgram", &gl_module);
            glShaderSource = uWin32LoadPFNGL("glShaderSource", &gl_module);
            glCompileShader = uWin32LoadPFNGL("glCompileShader", &gl_module);
            glAttachShader = uWin32LoadPFNGL("glAttachShader", &gl_module);
            glLinkProgram = uWin32LoadPFNGL("glLinkProgram", &gl_module);
            glDeleteShader = uWin32LoadPFNGL("glDeleteShader", &gl_module);
            glGetAttribLocation = uWin32LoadPFNGL("glGetAttribLocation", &gl_module);
            glGetUniformLocation = uWin32LoadPFNGL("glGetUniformLocation", &gl_module);
            glUniform1f = uWin32LoadPFNGL("glUniform1f", &gl_module);
            glUniform2f = uWin32LoadPFNGL("glUniform2f", &gl_module);
            glUniform3f = uWin32LoadPFNGL("glUniform3f", &gl_module);
            glGenVertexArrays = uWin32LoadPFNGL("glGenVertexArrays", &gl_module);
            glBindVertexArray = uWin32LoadPFNGL("glBindVertexArray", &gl_module);
            glGenBuffers = uWin32LoadPFNGL("glGenBuffers", &gl_module);
            glBindBuffer = uWin32LoadPFNGL("glBindBuffer", &gl_module);
            glBufferData = uWin32LoadPFNGL("glBufferData", &gl_module);
            glVertexAttribPointer = uWin32LoadPFNGL("glVertexAttribPointer", &gl_module);
            glEnableVertexAttribArray = uWin32LoadPFNGL("glEnableVertexAttribArray", &gl_module);
            glGenFramebuffers = uWin32LoadPFNGL("glGenFramebuffers", &gl_module);
            glBindFramebuffer = uWin32LoadPFNGL("glBindFramebuffer", &gl_module);
            glFramebufferTexture2D = uWin32LoadPFNGL("glFramebufferTexture2D", &gl_module);
            glCheckFramebufferStatus = uWin32LoadPFNGL("glCheckFramebufferStatus", &gl_module);

            /* glClearColor(1.0f, 0.0f, 0.0f, 1.0f); */
            /* glClear(GL_COLOR_BUFFER_BIT); */
            /* SwapBuffers(UEhdc); */
            /* ReleaseDC(hwnd, UEhdc); */

            win32_proxy_event = uEVENT_NONE;
            break;
	}

	case WM_LBUTTONDOWN:
	{
            win32_proxy_event = uEVENT_MOUSE_PRESS_LEFT;
            break;
	}

	case WM_SIZE:
	{
            //OutputDebugStringA("WM_SIZE\n");
            printf("WM_SIZE\n");
            // [ cfarvin::TODO ] pass resize info to main engine
            /* int width = LOWORD(lParam); */
            /* int height = HIWORD(lParam); */
            /*printf("width: %d\nheight: %d\n", width, height);*/
            win32_proxy_event = uEVENT_RESIZE;
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	case WM_PAINT:
	{
            //OutputDebugStringA("WM_PAINT\n");
            printf("WM_PAINT\n");
            /* HGLRC gl_rendering_context = wglGetCurrentContext(); */
            /* if (!gl_rendering_context) */
            /* { */
            /*     printf("[ UE::WIN::ERROR ] Failed to obtain gl context on paint cal.\n"); */
            /* } */

            win32.device_context = wglGetCurrentDC();
            if (!win32.device_context)
            {
                printf("[ UE::WIN::ERROR ] Failed to obtain device context on paint call\n");
            }

            /* glClearColor(1.0f, 0.0f, 0.0f, 1.0f); */
            /* glClear(GL_COLOR_BUFFER_BIT); */
            /* SwapBuffers(win32.device_context); */
            ReleaseDC(win32.window, win32.device_context);
            win32_proxy_event = uEVENT_NONE;
            break;
	}
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// [ cfarvin::NOTE ] [ cfarvin::RETURN ] We just weren't loading the function pointers for
// the opengl functions. Working on making a cache of the function names and populating them
// in this function from WM_CREATE msg.
inline void *
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

    if (!pfngl)
    {
        printf("[ UE::WIN::ERROR ] Could not find PFNGL %s", fn_name);
        assert(false);
    }

    return pfngl;
}