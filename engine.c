#include <engine_tools/type_tools.h>
#include <engine_tools/event_tools.h>
#include <engine_tools/ogl_tools.h>
#include <engine_tools/stats_tools.h>

#include <nix/nix_platform.h>
#include <win/win_platform.h>

//
// renderers
//
#include <renderers/triangle_renderer.h>

uint8 RUNNING = 1;

void
refreshInputState()
{
#if __linux__
    EVENT event = x11_handleEvents();
#endif // __linux__
        
    stats.events_handled_this_loop = 0;
                    
    switch(event)
    {
        case EVENT_NONE:
        {
            break;
        }
        case EVENT_CLOSE:
        {
            RUNNING = 0;
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_PRESS_LEFT:
        {
            printf("ButtonPress | Mouse Left\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_PRESS_RIGHT:
        {
            printf("ButtonPress | Mouse Right\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_PRESS_MIDDLE:
        {
            printf("ButtonPress | Mouse Middle\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_RELEASE_LEFT:
        {
            printf("ButtonRelease | Mouse Left\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_RELEASE_RIGHT:
        {
            printf("ButtonRelease | Mouse Right\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_RELEASE_MIDDLE:
        {
            printf("ButtonRelease | Mouse Middle\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_MOUSE_SCROLL_UP:
        {
            printf("ButtonPress | Mouse Scroll Up\n");
            stats.events_handled_this_loop++;                    
            break;
        }
        case EVENT_MOUSE_SCROLL_DOWN:
        {
            printf("ButtonPress | Mouse Scroll Down\n");
            stats.events_handled_this_loop++;
            break;
        }
        case EVENT_RESIZE:
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
}

void
initializeRenderers()
{
    initRenderer_triangle();
}

void
swapBuffers()
{
#if __linux__
    glXSwapBuffers(x11.display, x11.engine_window);
#endif // __linux__
    
}

void
destroyEngine()
{
    printf("[ DESTROY ENGINE ]\n");
#if __linux__
    x11_destroy();
#endif // __linux__

}

int
main(int argc, char** argv)
{
    initializeGameWindowsAndContext();
    initializeRenderers();

    while(RUNNING)
    {
        glError;
        
        refreshInputState();

        glError;
        render_triangle();
        glError;

        glError;
        swapBuffers();
        glError;
    }

    destroyEngine();
    return 0;
}
