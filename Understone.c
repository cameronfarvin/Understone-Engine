#include <engine_tools/type_tools.h>
__UE_global__ bool RUNNING = true;

// Set __uDEBUG_SYSTEM__ == 1 in compiler invocation to enable system debugging
// -- msvc: /D__UE_debug__ == 1#1
#include <engine_tools/debug_tools.h>

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

#if __UE_debug__ == 1
#define uVULKAN_MAX_NANOSECOND_WAIT ~((u64)0)
#else
#define uVULKAN_MAX_NANOSECOND_WAIT 100000
#endif // __UE_debug__ == 1


//
// [ begin ] Global members
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
__UE_global__ char* required_instance_validation_layers[] =
{
    "VK_LAYER_KHRONOS_validation"
};
#endif //  __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

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
    (dt->present_info).pWaitSemaphores = &(dt->is_render_complete[dt->frame]);

    uDebugStatement(VkResult result = )vkQueuePresentKHR(dt->present_queue, &(dt->present_info));
    uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to present.\n");
}


__UE_internal__ __UE_inline__ void
uSubmitGraphicsQueue(const uVulkanDrawTools* const restrict dt)
{
    uAssertMsg_v(dt,                   "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(dt->graphics_queue,   "[ render ] VkQueue (graphics) must be non zero.\n");
    uAssertMsg_v(dt->in_flight_fences, "[ render ] VkFence ptr must be non null.\n");


    uDebugStatement(VkResult result = )vkQueueSubmit(dt->graphics_queue,
                                                     1,

                                                     // Regarding dt->submit_info:
                                                     //   cbuffer filled w/ _next_ frame commands;
                                                     //   awaits dt->is_image_acquired[dt->frame]
                                                     //   signals dt->is_render_complete[dt->frame]
                                                     &(dt->submit_info),

                                                     // Signal this fence when cbuffer execution finishes
                                                     dt->in_flight_fences[dt->frame]);
    uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to submit queue.\n");
}


__UE_internal__ __UE_inline__ void
uUpdateQueueSubmissionOrder(_mut_ uVulkanDrawTools* const restrict dt,
                            const u32*              const restrict next_frame_idx)
{
    uAssertMsg_v(dt,             "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(next_frame_idx, "[ render ] Next frame index ptr must be non null.\n");
    uAssertMsg_v(*next_frame_idx <= uVULKAN_NUM_COMMAND_BUFFERS,
        "[ vulkan ] Next frame index value exceeds command buffer length.\n")


    (dt->submit_info).pCommandBuffers   = (VkCommandBuffer*)(&dt->command_buffers[*next_frame_idx]);
    (dt->submit_info).pWaitSemaphores   = &(dt->is_image_acquired[dt->frame]);  // what to wait on before execution
    (dt->submit_info).pSignalSemaphores = &(dt->is_render_complete[dt->frame]); // what to signal when execution is done
}


__UE_internal__ __UE_inline__ void
uEnsureFrameReadiness(_mut_ uVulkanDrawTools* const restrict dt
                      /* const u32*              const restrict next_frame_idx */)
{
    uAssertMsg_v(dt,                   "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(dt->logical_device,   "[ render ] VkDevice must be non zero.\n");
    uAssertMsg_v(dt->in_flight_fences, "[ render ] VkFence ptr must be non null.\n");
    /* uAssertMsg_v(next_frame_idx,      "[ render ] Next frame index ptr must be non null.\n"); */


    // Note: Move on when the fence from the command buffer for this frame has been signaled.
    //       See dt->submit_info updates.
    uDebugStatement(VkResult result = )vkWaitForFences(dt->logical_device,
                                                       1,

                                                       // This fence is signaled (complete)
                                                       // when execution resumes on this thread
                                                       &dt->in_flight_fences[dt->frame],

                                                       VK_TRUE,
                                                       uVULKAN_MAX_NANOSECOND_WAIT);
    uAssertMsg_v(result != VK_TIMEOUT, "[ render ] [ timeout ] Fence timeout on frame: %d.\n", dt->frame);


    // Reset the fence to the un-signaled (in-progress) state.
    vkResetFences(dt->logical_device, 1, &dt->in_flight_fences[dt->frame]);


    /* if (dt->swap_chain_image_fences[*next_frame_idx] != VK_NULL_HANDLE) */
    /* { */
    /*     result = vkWaitForFences(dt->logical_device, */
    /*                              1, */
    /*                              &(dt->swap_chain_image_fences[*next_frame_idx]), */
    /*                              VK_TRUE, */
    /*                              uVULKAN_MAX_NANOSECOND_WAIT); */

    /*     uAssertMsg_v(result != VK_TIMEOUT, "[ render ] [ timeout ] Fence timeout on image: %d.\n", *next_frame_idx); */
    /*     uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to ensure frame readiness.\n"); */
    /* } */
    /* dt->swap_chain_image_fences[*next_frame_idx] = dt->fences[dt->frame]; */
}


__UE_internal__ __UE_inline__ void
uAcquireNextSwapChainFrameIndex(const uVulkanDrawTools* const restrict dt,
                                _mut_ u32*              const restrict return_idx)
{
    uAssertMsg_v(dt,                 "[ render ] uVulkanDrawtools must be non zero.\n");
    uAssertMsg_v(dt->logical_device, "[ render ] VkDevice must be non zero.\n");
    uAssertMsg_v(dt->swap_chain,     "[ render ] VkSwapchainKHR must be non zero.\n");
    uAssertMsg_v(return_idx,         "[ render ] Return index ptr must be non null.\n");


    uDebugStatement(VkResult result = )vkAcquireNextImageKHR(dt->logical_device,
                                                             dt->swap_chain,
                                                             uVULKAN_MAX_NANOSECOND_WAIT,
                                                             dt->is_render_complete[dt->frame],
                                                             VK_NULL_HANDLE,
                                                             return_idx);

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
    vkAcquireNextImageKHR(dt->logical_device,
                          dt->swap_chain,
                          uVULKAN_MAX_NANOSECOND_WAIT,
                          dt->rs_image_available,
                          NULL,
                          &next_frame_idx);

    VkSemaphore wait_semaphores[1] = { 0 };
    wait_semaphores[0] = dt->rs_image_available;

    VkSemaphore signal_semaphores[1] = { 0 };
    signal_semaphores[0] = dt->rs_render_finished;

    VkPipelineStageFlags wait_stages[1] = { 0 };
    wait_stages[0] = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    // graphics queue submit info
    VkSubmitInfo submit_info = { 0 };
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &(dt->command_buffers[next_frame_idx]);
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    // submit graphics queue
    VkResult result = vkQueueSubmit(dt->graphics_queue, 1, &submit_info, NULL);
    uAssertMsg_v(result == VK_SUCCESS, "[ render ] Could not submit graphics queue.\n");

    VkSwapchainKHR swap_chains[1] = { 0 };
    swap_chains[0] = dt->swap_chain;

    // Presentation
    VkPresentInfoKHR present_info = { 0 };
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swap_chains;
    present_info.pImageIndices = &next_frame_idx;
    present_info.pResults = NULL;

    vkQueuePresentKHR(dt->present_queue, &present_info);
    vkQueueWaitIdle(dt->present_queue);

    // increment frame number
    dt->frame = (dt->frame + 1) % uVULKAN_MAX_FRAMES_IN_FLIGHT;
}


__UE_internal__ void __UE_call__
uDestroyDrawTools(_mut_ uVulkanDrawTools* const restrict draw_tools)
{
    uVulkanInfo* v_info = (uVulkanInfo*)uGetVulkanInfo();

    uAssertMsg_v(v_info,                 "[ engine ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ engine ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(draw_tools,             "[ engine ] uVulkanDrawTools ptr must be non null.\n");
    uAssertMsg_v(!RUNNING,
                 "[ engine ] Tear down called while [  __UE_global__ RUNNING == true ].\n");


    if (draw_tools && v_info && v_info->logical_device)
    {
        // Wait for device to be idle
        vkDeviceWaitIdle(v_info->logical_device);

        for (u32 sync_obj_idx = 0; sync_obj_idx < uVULKAN_MAX_FRAMES_IN_FLIGHT; sync_obj_idx++)
        {
            vkDestroySemaphore(v_info->logical_device, draw_tools->is_image_acquired[sync_obj_idx], NULL);
            vkDestroySemaphore(v_info->logical_device, draw_tools->is_render_complete[sync_obj_idx], NULL);
            vkDestroyFence(v_info->logical_device,     draw_tools->in_flight_fences[sync_obj_idx], NULL);
        }

        /* for (u8 fence_idx = 0; fence_idx < uVULKAN_NUM_COMMAND_BUFFERS; fence_idx++) */
        /* { */
        /*     vkDestroyFence(v_info->logical_device, draw_tools->swap_chain_image_fences[fence_idx], NULL); */
        /* } */
    }
}


__UE_internal__ void __UE_call__
uDestroyEngine()
{
    uAssertMsg_v(!RUNNING, "[ engine ] Tear down called while [  __UE_global__ RUNNING == true ].\n");
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

#if __UE_debug__ == 1
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
#endif // __UE_debug__ == 1

    if (argc && argv) {}

#if __UE_debug__ == 1
    printf("[ build ] DEBUG build\n");
#else
    printf("[ build ] RELEASE build.\n");
#endif

    uVulkanDrawTools draw_tools = { 0 };

    uInitializeVulkan(&draw_tools,
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                      required_instance_validation_layers,
                      sizeof(required_instance_validation_layers)/sizeof(char*),
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
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
