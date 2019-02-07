#include <engine_tools/type_tools.h>
#include <engine_tools/event_tools.h>
#include <engine_tools/ogl_tools.h>
#include <engine_tools/stats_tools.h>

#if __linux__
#include <nix/nix_platform.h>
#elif _WIN32
#include <win/win_platform.h>
#endif // _WIN32

#include <renderers/master_renderer.h>

bool RUNNING = true;

void
uHandleWindowResize()
{
    printf("uHandleWindowResize()\n");
}

void
uRefreshInputState()
{
#if __linux__
    uEVENT event = uX11HandleEvents();
#elif _WIN32
    uEVENT event = uWin32HandleEvents();
#endif // __linux__ _WIN32

    stats.events_handled_this_loop = 0;

    switch(event)
    {
        case uEVENT_NONE:
        {
            break;
        }
        case uEVENT_RESIZE:
        {
            glViewport(0,
                       0,
                       viewport.width,
                       viewport.height);
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

        default:
        {
            return;
        }
    }
}

void
uInitializeGameWindowsAndContext()
{
#if __linux__
    uX11CreateWindow();
#endif // __linux__

#if _WIN32
    uWin32CreateWindow();
#endif // _WIN32

    uRefreshInputState();
}

void
uInitializeRenderers()
{
    initRenderer_triangle(&triangle_renderer);
}

static inline void
uSwapBuffers()
{
#if __linux__
    glXSwapBuffers(x11.display, x11.engine_window);
#elif _WIN32
    SwapBuffers(win32.device_context);
#endif
}

void
uDestroyEngine()
{
    printf("[ DESTROY ENGINE ]\n");
#if __linux__
    uX11Destroy();
#endif // __linux__
    // [ cfarvin::TODO ] destroy eingine functionality for win32

}

/* #if _WIN32 */
/* int CALLBACK */
/* WinMain(HINSTANCE hInstance, */
/*         HINSTANCE hPrevInstance, */
/*         LPSTR lpCmdLine, */
/*         int nCmdShow) */
/* #else */
/*     int */
/*     main(int argc, char** argv) */
/* #endif // _WIN32 */
int main(int argc, char** argv)
{
#if _WIN32
    // win32.instance = hInstance;
    win32.instance = GetModuleHandle(NULL);
    // [ cfarvin::NOTE ] nCmdShow usually passed from WinMain,
    // Bypassing for now to use SUBSYSTEM:CONSOLE, hard passing
    // value of <10> "SW_SHOWDEFAULT"
    /* win32.command_show = nCmdShow; */
    win32.command_show = 10;
    win32.class_name  = "UE Window Class";
#endif // _WIN32

    uInitializeGameWindowsAndContext();
    uInitializeRenderers();

    while(RUNNING)
    {
        glError;

        uRefreshInputState();

        glError;
        render_triangle(&triangle_renderer);
        glError;

        uSwapBuffers();
        glError;
    }

    uDestroyEngine();

    printf("[ SUCCESS ]\n");
    return 0;
}
