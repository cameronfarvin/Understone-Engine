#include "type_tools.h"
bool kRunning = true;

#if __linux__
#include "nix_platform.h"
#elif _WIN32
// Note: platform win32 khr macro must be included before
//       vulkan/vulkan.h header
#define VK_USE_PLATFORM_WIN32_KHR 1
#include "win_platform.h"
#endif // _WIN32

// Set __uDEBUG_SYSTEM__ == 1 in compiler invocation to enable system debugging
// -- msvc: /D__UE_debug__ == 1#1
#include "data_structures.h"
#include "debug_tools.h"
#include "event_tools.h"
#include "memory_tools.h"
#include "vulkan_tools/shader_tools.h"
#include "vulkan_tools/vulkan_tools.h"
#include "window_tools.h"

// Set __uTESTS_ENABLED__ == 0 in tests.h to disable tests on startup
#include "tests/tests.h"

//
// [ begin ] Global members
size_t kTotalFrameCount = 0;
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
const char* kRequiredInstanceValidationLayers[] = { "VK_LAYER_KHRONOS_validation" };
#endif //  __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

const char* kRequiredInstanceExtensions[] = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
                                              "VK_KHR_surface",
#if _WIN32
                                              "VK_KHR_win32_surface"
#endif // _WIN32
};

const char* kRequiredDeviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
// [ end ] Global members
//

static __UE_inline__ void
uHandleWindowResize()
{
    uDebugPrint("[ resize ] width: %d, height: %d\n", kGameWindow.width, kGameWindow.height);

    if (!kGameWindow.is_minimized)
    {
        uRebuildVulkanSwapChain();
        kVulkanDrawToolsOutdated = true;
    }
}

// Query the mouse and keyboard state
static __UE_inline__ void
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

    switch (sys_event)
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
            kRunning = false;
            return;
        }
    }
}

static __UE_inline__ void
uUpdatePresentInfoAndPresent(uVulkanDrawTools* const restrict dt, const u32* const restrict next_frame_idx)
{
    uAssertMsg_v(dt, "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(next_frame_idx, "[ render ] Next frame index ptr must be non null.\n");

    (dt->present_info).pImageIndices   = ( u32* )next_frame_idx;
    (dt->present_info).pWaitSemaphores = &(dt->render_finished[dt->frame]);

    VkResult result = vkQueuePresentKHR(dt->present_queue, &(dt->present_info));

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        uVkVerbose("Swap chain was out of date.\n");
        kVulkanDrawToolsOutdated = true;
        uRebuildVulkanSwapChain();
    }
    else if (result == VK_SUBOPTIMAL_KHR)
    {
        uVkVerbose("Swap chain was suboptimal.\n");
        kVulkanDrawToolsOutdated = true;
        uRebuildVulkanSwapChain();
    }
    else
    {
        uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to present.\n");
    }
}

