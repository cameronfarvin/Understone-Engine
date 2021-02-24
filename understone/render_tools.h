#ifndef __UE_RENDER_TOOLS__
#define __UE_RENDER_TOOLS__

#include "vulkan_macros.h"
#include "vulkan_types.h"
#include "window_tools.h"

// Forward declaration from vulkan_tools.h
__UE_inline__ void
uRebuildVulkanSwapChain(const VkPipelineShaderStageCreateInfo* const restrict shader_stage_create_infos,
                        const size_t                                          num_shader_stage_create_infos,
                        const uGameWindow* const restrict                     game_window);

// [ cfarvin::TODO ] Better way to get these three, also not correct style
bool     is_render_tool_outdated  = false;
const u8 kNumVulkanCommandBuffers = 3; // extern defined in vulkan_tools.h
const u8 kMaxVulkanFramesInFlight = 2; // extern defined in vulkan_tools.h
size_t   total_frame_count        = 0;

typedef struct
{
    const VkAttachmentDescription* attachment_descriptions;
    const VkAttachmentReference*   attachment_references;
    const VkPipelineLayout         pipeline_layout;
    const VkRenderPass             render_pass;
    const VkPipeline               graphics_pipeline;
    const u32                      num_attachments;
} uVulkanRenderInfo;

const uVulkanRenderInfo* uAPI_PRIME_VULKAN_RENDER_INFO = NULL;

__UE_inline__ const uVulkanRenderInfo*
                    uGetVulkanRenderInfo()
{
    if (!uAPI_PRIME_VULKAN_RENDER_INFO)
    {
        *( uVulkanRenderInfo** )&uAPI_PRIME_VULKAN_RENDER_INFO =
          ( uVulkanRenderInfo* )calloc(1, sizeof(uVulkanRenderInfo));
    }

    return uAPI_PRIME_VULKAN_RENDER_INFO;
}

typedef struct
{
    VkSemaphore          image_available[kMaxVulkanFramesInFlight];
    VkSemaphore          render_finished[kMaxVulkanFramesInFlight];
    VkFence              in_flight_fences[kMaxVulkanFramesInFlight];
    VkFence              in_flight_images[kNumVulkanCommandBuffers];
    VkCommandBuffer      command_buffers[kNumVulkanCommandBuffers];
    VkPipelineStageFlags stage_flags[1]; // Note: act as a sync primitive
    VkDevice             logical_device;
    VkSwapchainKHR       swap_chain;
    VkSubmitInfo         submit_info;
    VkQueue              graphics_queue;
    VkQueue              present_queue;
    VkPresentInfoKHR     present_info;
    u32                  frame;

    // [ cfarvin::REVISIT ]
    VkPipelineShaderStageCreateInfo* shader_stage_create_infos;
    u8                               num_shaders;
} uVulkanRenderTools;

