#ifndef __UE_VULKAN_TYPES_H__
#define __UE_VULKAN_TYPES_H__

#include <vulkan/vulkan.h>

#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
VkDebugUtilsMessengerEXT           vulkan_main_debug_messenger;
VkDebugUtilsMessengerCreateInfoEXT vulkan_main_debug_messenger_info  = {};
VkDebugUtilsMessengerCreateInfoEXT vulkan_setup_debug_messenger_info = {};
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

extern bool kRunning;

//
// [ begin ] Prime uVulkanCommandInfo
typedef struct
{
    const VkCommandBuffer* command_buffers;
    const VkCommandPool    command_pool;
    const u32              num_command_buffers;
} uVulkanCommandInfo;
const uVulkanCommandInfo* uAPI_PRIME_VULKAN_COMMAND_INFO = NULL;
__UE_inline__ static const uVulkanCommandInfo*
uGetVulkanCommandInfo()
{
    if (!uAPI_PRIME_VULKAN_COMMAND_INFO) { *( uVulkanCommandInfo** )&uAPI_PRIME_VULKAN_COMMAND_INFO = ( uVulkanCommandInfo* )calloc(1, sizeof(uVulkanCommandInfo)); }

    return uAPI_PRIME_VULKAN_COMMAND_INFO;
}
// [ end ] Prime uVulkanCommandInfo
//

//
// [ begin ] Prime uVulkanSurfaceInfo
typedef struct
{
    const VkSurfaceFormatKHR*      surface_formats;
    const VkPresentModeKHR*        present_modes;
    const VkSurfaceCapabilitiesKHR surface_capabilities;
    const VkExtent2D               surface_extent;
    const VkSurfaceKHR             surface;
    const u32                      num_surface_formats;
    const u32                      designated_format_index;
    const u32                      designated_present_index;
    const u32                      num_present_modes;
} uVulkanSurfaceInfo;
const uVulkanSurfaceInfo* uAPI_PRIME_VULKAN_SURFACE_INFO = NULL;
__UE_inline__ static const uVulkanSurfaceInfo*
uGetVulkanSurfaceInfo()
{
    if (!uAPI_PRIME_VULKAN_SURFACE_INFO) { *( uVulkanSurfaceInfo** )&uAPI_PRIME_VULKAN_SURFACE_INFO = ( uVulkanSurfaceInfo* )calloc(1, sizeof(uVulkanSurfaceInfo)); }

    return uAPI_PRIME_VULKAN_SURFACE_INFO;
}
// [ end ] Prime uVulkanSurfaceInfo
//

//
// [ begin ] Prime uVulkanQueueInfo
// Note: as queue indices are added, ensure that unique value extraction
//       in uCreateVulkanLogicalDevice() is updated as well.
#define uVULKAN_NUM_QUEUES           2
#define uVULKAN_GRAPHICS_QUEUE_INDEX 0
#define uVULKAN_PRESENT_QUEUE_INDEX  1
typedef struct
{
    const VkQueue queues[uVULKAN_NUM_QUEUES];
    const u32     designated_graphics_index;
    const u32     designated_present_index;
} uVulkanQueueInfo;
const uVulkanQueueInfo* uAPI_PRIME_VULKAN_QUEUE_INFO = NULL;
__UE_inline__ static const uVulkanQueueInfo*
uGetVulkanQueueInfo()
{
    if (!uAPI_PRIME_VULKAN_QUEUE_INFO) { *( uVulkanQueueInfo** )&uAPI_PRIME_VULKAN_QUEUE_INFO = ( uVulkanQueueInfo* )calloc(1, sizeof(uVulkanQueueInfo)); }

    return uAPI_PRIME_VULKAN_QUEUE_INFO;
}
// [ end ] Prime uVulkanQueueInfo
//

//
// [ begin ] Prime uVulkanInfo
typedef struct
{
    const VkInstance       instance;
    const VkPhysicalDevice physical_device;
    const VkDevice         logical_device;
} uVulkanInfo;
const uVulkanInfo* uAPI_PRIME_VULKAN_INFO = NULL;
__UE_inline__ static const uVulkanInfo*
uGetVulkanInfo()
{
    if (!uAPI_PRIME_VULKAN_INFO) { *( uVulkanInfo** )&uAPI_PRIME_VULKAN_INFO = ( uVulkanInfo* )calloc(1, sizeof(uVulkanInfo)); }

    return uAPI_PRIME_VULKAN_INFO;
}
// [ end ] Prime uVulkanInfo
//