static __UE_inline__ void
uUpdateGraphicsInfoAndSubmit(uVulkanDrawTools* const restrict dt, const u32* const restrict next_frame_idx)
{
    uAssertMsg_v(dt, "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(dt->graphics_queue, "[ render ] VkQueue (graphics) must be non zero.\n");
    uAssertMsg_v(dt->in_flight_fences, "[ render ] VkFence ptr must be non null.\n");
    uAssertMsg_v(next_frame_idx, "[ render ] Next frame index ptr must be non null.\n");
    uAssertMsg_v(*next_frame_idx <= uVULKAN_NUM_COMMAND_BUFFERS, "[ vulkan ] Next frame index value exceeds command buffer length.\n");

    (dt->submit_info).pCommandBuffers   = ( VkCommandBuffer* )(&dt->command_buffers[*next_frame_idx]);
    (dt->submit_info).pWaitSemaphores   = &(dt->image_available[dt->frame]); // what to wait on before execution
    (dt->submit_info).pSignalSemaphores = &(dt->render_finished[dt->frame]); // what to signal when execution is done

    uDebugStatement(VkResult result =) vkQueueSubmit(dt->graphics_queue, 1, &(dt->submit_info), dt->in_flight_fences[dt->frame]);
    uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to submit graphics queue.\n");
}

static __UE_inline__ void
uEnsureFrameLanded(uVulkanDrawTools* const restrict dt, const u32* const restrict next_frame_idx)
{
    uAssertMsg_v(dt, "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(dt->logical_device, "[ render ] VkDevice must be non zero.\n");
    uAssertMsg_v(dt->in_flight_fences, "[ render ] VkFence ptr must be non null.\n");
    uAssertMsg_v(next_frame_idx, "[ render ] Next frame index ptr must be non null.\n");

    if (dt->in_flight_images[*next_frame_idx] != VK_NULL_HANDLE)
    {
        uDebugStatement(VkResult result =) vkWaitForFences(dt->logical_device, 1, &(dt->in_flight_images[*next_frame_idx]), VK_TRUE, uVULKAN_MAX_NANOSECOND_WAIT);

        uAssertMsg_v(result != VK_TIMEOUT, "[ render ] [ timeout ] Fence timeout on image: %d.\n", *next_frame_idx);
        uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to ensure frame readiness.\n");
    }

    dt->in_flight_images[*next_frame_idx] = dt->in_flight_fences[dt->frame];

    vkResetFences(dt->logical_device, 1, &(dt->in_flight_fences[dt->frame]));
}

static __UE_inline__ void
uAcquireNextSwapChainFrameIndex(const uVulkanDrawTools* const restrict dt, u32* const restrict return_idx)
{
    uAssertMsg_v(dt, "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(dt->logical_device, "[ render ] VkDevice must be non zero.\n");
    uAssertMsg_v(dt->swap_chain, "[ render ] VkSwapchainKHR must be non zero.\n");
    uAssertMsg_v(return_idx, "[ render ] Return index ptr must be non null.\n");

    VkResult result = vkAcquireNextImageKHR(dt->logical_device, dt->swap_chain, uVULKAN_MAX_NANOSECOND_WAIT, dt->image_available[dt->frame], NULL, return_idx);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        uVkVerbose("Swap chain was out of date.\n");
        kVulkanDrawToolsOutdated = true;
        uRebuildVulkanSwapChain();
    }
    else if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        uVkVerbose("Swap chain was suboptimal.\n");
        kVulkanDrawToolsOutdated = true;
        uRebuildVulkanSwapChain();
    }
    else
    {
        uAssertMsg_v(result != VK_TIMEOUT, "[ render ] [ timeout ] Could not acquire next swap chain image.\n");
        uAssertMsg_v(result == VK_SUCCESS, "[ render ] Could not acquire next swap chain image.\n");
        uAssertMsg_v((*return_idx <= ( u32 )uVULKAN_MAX_FRAMES_IN_FLIGHT),
                     "[ redner ] Acquired swap chain image index greater than "
                     "frame count.\n");
    }
}

static __UE_inline__ void
uDrawFrame(uVulkanDrawTools* const restrict dt)
{
    // Don't do this if the window is minimized
    if (kGameWindow.is_minimized) { return; }

    uAssertMsg_v(dt, "[ render ] uVulkanDrawtools must be non zero.\n");

    vkWaitForFences(dt->logical_device, 1, &(dt->in_flight_fences[dt->frame]), VK_TRUE, uVULKAN_MAX_NANOSECOND_WAIT);

    u32 next_frame_idx = 0;
    uAcquireNextSwapChainFrameIndex(dt, &next_frame_idx);
    uEnsureFrameLanded(dt, &next_frame_idx);
    uUpdateGraphicsInfoAndSubmit(dt, &next_frame_idx);
    uUpdatePresentInfoAndPresent(dt, &next_frame_idx);

    // increment frame number
    dt->frame = (dt->frame + 1) % uVULKAN_MAX_FRAMES_IN_FLIGHT;
    kTotalFrameCount++;
}

static void
uDestroyEngine()
{
    uAssertMsg_v(!kRunning, "[ engine ] Tear down called while ` kRunning == true`.\n");
    uDestroyVulkan();

#if _WIN32
    uDestroyWin32();
#endif // _WIN32
}

int
main(int argc, char** argv)
{
// See tests/tests.h to disable
#if __uTESTS_ENABLED__
    runAllTests();
#endif

#if __UE_debug__ == 1
#ifdef _WIN32
    // Enable _CRT Allocation Analysis
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
#endif // _WIN32
#endif // __UE_debug__ == 1

    if (argc && argv) {}

#if __UE_debug__ == 1
    printf("[ engine ] - debug -\n");
#else
    printf("[ engine ] - release -\n");
#endif

    uVulkanDrawTools draw_tools = {};

    uInitializeVulkan(&draw_tools,
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                      ( const s8** )kRequiredInstanceValidationLayers,
                      sizeof(kRequiredInstanceValidationLayers) / sizeof(char*),
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                      ( const s8** )kRequiredInstanceExtensions,
                      sizeof(kRequiredInstanceExtensions) / sizeof(char*),
                      ( const s8** )kRequiredDeviceExtensions,
                      sizeof(kRequiredDeviceExtensions) / sizeof(char*));

    while (kRunning)
    {
        if (kVulkanDrawToolsOutdated && !kGameWindow.is_minimized) { uRebuidlDrawTools(&draw_tools); }

        uDrawFrame(&draw_tools);
        uRefreshInputState();
    }

    uDestroyDrawTools(( uVulkanDrawTools* )&draw_tools, false);
    uDestroyEngine();

    printf("[ engine ] Graceful exit.\n");
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

  - #define + method that takes __FILE__ and __LINE__ in shader comp error
  reporting (d: 3)
  - Parse all files for preproc directive uBKPTERR, load those lines into GDB as
  breakpoints
  - Vulkan(d: 13)
*/