__UE_inline__ void
uCreateRenderTools(uVulkanRenderTools* const restrict                    render_tools,
                   const VkPipelineShaderStageCreateInfo* const restrict shader_stage_create_infos,
                   const size_t                                          num_shaders)
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanQueueInfo*   queue_info   = ( uVulkanQueueInfo* )uGetVulkanQueueInfo();
    uVulkanImageGroup*  image_group  = ( uVulkanImageGroup* )uGetVulkanImageGroup();
    uVulkanCommandInfo* command_info = ( uVulkanCommandInfo* )uGetVulkanCommandInfo();

    uAssertMsg_v(v_info, "[ render ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(queue_info, "[ render ] uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(image_group, "[ render ] uVulkanImageGroup ptr must be non null.\n");
    uAssertMsg_v(command_info, "[ render ] uVulkanCommandInfo ptr must be non null.\n");
    uAssertMsg_v(render_tools, "[ render ] uVulkanRenderTools ptr must be non null\n.");
    uAssertMsg_v(shader_stage_create_infos, "[ render ] VkPipelineShaderStageCreateInfo ptr must be non null\n.");
    uAssertMsg_v(num_shaders, "[ render ] Num shaders must be non zero.\n");

    render_tools->shader_stage_create_infos = ( VkPipelineShaderStageCreateInfo* )shader_stage_create_infos;
    render_tools->num_shaders               = num_shaders;

    for (u8 frame_idx = 0; frame_idx < 1; frame_idx++)
    {
        render_tools->stage_flags[frame_idx] = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }

    render_tools->logical_device = v_info->logical_device;
    render_tools->swap_chain     = image_group->swap_chain;
    render_tools->graphics_queue = queue_info->queues[uVULKAN_GRAPHICS_QUEUE_INDEX];
    render_tools->present_queue  = queue_info->queues[uVULKAN_PRESENT_QUEUE_INDEX];

    VkSubmitInfo submit_info         = {};
    submit_info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount   = 1;
    submit_info.pWaitDstStageMask    = render_tools->stage_flags;
    submit_info.commandBufferCount   = 1;
    submit_info.signalSemaphoreCount = 1;

    render_tools->submit_info = submit_info;
    memcpy(&(render_tools->command_buffers[0]),
           command_info->command_buffers,
           kNumVulkanCommandBuffers * sizeof(VkCommandBuffer));

    VkPresentInfoKHR present_info   = {};
    present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.swapchainCount     = 1;
    present_info.pSwapchains        = &(render_tools->swap_chain);
    present_info.pResults           = NULL;

    render_tools->present_info = present_info;

    VkSemaphoreCreateInfo semaphore_create_info = {};
    semaphore_create_info.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_create_info = {};
    fence_create_info.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.flags             = VK_FENCE_CREATE_SIGNALED_BIT; // Done state

#if _WIN32
#pragma warning(push)
#pragma warning(disable : 4127)
#endif // _WIN32
    uAssertMsg_v((kMaxVulkanFramesInFlight == (kNumVulkanCommandBuffers - 1)),
                 "[ api ] Max in-flight frames must be exactly one less than number "
                 "command buffers.\n");
#if _WIN32
#pragma warning(pop)
#endif // _WIN32

    VkResult   result                  = VK_SUCCESS;
    const char sync_create_error_msg[] = "[ render ] Unable to create render syn objects.\n";

    for (u8 sync_obj_idx = 0; sync_obj_idx < kMaxVulkanFramesInFlight; sync_obj_idx++)
    {
        // Create image acquired semaphores
        result = vkCreateSemaphore(v_info->logical_device,
                                   &semaphore_create_info,
                                   NULL,
                                   &(render_tools->image_available[sync_obj_idx]));

        if (result != VK_SUCCESS)
        {
            uFatal(sync_create_error_msg);
        }

        // Create present ready semaphores
        result = vkCreateSemaphore(v_info->logical_device,
                                   &semaphore_create_info,
                                   NULL,
                                   &(render_tools->render_finished[sync_obj_idx]));

        if (result != VK_SUCCESS)
        {
            uFatal(sync_create_error_msg);
        }

        // Create in-flight fences
        result = vkCreateFence(v_info->logical_device,
                               &fence_create_info,
                               NULL,
                               &(render_tools->in_flight_fences[sync_obj_idx]));

        if (result != VK_SUCCESS)
        {
            uFatal(sync_create_error_msg);
        }
    }

    for (u8 cmd_buff_idx = 0; cmd_buff_idx < kNumVulkanCommandBuffers; cmd_buff_idx++)
    {
        render_tools->in_flight_images[cmd_buff_idx] = VK_NULL_HANDLE;
    }
}

// Note: Does not free or memset uVulkanRenderInfo pointer, but does free it's applicable members.
__UE_inline__ void
uClearVulkanRenderInfo()
{
    uVulkanInfo*       v_info      = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanRenderInfo* render_info = ( uVulkanRenderInfo* )uGetVulkanRenderInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(render_info, "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");

    // Clear pipeline layout, render pass, graphics pipeline, frame buffer(s) so they can be rebuilt.
    if (render_info)
    {
        if (v_info->logical_device)
        {
            vkDestroyPipeline(v_info->logical_device, render_info->graphics_pipeline, NULL);
            vkDestroyPipelineLayout(v_info->logical_device, render_info->pipeline_layout, NULL);
            vkDestroyRenderPass(v_info->logical_device, render_info->render_pass, NULL);
        }

        if (render_info->attachment_descriptions)
        {
            free(( VkAttachmentDescription* )render_info->attachment_descriptions);
        }

        if (render_info->attachment_references)
        {
            free(( VkAttachmentReference* )render_info->attachment_references);
        }
    }
}

// Note: Completely destroys and frees the uVulkanRenderInfo pointer and it's applicable members.
__UE_inline__ void
uDestroyVulkanRenderInfo()
{
    uVulkanRenderInfo* render_info = ( uVulkanRenderInfo* )uGetVulkanRenderInfo();
    uAssertMsg_v(render_info, "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");

    uClearVulkanRenderInfo();

    if (render_info)
    {
        free(render_info);
    }
}

// Note: Frees the uVulkanRenderInfo pointer's applicable members and memsets the top-level handle.
__UE_inline__ void
uResetVulkanRenderInfo()
{
    uVulkanRenderInfo* render_info = ( uVulkanRenderInfo* )uGetVulkanRenderInfo();
    uAssertMsg_v(render_info, "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");

    uClearVulkanRenderInfo();

    if (render_info)
    {
        memset(render_info, 0, sizeof(uVulkanRenderInfo));
    }
}