//
// [ begin ] Prime uVulkanImageGroup
typedef struct
{
    VkImage*             images;
    VkImageView*         image_views;   // Note: To have num_images elements
    VkFramebuffer*       frame_buffers; // Note: To have num_images elements
    const u32            num_images;
    const VkSwapchainKHR swap_chain;
} uVulkanImageGroup;
const uVulkanImageGroup* uAPI_PRIME_VULKAN_IMAGE_GROUP = NULL;
__UE_inline__ static const uVulkanImageGroup*
uGetVulkanImageGroup()
{
    if (!uAPI_PRIME_VULKAN_IMAGE_GROUP) { *( uVulkanImageGroup** )&uAPI_PRIME_VULKAN_IMAGE_GROUP = ( uVulkanImageGroup* )calloc(1, sizeof(uVulkanImageGroup)); }

    return uAPI_PRIME_VULKAN_IMAGE_GROUP;
}
// [ end ] Prime uVulkanImageGroup
//

//
// [ begin ] Prime uVulkanRenderInfo
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
__UE_inline__ static const uVulkanRenderInfo*
uGetVulkanRenderInfo()
{
    if (!uAPI_PRIME_VULKAN_RENDER_INFO) { *( uVulkanRenderInfo** )&uAPI_PRIME_VULKAN_RENDER_INFO = ( uVulkanRenderInfo* )calloc(1, sizeof(uVulkanRenderInfo)); }

    return uAPI_PRIME_VULKAN_RENDER_INFO;
}
// [ end ] Prime uVulkanRenderInfo
//

//
// [ begin ] Draw Tools
bool kVulkanDrawToolsOutdated = false;
#define uVULKAN_NUM_COMMAND_BUFFERS  3
#define uVULKAN_MAX_FRAMES_IN_FLIGHT 2
typedef struct
{
    VkSemaphore          image_available[uVULKAN_MAX_FRAMES_IN_FLIGHT];
    VkSemaphore          render_finished[uVULKAN_MAX_FRAMES_IN_FLIGHT];
    VkFence              in_flight_fences[uVULKAN_MAX_FRAMES_IN_FLIGHT];
    VkFence              in_flight_images[uVULKAN_NUM_COMMAND_BUFFERS];
    VkCommandBuffer      command_buffers[uVULKAN_NUM_COMMAND_BUFFERS];
    VkPipelineStageFlags stage_flags[1]; // Note: act as a sync primitive
    VkDevice             logical_device;
    VkSwapchainKHR       swap_chain;
    VkSubmitInfo         submit_info;
    VkQueue              graphics_queue;
    VkQueue              present_queue;
    VkPresentInfoKHR     present_info;
    u32                  frame;
} uVulkanDrawTools;

__UE_inline__ static void
uCreateDrawTools(uVulkanDrawTools* const restrict draw_tools)
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanQueueInfo*   queue_info   = ( uVulkanQueueInfo* )uGetVulkanQueueInfo();
    uVulkanImageGroup*  image_group  = ( uVulkanImageGroup* )uGetVulkanImageGroup();
    uVulkanCommandInfo* command_info = ( uVulkanCommandInfo* )uGetVulkanCommandInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(queue_info, "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(image_group, "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");
    uAssertMsg_v(command_info, "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");

    for (u8 frame_idx = 0; frame_idx < 1; frame_idx++) { draw_tools->stage_flags[frame_idx] = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; }

    draw_tools->logical_device = v_info->logical_device;
    draw_tools->swap_chain     = image_group->swap_chain;
    draw_tools->graphics_queue = queue_info->queues[uVULKAN_GRAPHICS_QUEUE_INDEX];
    draw_tools->present_queue  = queue_info->queues[uVULKAN_PRESENT_QUEUE_INDEX];

    VkSubmitInfo submit_info         = {};
    submit_info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount   = 1;
    submit_info.pWaitDstStageMask    = draw_tools->stage_flags;
    submit_info.commandBufferCount   = 1;
    submit_info.signalSemaphoreCount = 1;

    draw_tools->submit_info = submit_info;
    memcpy(&(draw_tools->command_buffers[0]), command_info->command_buffers, uVULKAN_NUM_COMMAND_BUFFERS * sizeof(VkCommandBuffer));

    VkPresentInfoKHR present_info   = {};
    present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.swapchainCount     = 1;
    present_info.pSwapchains        = &(draw_tools->swap_chain);
    present_info.pResults           = NULL;

    draw_tools->present_info = present_info;

    VkSemaphoreCreateInfo semaphore_create_info = {};
    semaphore_create_info.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_create_info = {};
    fence_create_info.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.flags             = VK_FENCE_CREATE_SIGNALED_BIT; // Done state

#if _WIN32
#pragma warning(push)
#pragma warning(disable : 4127)
#endif // _WIN32
    uAssertMsg_v((uVULKAN_MAX_FRAMES_IN_FLIGHT == (uVULKAN_NUM_COMMAND_BUFFERS - 1)),
                 "[ api ] Max in-flight frames must be exactly one less than number "
                 "command buffers.\n");
#if _WIN32
#pragma warning(pop)
#endif // _WIN32

    VkResult   result                  = VK_SUCCESS;
    const char sync_create_error_msg[] = "[ vulkan ] Unable to create draw syn objects.\n";

    for (u8 sync_obj_idx = 0; sync_obj_idx < uVULKAN_MAX_FRAMES_IN_FLIGHT; sync_obj_idx++)
    {
        // Create image acquired semaphores
        result = vkCreateSemaphore(v_info->logical_device, &semaphore_create_info, NULL, &(draw_tools->image_available[sync_obj_idx]));

        if (result != VK_SUCCESS)
        {
            kRunning = false;
            uFatal(sync_create_error_msg);
        }

        // Create present ready semaphores
        result = vkCreateSemaphore(v_info->logical_device, &semaphore_create_info, NULL, &(draw_tools->render_finished[sync_obj_idx]));

        if (result != VK_SUCCESS)
        {
            kRunning = false;
            uFatal(sync_create_error_msg);
        }

        // Create in-flight fences
        result = vkCreateFence(v_info->logical_device, &fence_create_info, NULL, &(draw_tools->in_flight_fences[sync_obj_idx]));

        if (result != VK_SUCCESS)
        {
            kRunning = false;
            uFatal(sync_create_error_msg);
        }
    }

    for (u8 cmd_buff_idx = 0; cmd_buff_idx < uVULKAN_NUM_COMMAND_BUFFERS; cmd_buff_idx++) { draw_tools->in_flight_images[cmd_buff_idx] = VK_NULL_HANDLE; }
}

