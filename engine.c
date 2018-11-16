#include <engine_tools/type_tools.h>
#include <engine_tools/event_tools.h>
#include <engine_tools/ogl_tools.h>
#include <engine_tools/stats_tools.h>

#if __linux__
#include <nix/nix_platform.h>
#elif _WIN32
#include <win/win_platform.h>
#endif // _WIN32

//
// renderers
//
#include <renderers/triangle_renderer.h>

uint8 RUNNING = 1;

void
refreshInputState()
{
    uEVENT event = uEVENT_NONE;
#if __linux__
    event = x11_handleEvents();
#elif _WIN32
    event = win32_handleEvents();
#endif // __linux__ _WIN32

    stats.events_handled_this_loop = 0;

    switch(event)
    {
        case uEVENT_NONE:
        {
            break;
        }
        case uEVENT_CLOSE:
        {
            RUNNING = 0;
            stats.events_handled_this_loop++;
            break;
        }
        case uEVENT_MOUSE_PRESS_LEFT:
        {
            printf("ButtonPress | Mouse Left\n");
            stats.events_handled_this_loop++;
            break;
        }
        case uEVENT_MOUSE_PRESS_RIGHT:
        {
            printf("ButtonPress | Mouse Right\n");
            stats.events_handled_this_loop++;
            break;
        }
        case uEVENT_MOUSE_PRESS_MIDDLE:
        {
            printf("ButtonPress | Mouse Middle\n");
            stats.events_handled_this_loop++;
            break;
        }
        case uEVENT_MOUSE_RELEASE_LEFT:
        {
            printf("ButtonRelease | Mouse Left\n");
            stats.events_handled_this_loop++;
            break;
        }
        case uEVENT_MOUSE_RELEASE_RIGHT:
        {
            printf("ButtonRelease | Mouse Right\n");
            stats.events_handled_this_loop++;
            break;
        }
        case uEVENT_MOUSE_RELEASE_MIDDLE:
        {
            printf("ButtonRelease | Mouse Middle\n");
            stats.events_handled_this_loop++;
            break;
        }
        case uEVENT_MOUSE_SCROLL_UP:
        {
            printf("ButtonPress | Mouse Scroll Up\n");
            stats.events_handled_this_loop++;
            break;
        }
        case uEVENT_MOUSE_SCROLL_DOWN:
        {
            printf("ButtonPress | Mouse Scroll Down\n");
            stats.events_handled_this_loop++;
            break;
        }
        case uEVENT_RESIZE:
        {
            glViewport(0,
                       0,
                       viewport.width,
                       viewport.height);
        }
    }
}

void
initializeGameWindowsAndContext()
{
#if __linux__
    x11_createWindow();
#endif // __linux__

#if _WIN32
    win32_createWindow();
#endif // _WIN32
}

void
initializeRenderers()
{
    // [ cfarvin::REMOVE ] linux #if
#if __linux__
    initRenderer_triangle();
#endif // __linux__
}

inline void
swapBuffers()
{
#if __linux__
    glXSwapBuffers(x11.display, x11.engine_window);
#eleif _WIN32
    SwapBuffers(WIN32_INFO.device_context);
#endif
}

void
destroyEngine()
{
    printf("[ DESTROY ENGINE ]\n");
#if __linux__
    x11_destroy();
#endif // __linux__

}

#if _WIN32
int CALLBACK
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
#else
    int
    main(int argc, char** argv)
#endif // _WIN32
{
#if _WIN32
    win32.instance = hInstance;
    win32.command_show = nCmdShow;

    win32.class_name  = "UE Window Class";

    WNDCLASSEX window_class = { 0 };
    window_class.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    window_class.lpfnWndProc = UninstallEngineWindowProc;
    window_class.hInstance = win32.instance;
    window_class.lpszClassName = win32.class_name;
    window_class.cbSize = sizeof(WNDCLASSEX);
    /* window_class.hIcon = NULL; */
    /* window_class.hCursor = NULL; */
    /* window_class.hbrBackground = NULL; */
    /* window_class.lpszMenuName = NULL; */
    /* window_class.hIconSm = NULL; */

    if(!RegisterClassEx(&window_class))
    {
        printf("[ UE::WIN::ERROR ] Could not register window class\n");
    }

    win32.window = CreateWindowEx(0,
                                  window_class.lpszClassName,
                                  "UE",
                                  WS_OVERLAPPEDWINDOW|WS_VISIBLE,
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
    win32_handleEvents();

#endif // _WIN32

    /* initializeGameWindowsAndContext(); */
    /* initializeRenderers(); */

    while(RUNNING)
    {
        /* glError; */

        refreshInputState();

        /* glError; */
        /* /\* render_triangle(); *\/ */
        /* glError; */

        /* glError; */
        /* swapBuffers(); */
        /* glError; */
    }

    /* destroyEngine(); */

    printf("[ SUCCESS ]\n");
    return 0;
}
