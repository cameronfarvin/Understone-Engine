//
//
//  Understone Engine
//  An acedmic dabbling in masochism.
//
//  By: Cameron Farvin // assemblyDruid
//
//

// Set __uDEBUG_SYSTEM__ == 0 in debug_tools.h to disable system debugging functionality,
#include <engine_tools/debug_tools.h>
#include <engine_tools/type_tools.h>
#include <engine_tools/memory_tools.h>
#include <engine_tools/event_tools.h>
// Set __uDEBUG_GL__ == 0 in ogl_tools.h to disable all debugging functionality,
#include <engine_tools/ogl_tools.h>
#include <engine_tools/stats_tools.h>
#include <data_structures/data_structures.h>
// Set __uTESTS_ENABLED__ == 0 in tests.h to disable tests on startup
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
    /* uDBGPrint("viewport.width: %ld\nviewport.height: %ld\n", viewport.width, viewport.height); */
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
    glError;
    /* initRenderer_triangle(); */
    /* initRenderer_test_bitmap(); */
    /* initRenderer_texture_test(); */
    glError;
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
    uDBGPrint("[ DESTROY ENGINE ]\n");
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

#if __uTESTS_ENABLED__
    // See tests/tests.h to disable
    runAllTests();
#endif

    uDBGPrint("[ UNDERSTONE ENGINE ]\n");
    if (argc)
    {
        for (size_t ii = 0; ii < (size_t) argc; ii++)
        {
            uDBGPrint("\targ%zd: %s\n", ii, argv[ii]);
        }
    }

    uDetermineSystemEndianness();
    /* uInitializeGameWindowsAndContext(); */
    /* uInitializeRenderers(); */

    /* /\* glClearColor(0.8f, 0.16f, 0.32f, 1.0f); *\/ // pinkish */
    /* glClearColor(0.3f, 0.5f, 0.7f, 1.0f); */

    /* while(RUNNING) */
    /* { */
    /*     glClear(GL_COLOR_BUFFER_BIT); */
    /*     uRefreshInputState(); */

    /*     /\* render_triangle(); *\/ */
    /*     /\* render_test_bitmap(&test_bitmap_renderer); *\/ */
    /*     /\* render_texture_test(&texture_test_renderer); *\/ */

    /*     uSwapBuffers(); */
    /*     glError; */
    /* } */

    uDestroyEngine();

    uDBGPrint("[ SUCCESS ]\n");
    return 0;
}

/*
  In Progress:
  - Arena allocation
  - Improve OGL system
  - Improve debug/error reporting system
  - Annotate all functions

  Master TODO:
  - Textures/Load Bitmap (d: 6)
  - Fonts (d: 10)

  - Particle System (d: 6)
  - Data Structures (Hash, List) (d: 5)
  - Statistics update (d: 2)

  - #define + method that takes __FILE__ and __LINE__ in shader comp error reporting (d: 3)
  - Parse all files for preproc directive uBKPTERR, load those lines into GDB as breakpoints
  - Vulkan(d: 13)
*/