__UE_inline__ static void
uDestroyDrawTools(uVulkanDrawTools* const restrict draw_tools, bool is_rebuilding_swap_chain)
{
    uVulkanInfo* v_info = ( uVulkanInfo* )uGetVulkanInfo();

    if (is_rebuilding_swap_chain) {} // Silence release build warnings
    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(draw_tools, "[ vulkan ] uVulkanDrawTools ptr must be non null.\n");
    uAssertMsg_v(is_rebuilding_swap_chain || !kRunning, "[ vulkan ] Tear down called while ` kRunning == true`.\n");

    if (draw_tools && v_info && v_info->logical_device)
    {
        // Wait for device to be idle
        vkDeviceWaitIdle(v_info->logical_device);

        for (u32 sync_obj_idx = 0; sync_obj_idx < uVULKAN_MAX_FRAMES_IN_FLIGHT; sync_obj_idx++)
        {
            vkDestroySemaphore(v_info->logical_device, draw_tools->image_available[sync_obj_idx], NULL);
            vkDestroySemaphore(v_info->logical_device, draw_tools->render_finished[sync_obj_idx], NULL);
            vkDestroyFence(v_info->logical_device, draw_tools->in_flight_fences[sync_obj_idx], NULL);
        }
    }
}

__UE_inline__ void
uRebuidlDrawTools(uVulkanDrawTools* const restrict draw_tools)
{
    uAssertMsg_v(draw_tools, "[ vulkan ] uVulkanDrawTools ptr must be non null.\n");

    uDestroyDrawTools(draw_tools, true);
    uCreateDrawTools(draw_tools);

    kVulkanDrawToolsOutdated = false;
}
// [ end ] Draw Tools

__UE_inline__ void
uDestroyVulkanRenderInfo()
{
    uVulkanInfo*       v_info      = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanRenderInfo* render_info = ( uVulkanRenderInfo* )uGetVulkanRenderInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(render_info, "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");

    // Destroy pipeline, pipeline layout, renderpass;
    uVkVerbose("Destroying pipeline components...\n");
    if (render_info)
    {
        if (v_info->logical_device)
        {
            // Destroy pipeline layout, render pass, graphics pipeline, frame
            // buffer(s)
            vkDestroyPipeline(v_info->logical_device, render_info->graphics_pipeline, NULL);
            vkDestroyPipelineLayout(v_info->logical_device, render_info->pipeline_layout, NULL);
            vkDestroyRenderPass(v_info->logical_device, render_info->render_pass, NULL);
        }

        if (render_info->attachment_descriptions) { free(( VkAttachmentDescription* )render_info->attachment_descriptions); }

        if (render_info->attachment_references) { free(( VkAttachmentReference* )render_info->attachment_references); }

        if (kRunning) { memset(render_info, 0, sizeof(uVulkanRenderInfo)); }
        else
        {
            free(render_info);
        }
    }
}

__UE_inline__ void
uDestroyVulkanQueueInfo()
{
    uVulkanQueueInfo* queue_info = ( uVulkanQueueInfo* )uGetVulkanQueueInfo();
    uAssertMsg_v(queue_info, "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");

    uVkVerbose("Destroying queue components...\n");

    if (queue_info)
    {
        if (kRunning) { memset(queue_info, 0, sizeof(uVulkanQueueInfo)); }
        else
        {
            free(queue_info);
        }
    }
}

