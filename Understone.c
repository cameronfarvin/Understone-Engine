// Set __uDEBUG_SYSTEM__ == 1 in compiler invocation to enable system debugging
// -- msvc: /D__UE_DEBUG__#1

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

#define uVULKAN_MAX_NANOSECOND_WAIT 100000000


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
uUpdatePresentInfoAndPresent(_mut_ uVulkanDrawTools* const restrict dt,
                             const u32*              const restrict next_frame_idx)
{
    uAssertMsg_v(dt,             "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(next_frame_idx, "[ render ] Next frame index ptr must be non null.\n");


    (dt->present_info).pImageIndices   = (u32*)next_frame_idx;
    (dt->present_info).pWaitSemaphores = &(dt->signal_semaphores[dt->frame]);

#if __UE_DEBUG__
    VkResult result = vkQueuePresentKHR(dt->present_queue, &(dt->present_info));
    uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to present.\n");
#else
    vkQueuePresentKHR(dt->present_queue, &(dt->present_info));
#endif // __UE_DEBUG__
}


__UE_internal__ __UE_inline__ void
uSubmitGraphicsQueue(const uVulkanDrawTools* const restrict dt)
{
    uAssertMsg_v(dt,                 "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(dt->graphics_queue, "[ render ] VkQueue (graphics) must be non zero.\n");
    uAssertMsg_v(dt->fences,         "[ render ] VkFence ptr must be non null.\n");

#if __UE_DEBUG__
    VkResult result = vkQueueSubmit(dt->graphics_queue, 1, &(dt->submit_info), dt->fences[dt->frame]);
    uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to submit queue.\n");
#else
    vkQueueSubmit(dt->graphics_queue, 1, &(dt->submit_info), dt->fences[dt->frame]);
#endif // __UE_DEBUG__
}


__UE_internal__ __UE_inline__ void
uUpdateQueueSubmissionOrder(_mut_ uVulkanDrawTools* const restrict dt,
                            const u32*              const restrict next_frame_idx)
{
    uAssertMsg_v(dt,             "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(next_frame_idx, "[ render ] Next frame index ptr must be non null.\n");


    (dt->submit_info).pCommandBuffers   = (VkCommandBuffer*)(&dt->command_buffers[*next_frame_idx]);
    (dt->submit_info).pWaitSemaphores   = &(dt->wait_semaphores[dt->frame]);
    (dt->submit_info).pSignalSemaphores = &(dt->signal_semaphores[dt->frame]);
}


__UE_internal__ __UE_inline__ void
uEnsureFrameReadiness(_mut_ uVulkanDrawTools* const restrict dt,
                      const u32*              const restrict next_frame_idx)
{
    uAssertMsg_v(dt,                  "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(dt->logical_device,  "[ render ] VkDevice must be non zero.\n");
    uAssertMsg_v(dt->fences,          "[ render ] VkFence ptr must be non null.\n");
    uAssertMsg_v(next_frame_idx, "[ render ] Next frame index ptr must be non null.\n");


    VkResult result = vkWaitForFences(dt->logical_device,
                                      1,
                                      &dt->fences[dt->frame],
                                      VK_TRUE,
                                      uVULKAN_MAX_NANOSECOND_WAIT);

    uAssertMsg_v(result != VK_TIMEOUT, "[ render ] [ timeout ] Fence timeout on frame: %d.\n", dt->frame);
    vkResetFences(dt->logical_device, 1, &dt->fences[dt->frame]);

    // [ cfarvin::REVISIT ] Is this necessary?
    if (dt->swap_chain_image_fences[*next_frame_idx] != VK_NULL_HANDLE)
    {
        result = vkWaitForFences(dt->logical_device,
                                 1,
                                 &(dt->swap_chain_image_fences[*next_frame_idx]),
                                 VK_TRUE,
                                 uVULKAN_MAX_NANOSECOND_WAIT);

        uAssertMsg_v(result != VK_TIMEOUT, "[ render ] [ timeout ] Fence timeout on image: %d.\n", *next_frame_idx);
        uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to ensure frame readiness.\n");
    }

    dt->swap_chain_image_fences[*next_frame_idx] = dt->fences[dt->frame];
}


__UE_internal__ __UE_inline__ void
uAcquireNextSwapChainFrameIndex(const uVulkanDrawTools* const restrict dt,
                                _mut_ u32*              const restrict return_idx)
{
    uAssertMsg_v(dt,                 "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(dt->logical_device, "[ render ] VkDevice must be non zero.\n");
    uAssertMsg_v(dt->swap_chain,     "[ render ] VkSwapchainKHR must be non zero.\n");
    uAssertMsg_v(return_idx,         "[ render ] Return index ptr must be non null.\n");



#if __UE_DEBUG__
    VkResult result = vkAcquireNextImageKHR(dt->logical_device,
                                            dt->swap_chain,
                                            uVULKAN_MAX_NANOSECOND_WAIT,
                                            dt->wait_semaphores[dt->frame], // Sets the semaphore
                                            VK_NULL_HANDLE,
                                            return_idx);
#else
    vkAcquireNextImageKHR(dt->logical_device,
                          dt->swap_chain,
                          uVULKAN_MAX_NANOSECOND_WAIT,
                          dt->wait_semaphores[dt->frame], // Sets the semaphore
                          VK_NULL_HANDLE,
                          return_idx);
#endif // __UE_DEBUG__


    uAssertMsg_v(result != VK_TIMEOUT,
                 "[ render ] [ timeout ] Could not acquire next swap chain image.\n");
    uAssertMsg_v(result == VK_SUCCESS,
                 "[ render ] Could not acquire next swap chain image.\n");
    uAssertMsg_v((*return_idx <= (u32)uVULKAN_MAX_FRAMES_IN_FLIGHT),
                 "[ redner ] Acquired swap chain image index greater than frame count.\n");
}


__UE_internal__ __UE_inline__ void
uDrawFrame(_mut_ uVulkanDrawTools* const restrict dt)
{
    uAssertMsg_v(dt, "[ render ] uVulkanDrawtools must be non zero.\n");


    u32 next_frame_idx = 0;
    uAcquireNextSwapChainFrameIndex(dt, &next_frame_idx);
    uEnsureFrameReadiness(dt, &next_frame_idx);
    uUpdateQueueSubmissionOrder(dt, &next_frame_idx);
    uSubmitGraphicsQueue(dt);
    uUpdatePresentInfoAndPresent(dt, &next_frame_idx);

    // Update frame count
    dt->frame = (dt->frame + 1) % uVULKAN_MAX_FRAMES_IN_FLIGHT;
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
        // Wait for device to be idle
        vkDeviceWaitIdle(v_info->logical_device);

        for (u32 sync_obj_idx = 0; sync_obj_idx < uVULKAN_MAX_FRAMES_IN_FLIGHT; sync_obj_idx++)
        {
            vkDestroySemaphore(v_info->logical_device, draw_tools->wait_semaphores[sync_obj_idx], NULL);
            vkDestroySemaphore(v_info->logical_device, draw_tools->signal_semaphores[sync_obj_idx], NULL);
            vkDestroyFence(v_info->logical_device,     draw_tools->fences[sync_obj_idx], NULL);
        }
// [ cfarvin::REVISIT ] These don't get destroyed properly. See creation in vulkan_tools.h::uCreateVulkanDrawSyncTools()
#if 0
        for (u8 fence_idx = 0; fence_idx < uVULKAN_NUM_COMMAND_BUFFERS; fence_idx++)
        {
            vkDestroyFence(v_info->logical_device, draw_tools->swap_chain_image_fences[fence_idx], NULL);
        }
#endif // 0
    }
}


__UE_internal__ __UE_call__ void
uDestroyEngine()
{
    uDestroyVulkan();

#if _WIN32
    uDestroyWin32((uWin32Info* const)win32_info);
#endif // _WIN32
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

#if __UE_DEBUG__
    printf("[ build ] DEBUG\n");
#else
    printf("[ build ] RELEASE\n");
#endif

    uVulkanDrawTools draw_tools = { 0 };

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

    uDebugPrint("[ engine ] Graceful exit.\n");
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
