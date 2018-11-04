#include <engine_tools.h>
#include <nix_platform.h>
#include <win_platform.h>
#include <stdint.h>

static uint8 RUNNING = 1;

void refreshInputState()
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
    }
}


int main(int argc, char** argv)
{
    for (size_t cl_arg = 0; cl_arg < (size_t) argc; cl_arg++)
    {
        printf("cl_arg[%zd]: %s\n", cl_arg, argv[cl_arg]);
    }
    
#if __linux__
    x11_createWindow();
#endif // __linux__

    glClearColor(0.5f, 0.05f, 0.73f, 1.0f);

    while(RUNNING)
    {
        refreshInputState();
        glClear(GL_COLOR_BUFFER_BIT);


#if __linux__
        glXSwapBuffers(x11.display, x11.engine_window);
#endif // __linux__
    }

#if __linux__
    x11_destroy();
#endif // __linux__
    
    return 0;
}
