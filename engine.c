#include <engine_tools/type_tools.h>
#include <engine_tools/event_tools.h>
#include <engine_tools/ogl_tools.h>
#include <engine_tools/stats_tools.h>
#include <data_structures/data_structures.h>
#include <tests/tests.h>

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
    /* printf("viewport.width: %ld\nviewport.height: %ld\n", viewport.width, viewport.height); */
    glViewport(0, 0, viewport.width, viewport.height);
    /* glMatrixMode(GL_PROJECTION); */
    /* glOrtho(0.0f, viewport.width, viewport.height, 0.0f, 0.0f, 1.0f); */
}

// Query the mouse and keyboard state
void
uRefreshInputState()
{
    uSystemEvent sys_event = uEventNone;
#if __linux__
    sys_event = uX11HandleEvents();
#elif _WIN32
    sys_event = uWin32HandleEvents();
#else
    assert(0);
#endif // __linux__ _WIN32
    switch(sys_event)
    {
        case uEventNone:
        {
            return;
        }
        case uEventResize:
        {
            uHandleWindowResize();
            return;
        }
        case uEventClose:
        {
            RUNNING = false;
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

int main(int argc, char** argv)
{
#if _WIN32
    win32.instance = GetModuleHandle(NULL);
    win32.command_show = 10;
    win32.class_name  = "UE Window Class";
#endif // _WIN32

#if RUN_TESTS_ON_STARTUP
    runAllTests();
#endif

    printf("[ UNINSTALL ENGINE ]\n");
    if (argc)
    {
        for (size_t ii = 0; ii < (size_t) argc; ii++)
        {
            printf("\targ%zd: %s\n", ii, argv[ii]);
        }
    }

    uInitializeGameWindowsAndContext();
    uInitializeRenderers();

    // [ cfarvin::DEBUG ] [ cfarvin::REMOVE ] [ cfarvin::EXPERIMENTAL ]
    r32 piCycle = 0;
    r32 cycleDelta = 0.025f;

    GLfloat transform[16] =
        {
            +1.0f, +0.0f, +0.0f, +0.0f,
            +0.0f, +1.0f, +0.0f, +0.0f,
            +0.0f, +0.0f, +1.0f, +0.0f,
            +0.0f, +0.0f, +0.0f, +1.0f,
        };

    while(RUNNING)
    {
        glError;

        uRefreshInputState();

        glError;

        if (piCycle > uPI)
        {
            piCycle = 0;
        }

        piCycle += cycleDelta;
        if (uGetInputPressed(uMouse_right))
        {
            // red triangle
            glUniform3f(triangle_renderer.fshdr_color_location, (GLfloat) sin(piCycle), 0.0, 0.0f);
        }
        else if (uGetInputPressed(uMouse_left))
        {
            // blue triangle
            glUniform3f(triangle_renderer.fshdr_color_location, 0.0f, 0.0f, (GLfloat) sin(piCycle));
            /* printf("mouse_pos.x: %ld\nmouse_pos.y: %ld\nviewport.width: %ld\nviewport.height: %ld\nmouse_x_corrected: %f\nmouse_y_corrected: %f\n", */
            /*        mouse_pos.x, */
            /*        mouse_pos.y, */
            /*        viewport.width, */
            /*        viewport.height, */
            /*        ((r32)mouse_pos.x / (r32)viewport.width), */
            /*        ((r32)mouse_pos.y / (r32)viewport.height)); */
        }
        else
        {
            // green triangle
            glUniform3f(triangle_renderer.fshdr_color_location, 0.0f, (GLfloat) sin(piCycle), 0.0f);
        }


        transform[3] = -((viewport.width - mouse_pos.x) / (viewport.width / 2.0f) - 1);
        transform[7] = -((viewport.height - mouse_pos.y) / (viewport.height / 2.0f) - 1);
        /* transform[3] = (mouse_pos.x / (r32)viewport.width); */
        /* transform[7] = (mouse_pos.y / (r32)viewport.height); */

        glUniformMatrix4fv(triangle_renderer.vshdr_mut_position_location,
                           1,
                           GL_TRUE,
                           transform);

        render_triangle(&triangle_renderer);
        glError;

        uSwapBuffers();
        glError;
    }

    uDestroyEngine();

    printf("[ SUCCESS ]\n");
    return 0;
}
