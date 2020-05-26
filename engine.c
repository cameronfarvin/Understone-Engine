// Set __uDEBUG_SYSTEM__ == 0 in debug_tools.h to disable system
// debugging functionality
#include <engine_tools/debug_tools.h>
#include <engine_tools/type_tools.h>
#include <engine_tools/memory_tools.h>
#include <engine_tools/event_tools.h>
#include <engine_tools/vulkan_tools.h>

#include <data_structures/data_structures.h>

// Set __uTESTS_ENABLED__ == 0 in tests.h to disable tests on startup
#include <tests/tests.h>

#if __linux__
#include <nix/nix_platform.h>
#elif _WIN32
#include <win/win_platform.h>
#endif // _WIN32



//
// Global Members
//
bool RUNNING = true;

uVulkanInfo main_vulkan = { 0 };

char* required_validation_layers[] =
{
    "VK_LAYER_KHRONOS_validation"
};

char* required_extensions[] =
{
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    "VK_KHR_surface",
#if _WIN32
    "VK_KHR_win32_surface"
#endif // _WIN32
};


void
uHandleWindowResize()
{
    uDebugPrint("TODO: Handle Window Resize\n");
    uDebugPrint("viewport.width: %ld\nviewport.height: %ld\n",
                viewport.width,
                viewport.height);
}

// Query the mouse and keyboard state
void
uRefreshInputState()
{
    uSystemEvent sys_event = uEventNone;

#if __linux__
    sys_event = uNixHandleEvents();
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
uDestroyEngine()
{
    // Note: Vulkan tears down platform surfaces/windows
    uDestroyVulkan(&main_vulkan);
}


int main(int argc, char** argv)
{

// See tests/tests.h to disable
#if __uTESTS_ENABLED__
    runAllTests();
#endif

    if (argc && argv) {}

    // [ cfarvin::TODO ] App name
    uInitializeVulkan(&main_vulkan,
                      "Understone",
                      required_validation_layers,
                      sizeof(required_validation_layers)/sizeof(char*),
                      required_extensions,
                      sizeof(required_extensions)/sizeof(char*));

    while(RUNNING)
    {
        uRefreshInputState();
    }

    uDestroyEngine();
    return 0;
}


/*
  Priority:
  - Abandon OpenGL in favor of Vulkan
  - Abandon X11 in favor of Weyland

  In Progress:
  - Arena allocation
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