__UE_inline__ void
uDestroyRenderToolSyncMembers(uVulkanRenderTools* const restrict render_tools)
{
    uVulkanInfo* v_info = ( uVulkanInfo* )uGetVulkanInfo();

    uAssertMsg_v(v_info, "[ render ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ render ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(render_tools, "[ render ] uVulkanRenderTools ptr must be non null.\n");

    if (render_tools && v_info && v_info->logical_device)
    {
        // Wait for device to be idle
        vkDeviceWaitIdle(v_info->logical_device);

        for (u32 sync_obj_idx = 0; sync_obj_idx < kMaxVulkanFramesInFlight; sync_obj_idx++)
        {
            vkDestroySemaphore(v_info->logical_device, render_tools->image_available[sync_obj_idx], NULL);
            vkDestroySemaphore(v_info->logical_device, render_tools->render_finished[sync_obj_idx], NULL);
            vkDestroyFence(v_info->logical_device, render_tools->in_flight_fences[sync_obj_idx], NULL);
        }
    }
}

__UE_inline__ void
uRebuidlRenderTools(uVulkanRenderTools* const restrict                    render_tools,
                    const VkPipelineShaderStageCreateInfo* const restrict shader_stage_create_infos,
                    const size_t                                          num_shaders)
{
    uAssertMsg_v(render_tools, "[ render ] uVulkanRenderTools ptr must be non null.\n");
    uAssertMsg_v(shader_stage_create_infos, "[ render ] VkPipelineShaderStageCreateInfo ptr must be non null\n.");

    uDestroyRenderToolSyncMembers(render_tools);
    uCreateRenderTools(render_tools, shader_stage_create_infos, num_shaders);

    is_render_tool_outdated = false;
}

// Returns true if outdated or suboptimal, otherwise false.
__UE_inline__ bool
uCheckOutdatedOrSuboptimalSwapChain(uVulkanRenderTools* const restrict render_tools, const VkResult result)
{
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        return true;
    }

    return false;
}

__UE_inline__ VkResult
uUpdatePresentInfoAndPresent(uVulkanRenderTools* const restrict render_tools, const u32* const restrict next_frame_idx)
{
    uAssertMsg_v(render_tools, "[ render ] uVulkanRendertools must be non zero.\n");
    uAssertMsg_v(next_frame_idx, "[ render ] Next frame index ptr must be non null.\n");
    uAssertMsg_v(render_tools->shader_stage_create_infos,
                 "[ render ] VkPipelineShaderStageCreateInfo ptr must be non null.\n");
    uAssertMsg_v(render_tools->num_shaders, "[ render ] Number of shaders must be non zero.\n");

    (render_tools->present_info).pImageIndices   = ( u32* )next_frame_idx;
    (render_tools->present_info).pWaitSemaphores = &(render_tools->render_finished[render_tools->frame]);

    VkResult result = vkQueuePresentKHR(render_tools->present_queue, &(render_tools->present_info));
    uCheckOutdatedOrSuboptimalSwapChain(render_tools, result);

    uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to present.\n");

    return result;
}

__UE_inline__ void
uUpdateGraphicsInfoAndSubmit(uVulkanRenderTools* const restrict render_tools, const u32* const restrict next_frame_idx)
{
    uAssertMsg_v(render_tools, "[ render ] uVulkanRendertools must be non zero.\n");
    uAssertMsg_v(render_tools->graphics_queue, "[ render ] VkQueue (graphics) must be non zero.\n");
    uAssertMsg_v(render_tools->in_flight_fences, "[ render ] VkFence ptr must be non null.\n");
    uAssertMsg_v(next_frame_idx, "[ render ] Next frame index ptr must be non null.\n");
    uAssertMsg_v(*next_frame_idx <= kNumVulkanCommandBuffers,
                 "[ render ] Next frame index value exceeds command buffer length.\n");

    (render_tools->submit_info).pCommandBuffers = ( VkCommandBuffer* )(&render_tools->command_buffers[*next_frame_idx]);
    (render_tools->submit_info).pWaitSemaphores =
      &(render_tools->image_available[render_tools->frame]); // what to wait on before execution
    (render_tools->submit_info).pSignalSemaphores =
      &(render_tools->render_finished[render_tools->frame]); // what to signal when execution is done

    uDebugStatement(VkResult result =) vkQueueSubmit(render_tools->graphics_queue,
                                                     1,
                                                     &(render_tools->submit_info),
                                                     render_tools->in_flight_fences[render_tools->frame]);
    uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to submit graphics queue.\n");
}

