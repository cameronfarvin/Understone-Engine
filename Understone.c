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
// [ begin ] Global members
__UE_global__ bool RUNNING = true;

__UE_global__ char* required_instance_validation_layers[] =
{
    "VK_LAYER_KHRONOS_validation"
};

__UE_global__ char* required_instance_extensions[] =
{
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    "VK_KHR_surface",
#if _WIN32
    "VK_KHR_win32_surface"
#endif // _WIN32
};

__UE_global__ char* required_device_extensions[] =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
// [ end ] Global members
//


__UE_internal__ __UE_inline__ void
uHandleWindowResize()
{
    uDebugPrint("TODO: Handle Window Resize\n");
    uDebugPrint("viewport.width: %ld\nviewport.height: %ld\n",
                viewport.width,
                viewport.height);
}


// Query the mouse and keyboard state
__UE_internal__ __UE_inline__ void
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


__UE_internal__ __UE_inline__ void
uDrawFrame(const uVulkanDrawTools* const restrict dt)
{
    uAssertMsg_v(dt,                 "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(dt->logical_device, "[ render ] VkDevice must be non zero.\n");
    uAssertMsg_v(dt->swap_chain,     "[ render ] VkSwapchainKHR must be non zero.\n");

    // [ cfarvin::TODO ] check return codes
    u32 image_idx;
    vkAcquireNextImageKHR(dt->logical_device,
                          dt->swap_chain,
                          ~((u64)0),
                          (dt->wait_semaphores)[0],
                          VK_NULL_HANDLE,
                          &image_idx);

    VkSubmitInfo* submit_info = (VkSubmitInfo*)&dt->submit_info;
    submit_info->pCommandBuffers = (VkCommandBuffer*)(&dt->command_buffers[image_idx]);

    VkResult result = vkQueueSubmit(dt->graphics_queue,
                                    1,
                                    submit_info,
                                    VK_NULL_HANDLE);
    if (result != VK_SUCCESS)
    {
        uFatal("[ render ] Unable to submit queue.\n");
    }

    VkPresentInfoKHR present_info = { 0 };
    present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores    = &(dt->signal_semaphores[0]);
    present_info.swapchainCount     = 1;
    present_info.pSwapchains        = &(dt->swap_chain);
    present_info.pImageIndices      = &image_idx;
    present_info.pResults           = NULL;

    result = vkQueuePresentKHR(dt->present_queue, &present_info);

    if (result != VK_SUCCESS)
    {
        uFatal("[ render ] Unable to present.\n");
    }
}


__UE_internal__ __UE_call__ void
uDestroyDrawTools(_mut_ uVulkanDrawTools* const restrict draw_tools)
{
    uVulkanInfo* v_info = (uVulkanInfo*)uGetVulkanInfo();

    uAssertMsg_v(v_info,                 "[ engine ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ engine ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(draw_tools,             "[ engine ] uVulkanDrawTools ptr must be non null.\n");


    if (draw_tools && v_info && v_info->logical_device)
    {
        for (u32 wait_idx = 0; wait_idx < uVULKAN_NUM_WAIT_SEMAPHORES; wait_idx++)
        {
            vkDestroySemaphore(v_info->logical_device, draw_tools->wait_semaphores[wait_idx], NULL);
        }

        for (u32 signal_idx = 0; signal_idx < uVULKAN_NUM_SIGNAL_SEMAPHORES; signal_idx++)
        {
            vkDestroySemaphore(v_info->logical_device, draw_tools->signal_semaphores[signal_idx], NULL);
        }
    }
}


__UE_internal__ __UE_call__ void
uDestroyEngine()
{
    // Note: Vulkan tears down platform surfaces/windows
    uDestroyVulkan(uGetVulkanInfo());
}


int main(int argc, char** argv)
{
// See tests/tests.h to disable
#if __uTESTS_ENABLED__
    runAllTests();
#endif

#if __UE_DEBUG__
#ifdef _WIN32
    // Enable _CRT Allocation Analysis
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR,  _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR,  _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_WARN,   _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN,   _CRTDBG_FILE_STDOUT);
#endif // _WIN32
#endif // __UE_DEBUG__

    if (argc && argv) {}

    const uVulkanDrawTools draw_tools = { 0 };

    uInitializeVulkan(&draw_tools,
                      required_instance_validation_layers,
                      sizeof(required_instance_validation_layers)/sizeof(char*),
                      required_instance_extensions,
                      sizeof(required_instance_extensions)/sizeof(char*),
                      required_device_extensions,
                      sizeof(required_device_extensions)/sizeof(char*));

    while(RUNNING)
    {
        uDrawFrame(&draw_tools);
        uRefreshInputState();
    }

    uDestroyDrawTools((uVulkanDrawTools*)&draw_tools);
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