__UE_inline__ static void
uDestroyVulkanCommandBuffers()
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanCommandInfo* command_info = ( uVulkanCommandInfo* )uGetVulkanCommandInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(command_info, "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");

    uVkVerbose("Destroying command buffers...\n");
    if (command_info)
    {
        if (v_info) { vkFreeCommandBuffers(v_info->logical_device, command_info->command_pool, command_info->num_command_buffers, command_info->command_buffers); }

        if (command_info->command_buffers) { free(( void* )command_info->command_buffers); }
    }
}

__UE_inline__ void
uDestroyVulkanCommandInfo()
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanCommandInfo* command_info = ( uVulkanCommandInfo* )uGetVulkanCommandInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(command_info, "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");

    uVkVerbose("Destroying command components...\n");
    if (command_info)
    {
        uDestroyVulkanCommandBuffers();

        if (v_info) { vkDestroyCommandPool(v_info->logical_device, command_info->command_pool, NULL); }

        if (kRunning) { memset(command_info, 0, sizeof(uVulkanCommandInfo)); }
        else
        {
            free(command_info);
        }
    }
}

__UE_inline__ void
uDestroyVulkanSurfaceInfo()
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");

    uVkVerbose("Destroying surface components...\n");
    if (surface_info)
    {
        if (v_info->instance) { vkDestroySurfaceKHR(v_info->instance, surface_info->surface, NULL); }

        if (surface_info->surface_formats) { free(( VkSurfaceFormatKHR* )surface_info->surface_formats); }

        if (surface_info->present_modes) { free(( VkPresentModeKHR* )surface_info->present_modes); }

        if (kRunning) { memset(surface_info, 0, sizeof(uVulkanSurfaceInfo)); }
        else if (surface_info)
        {
            free(surface_info);
        }
    }
}

__UE_inline__ void
uDestroyVulkanInfo()
{
    uVulkanInfo* v_info = ( uVulkanInfo* )uGetVulkanInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] VkInstance must be non zero.\n");

    uVkVerbose("Destroying primary vulkan components...\n");
    if (v_info)
    {
        if (v_info->instance)
        {
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
            // Destroy debug messenger
            PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT =
              ( PFN_vkDestroyDebugUtilsMessengerEXT )vkGetInstanceProcAddr(v_info->instance, "vkDestroyDebugUtilsMessengerEXT");
            uAssertMsg_v(vkDestroyDebugUtilsMessengerEXT,
                         "[ vulkan ] Unable to acquire fnptr: "
                         "vkDestroyDebugUtilsMessengerEXT().\n");
            if (vkDestroyDebugUtilsMessengerEXT && vulkan_main_debug_messenger) { vkDestroyDebugUtilsMessengerEXT(v_info->instance, vulkan_main_debug_messenger, NULL); }
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
        }

        if (v_info->logical_device) { vkDestroyDevice(v_info->logical_device, NULL); }

        if (v_info->instance) { vkDestroyInstance(v_info->instance, NULL); }
    }

    if (kRunning) { memset(v_info, 0, sizeof(uVulkanInfo)); }
    else if (v_info)
    {
        free(v_info);
    }
}

__UE_inline__ void
uDestroyVulkanImageGroup()
{
    uVulkanInfo*       v_info      = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanImageGroup* image_group = ( uVulkanImageGroup* )uGetVulkanImageGroup();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(image_group, "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");

    uVkVerbose("Destroying image group components...\n");
    if (image_group)
    {
        if (v_info && v_info->logical_device)
        {
            // Destroy image views, frame buffers
            for (u32 image_idx = 0; image_idx < image_group->num_images; image_idx++)
            {
                if (image_group->image_views) { vkDestroyImageView(v_info->logical_device, image_group->image_views[image_idx], NULL); }

                if (image_group->frame_buffers) { vkDestroyFramebuffer(v_info->logical_device, image_group->frame_buffers[image_idx], NULL); }
            }
        }

        if (image_group->swap_chain)
        {
            uVkVerbose("Destroying old swap chain...\n");
            vkDestroySwapchainKHR(v_info->logical_device, image_group->swap_chain, NULL);
        }

        if (image_group->images) { free(image_group->images); }

        if (image_group->image_views) { free(image_group->image_views); }

        if (image_group->frame_buffers) { free(image_group->frame_buffers); }
    }

    if (kRunning) { memset(image_group, 0, sizeof(uVulkanImageGroup)); }
    else if (image_group)
    {
        free(image_group);
    }
}

#endif // __UE_VULKAN_TYPES_H__