__UE_inline__ void
uEnsureFrameLanded(uVulkanRenderTools* const restrict render_tools, const u32* const restrict next_frame_idx)
{
    uAssertMsg_v(render_tools, "[ render ] uVulkanRendertools must be non zero.\n");
    uAssertMsg_v(render_tools->logical_device, "[ render ] VkDevice must be non zero.\n");
    uAssertMsg_v(render_tools->in_flight_fences, "[ render ] VkFence ptr must be non null.\n");
    uAssertMsg_v(next_frame_idx, "[ render ] Next frame index ptr must be non null.\n");

    if (render_tools->in_flight_images[*next_frame_idx] != VK_NULL_HANDLE)
    {
        uDebugStatement(VkResult result =) vkWaitForFences(render_tools->logical_device,
                                                           1,
                                                           &(render_tools->in_flight_images[*next_frame_idx]),
                                                           VK_TRUE,
                                                           uVULKAN_MAX_NANOSECOND_WAIT);

        uAssertMsg_v(result != VK_TIMEOUT, "[ render ] [ timeout ] Fence timeout on image: %d.\n", *next_frame_idx);
        uAssertMsg_v(result == VK_SUCCESS, "[ render ] Unable to ensure frame readiness.\n");
    }

    render_tools->in_flight_images[*next_frame_idx] = render_tools->in_flight_fences[render_tools->frame];

    vkResetFences(render_tools->logical_device, 1, &(render_tools->in_flight_fences[render_tools->frame]));
}

__UE_inline__ VkResult
uAcquireNextSwapChainFrameIndex(uVulkanRenderTools* const restrict render_tools, u32* const restrict return_idx)
{
    uAssertMsg_v(render_tools, "[ render ] uVulkanRendertools must be non zero.\n");
    uAssertMsg_v(render_tools->logical_device, "[ render ] VkDevice must be non zero.\n");
    uAssertMsg_v(render_tools->swap_chain, "[ render ] VkSwapchainKHR must be non zero.\n");
    uAssertMsg_v(return_idx, "[ render ] Return index ptr must be non null.\n");
    uAssertMsg_v(render_tools->shader_stage_create_infos,
                 "[ render ] VkPipelineShaderStageCreateInfo ptr  must be non null.\n");
    uAssertMsg_v(render_tools->num_shaders, "[ render ] Num shaders must be non zero.\n");

    VkResult result = vkAcquireNextImageKHR(render_tools->logical_device,
                                            render_tools->swap_chain,
                                            uVULKAN_MAX_NANOSECOND_WAIT,
                                            render_tools->image_available[render_tools->frame],
                                            NULL,
                                            return_idx);

    uAssertMsg_v(result != VK_TIMEOUT, "[ render ] [ timeout ] Could not acquire next swap chain image.\n");
    uAssertMsg_v(result == VK_SUCCESS, "[ render ] Could not acquire next swap chain image.\n");
    uAssertMsg_v((*return_idx <= ( u32 )kMaxVulkanFramesInFlight),
                 "[ redner ] Acquired swap chain image index greater than "
                 "frame count.\n");

    return result;
}

__UE_inline__ void
uRenderFrame(const uGameWindow* const restrict game_window, uVulkanRenderTools* const restrict render_tools)
{
    // [ cfarvin::TODO ] Move this to calling code; remove game_window param
    uAssertMsg_v(!game_window->is_minimized, "[ vkTrinagleExample ] Cannot render frame with window minimized\n.")
      uAssertMsg_v(render_tools, "[ render ] uVulkanRendertools must be non zero.\n");

    vkWaitForFences(render_tools->logical_device,
                    1,
                    &(render_tools->in_flight_fences[render_tools->frame]),
                    VK_TRUE,
                    uVULKAN_MAX_NANOSECOND_WAIT);

    VkResult result;
    u32      next_frame_idx = 0;

    result = uAcquireNextSwapChainFrameIndex(render_tools, &next_frame_idx);

    if (uCheckOutdatedOrSuboptimalSwapChain(render_tools, result))
    {
        is_render_tool_outdated = true;
        uRebuildVulkanSwapChain(render_tools->shader_stage_create_infos, render_tools->num_shaders, game_window);
    }

    uEnsureFrameLanded(render_tools, &next_frame_idx);
    uUpdateGraphicsInfoAndSubmit(render_tools, &next_frame_idx);
    result = uUpdatePresentInfoAndPresent(render_tools, &next_frame_idx);

    if (uCheckOutdatedOrSuboptimalSwapChain(render_tools, result))
    {
        is_render_tool_outdated = true;
        uRebuildVulkanSwapChain(render_tools->shader_stage_create_infos, render_tools->num_shaders, game_window);
    }

    // Increment frame number.
    render_tools->frame = (render_tools->frame + 1) % kMaxVulkanFramesInFlight;
    total_frame_count++;
}

#endif // __UE_RENDER_TOOLS__
