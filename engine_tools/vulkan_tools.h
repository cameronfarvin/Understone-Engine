#ifndef __UE_VULKAN_TOOLS_H__
#define __UE_VULKAN_TOOLS_H__

#ifndef __UE_VK_VERBOSE__
#define __UE_VK_VERBOSE__ 1
#endif // __UE_VK_VERBOSE__

#if __UE_VK_VERBOSE__
#define MAX_VKVERBOSE_LEN 256
char _vkverbose_buffer[MAX_VKVERBOSE_LEN];
char _vkMessage_buffer[MAX_VKVERBOSE_LEN];
#define uVkVerbose(...)                                                 \
    snprintf(_vkMessage_buffer, MAX_VKVERBOSE_LEN, __VA_ARGS__);        \
    snprintf(_vkverbose_buffer,                                         \
             MAX_VKVERBOSE_LEN,                                         \
             "[ vulkan ] %s",                                           \
             _vkMessage_buffer);                                        \
    fputs(_vkverbose_buffer, stdout);                                   \
    fflush(stdout);
#else
#define uVkVerbose(...) /* uVKVerbose() REMOVED */
#endif // __UE_VK_VERBOSE__


#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <win/win_platform.h>

const uWin32Info* win32_info = NULL;
#endif // _WIN32

#include <vulkan/vulkan.h>
#include <engine_tools/memory_tools.h>
#include <engine_tools/window_tools.h>
#include <data_structures/data_structures.h>

// [ cfarvin::REMOVE ] Remove stdio.h
#include <stdio.h>


//
// [ begin ] Prime uVulkanCommandInfo
typedef struct
{
    const VkCommandBuffer* command_buffers;
    const VkCommandPool    command_pool;
    const u32              num_command_buffers;
} uVulkanCommandInfo;
__UE_singleton__ uVulkanCommandInfo* uAPI_PRIME_VULKAN_COMMAND_INFO = NULL;
__UE_internal__ __UE_inline__ const uVulkanCommandInfo* const
uGetVulkanCommandInfo()
{
    if (!uAPI_PRIME_VULKAN_COMMAND_INFO)
    {
        *(uVulkanCommandInfo**)&uAPI_PRIME_VULKAN_COMMAND_INFO =
            calloc(1, sizeof(uVulkanCommandInfo));
    }

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
__UE_singleton__ uVulkanSurfaceInfo* uAPI_PRIME_VULKAN_SURFACE_INFO = NULL;
__UE_internal__ __UE_inline__ const uVulkanSurfaceInfo* const
uGetVulkanSurfaceInfo()
{
    if (!uAPI_PRIME_VULKAN_SURFACE_INFO)
    {
        *(uVulkanSurfaceInfo**)&uAPI_PRIME_VULKAN_SURFACE_INFO =
            calloc(1, sizeof(uVulkanSurfaceInfo));
    }

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
    const VkQueue          queues[uVULKAN_NUM_QUEUES];
    const u32              designated_graphics_index;
    const u32              designated_present_index;
} uVulkanQueueInfo;
__UE_singleton__ uVulkanQueueInfo* uAPI_PRIME_VULKAN_QUEUE_INFO = NULL;
__UE_internal__ __UE_inline__ const uVulkanQueueInfo* const
uGetVulkanQueueInfo()
{
    if (!uAPI_PRIME_VULKAN_QUEUE_INFO)
    {
        *(uVulkanQueueInfo**)&uAPI_PRIME_VULKAN_QUEUE_INFO =
            (uVulkanQueueInfo*)calloc(1, sizeof(uVulkanQueueInfo));
    }

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
    const VkSwapchainKHR   swap_chain;
} uVulkanInfo;
__UE_singleton__ uVulkanInfo* uAPI_PRIME_VULKAN_INFO = NULL;
__UE_internal__ __UE_inline__ const uVulkanInfo* const
uGetVulkanInfo()
{
    if (!uAPI_PRIME_VULKAN_INFO)
    {
        *(uVulkanInfo**)&uAPI_PRIME_VULKAN_INFO =
            (uVulkanInfo*)calloc(1, sizeof(uVulkanInfo));
    }

    return uAPI_PRIME_VULKAN_INFO;
}
// [ end ] Prime uVulkanInfo
//


//
// [ begin ] Prime uVulkanImageGroup
// [ cfarvin::REVISIT ] Consider re-naming to uVulkanPresentGroup?
typedef struct
{
    _mut_ VkImage*       images;
    _mut_ VkImageView*   image_views;   // Note: To have num_images elements
    _mut_ VkFramebuffer* frame_buffers; // Note: To have num_images elements
    const u32            num_images;
} uVulkanImageGroup;
__UE_singleton__ uVulkanImageGroup* uAPI_PRIME_VULKAN_IMAGE_GROUP = NULL;
__UE_internal__ __UE_inline__ const uVulkanImageGroup* const
uGetVulkanImageGroup()
{
    if(!uAPI_PRIME_VULKAN_IMAGE_GROUP)
    {
        *(uVulkanImageGroup**)&uAPI_PRIME_VULKAN_IMAGE_GROUP =
            (uVulkanImageGroup*)calloc(1, sizeof(uVulkanImageGroup));
    }

    return uAPI_PRIME_VULKAN_IMAGE_GROUP;
}
// [ begin ] Prime uVulkanImageGroup
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
__UE_singleton__ uVulkanRenderInfo* uAPI_PRIME_VULKAN_RENDER_INFO = NULL;
__UE_internal__ __UE_inline__ const uVulkanRenderInfo* const
uGetVulkanRenderInfo()
{
    if(!uAPI_PRIME_VULKAN_RENDER_INFO)
    {
        *(uVulkanRenderInfo**)&uAPI_PRIME_VULKAN_RENDER_INFO =
            (uVulkanRenderInfo*)calloc(1, sizeof(uVulkanRenderInfo));
    }

    return uAPI_PRIME_VULKAN_RENDER_INFO;
}
// [ begin ] Prime uVulkanRenderInfo
//


#define uVULKAN_NUM_COMMAND_BUFFERS   3
#define uVULKAN_MAX_FRAMES_IN_FLIGHT  2
typedef struct
{
    VkSemaphore          wait_semaphores[uVULKAN_MAX_FRAMES_IN_FLIGHT];
    VkSemaphore          signal_semaphores[uVULKAN_MAX_FRAMES_IN_FLIGHT];
    VkFence              swap_chain_image_fences[uVULKAN_NUM_COMMAND_BUFFERS];
    VkFence              fences[uVULKAN_MAX_FRAMES_IN_FLIGHT];
    VkPipelineStageFlags stage_flags[uVULKAN_MAX_FRAMES_IN_FLIGHT];
    VkDevice             logical_device;
    VkSwapchainKHR       swap_chain;
    VkSubmitInfo         submit_info;
    VkQueue              graphics_queue;
    VkQueue              present_queue;
    VkPresentInfoKHR     present_info;
    u32                  frame;

    // [ cfarvin::REVISIT ] At most we want tripple buffering?
    VkCommandBuffer  command_buffers[uVULKAN_NUM_COMMAND_BUFFERS];
} uVulkanDrawTools;


#if __UE_debug__ == 1
VkDebugUtilsMessengerEXT           vulkan_main_debug_messenger;
VkDebugUtilsMessengerCreateInfoEXT vulkan_main_debug_messenger_info  = { 0 };
VkDebugUtilsMessengerCreateInfoEXT vulkan_setup_debug_messenger_info = { 0 };
#endif // __UE_debug__ == 1

//
// [ begin ] Forward decls
__UE_internal__ void __UE_call__
uDestroyVulkan();

__UE_internal__ void __UE_call__
uQueryVulkanDeviceExtensions(const VkPhysicalDevice*             restrict physical_device,
                             const s8**              const const restrict user_device_extension_names,
                             const u16                                    num_user_device_extension_names,
                             _mut_ u16*              const       restrict num_verified_extension_names);

__UE_internal__ __UE_inline__ bool
uSelectVulkanSwapChain(_mut_ uVulkanSurfaceInfo* const restrict surface_info);

__UE_internal__ __UE_inline__ void
uCreateVulkanSwapChain(_mut_ uVulkanInfo*        const restrict v_info,
                       _mut_ uVulkanSurfaceInfo* const restrict surface_info,
                       const uVulkanQueueInfo*   const restrict queue_info,
                       _mut_ uVulkanImageGroup*  _mut_ restrict image_group);

__UE_internal__ __UE_inline__ void
uCreateVulkanImageViews(const uVulkanInfo*        const restrict v_info,
                        _mut_ uVulkanImageGroup*  _mut_ restrict image_group,
                        const uVulkanSurfaceInfo* const restrict surface_info);

// [ cfarvin::TODO ] When we are rendering w/ more than one attachment:
//                   Update this function to take the number and type of attachments.
__UE_internal__ __UE_inline__ void
uCreateVulkanRenderPass(const uVulkanInfo*        const restrict v_info,
                        const uVulkanSurfaceInfo* const restrict surface_info,
                        _mut_ uVulkanRenderInfo*  const restrict render_info);

__UE_internal__ void __UE_call__
uCreateVulkanGraphicsPipeline(const uVulkanInfo*        const restrict v_info,
                              const uVulkanSurfaceInfo* const restrict surface_info,
                              _mut_ uVulkanRenderInfo*  const restrict render_info);

__UE_internal__ void __UE_call__
uCreateVulkanFrameBuffers(const uVulkanInfo*        const restrict v_info,
                          const uVulkanSurfaceInfo* const restrict surface_info,
                          const uVulkanImageGroup*  const restrict image_group,
                          const uVulkanRenderInfo*  const restrict render_info);

__UE_internal__ void __UE_call__
uCreateVulkanCommandBuffers(const uVulkanInfo*        const restrict v_info,
                            const uVulkanCommandInfo* const restrict command_info,
                            const uVulkanImageGroup*  const restrict image_group,
                            const uVulkanRenderInfo*  const restrict render_info,
                            const uVulkanSurfaceInfo* const restrict surface_info);
// [ end ] Forward decls
//


__UE_internal__ __UE_inline__ void
uDestroyVulkanPipelineRenderObjects()
{
    uVulkanInfo*       v_info      = (uVulkanInfo*)uGetVulkanInfo();
    uVulkanRenderInfo* render_info = (uVulkanRenderInfo*)uGetVulkanRenderInfo();

    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(render_info,            "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");


    if (v_info->logical_device)
    {
        // Destroy pipeline layout, render pass, graphics pipeline, frame buffer(s)
        vkDestroyPipeline(v_info->logical_device, render_info->graphics_pipeline, NULL);
        vkDestroyPipelineLayout(v_info->logical_device, render_info->pipeline_layout, NULL);
        vkDestroyRenderPass(v_info->logical_device, render_info->render_pass, NULL);
    }
}


__UE_internal__ __UE_inline__ void
uDestroyVulkanRenderInfo()
{
    uVulkanInfo*       v_info      = (uVulkanInfo*)uGetVulkanInfo();
    uVulkanRenderInfo* render_info = (uVulkanRenderInfo*)uGetVulkanRenderInfo();

    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(render_info,            "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");


    if (render_info)
    {
        uDestroyVulkanPipelineRenderObjects();

        if (v_info->logical_device)
        {
            vkDestroyDevice(v_info->logical_device, NULL); // [ cfarvin::REVISIT ]
        }

        if (render_info->attachment_descriptions)
        {
            free((VkAttachmentDescription*)render_info->attachment_descriptions);
        }

        if (render_info->attachment_references)
        {
            free((VkAttachmentReference*)render_info->attachment_references);
        }

        free(render_info);
    }
}



__UE_internal__ __UE_inline__ void
uDestroyVulkanQueueInfo()
{
    uVulkanQueueInfo* queue_info = (uVulkanQueueInfo*)uGetVulkanQueueInfo();
    uAssertMsg_v(queue_info, "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");


    if (queue_info)
    {
        free(queue_info);
    }
}


__UE_internal__ __UE_inline__ void
uDestroyVulkanCommandBuffers()
{
    uVulkanInfo*        v_info       = (uVulkanInfo*)uGetVulkanInfo();
    uVulkanCommandInfo* command_info = (uVulkanCommandInfo*)uGetVulkanCommandInfo();

    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(command_info,           "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");

    vkFreeCommandBuffers(v_info->logical_device,
                         command_info->command_pool,
                         command_info->num_command_buffers,
                         command_info->command_buffers);

    if (command_info->command_buffers)
    {
        free(command_info->command_buffers);
    }
}


__UE_internal__ __UE_inline__ void
uDestroyVulkanCommandInfo()
{
    uVulkanInfo*        v_info       = (uVulkanInfo*)uGetVulkanInfo();
    uVulkanCommandInfo* command_info = (uVulkanCommandInfo*)uGetVulkanCommandInfo();

    uAssertMsg_v(v_info,           "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(command_info,     "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");


    if (command_info)
    {
        uDestroyVulkanCommandBuffers();

        if(v_info && v_info->logical_device)
        {
            vkDestroyCommandPool(v_info->logical_device,
                                 command_info->command_pool,
                                 NULL);
        }

        free(command_info);
    }
}


__UE_internal__ __UE_inline__ void
uDestroyVulkanSurfaceInfo()
{
    uVulkanSurfaceInfo* surface_info = (uVulkanSurfaceInfo*)uGetVulkanSurfaceInfo();
    uVulkanInfo*        v_info       = (uVulkanInfo*)uGetVulkanInfo();

    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance,       "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(surface_info,           "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");


    if (v_info->logical_device)
    {
        // Destroy swap chain
        vkDestroySwapchainKHR(v_info->logical_device, v_info->swap_chain, NULL);
    }

    if (surface_info)
    {
        if (v_info->instance)
        {
            vkDestroySurfaceKHR(v_info->instance, surface_info->surface, NULL);
        }

        if (surface_info->surface_formats)
        {
            free((VkSurfaceFormatKHR*)surface_info->surface_formats);
        }

        if (surface_info->present_modes)
        {
            free((VkPresentModeKHR*)surface_info->present_modes);
        }

        free(surface_info);
    }
}


__UE_internal__ __UE_inline__ void
uDestroyVulkanInfo()
{
    uVulkanInfo* v_info = (uVulkanInfo*)uGetVulkanInfo();

    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance,       "[ vulkan ] VkInstance must be non zero.\n");

    if (v_info)
    {
        if (v_info->instance)
        {
#if __UE_debug__ == 1
            // Destroy debug messenger
            PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT =
                (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(v_info->instance,
                                                                           "vkDestroyDebugUtilsMessengerEXT");
            uAssertMsg_v(vkDestroyDebugUtilsMessengerEXT,
                         "[ vulkan ] Unable to acquire fnptr: vkDestroyDebugUtilsMessengerEXT().\n");
            if (vkDestroyDebugUtilsMessengerEXT && vulkan_main_debug_messenger)
            {
                vkDestroyDebugUtilsMessengerEXT(v_info->instance,
                                                vulkan_main_debug_messenger,
                                                NULL);
            }
#endif // __UE_debug__ == 1
        }

        if (v_info->instance)
        {
            vkDestroyInstance(v_info->instance, NULL);
        }

        free(v_info);
    }
}


__UE_internal__ __UE_inline__ void
uDestroyVulkanImageGroup()
{
    uVulkanInfo*       v_info      = (uVulkanInfo*)uGetVulkanInfo();
    uVulkanImageGroup* image_group = (uVulkanImageGroup*)uGetVulkanImageGroup();

    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(image_group,            "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");


    if (image_group)
    {
        if (v_info && v_info->logical_device)
        {
            // Destroy image views, frame buffers
            for (u32 image_idx = 0; image_idx < image_group->num_images; image_idx++)
            {
                if (image_group->image_views)
                {
                    vkDestroyImageView(v_info->logical_device,
                                       image_group->image_views[image_idx],
                                       NULL);
                }

                if (image_group->frame_buffers)
                {
                    vkDestroyFramebuffer(v_info->logical_device,
                                         image_group->frame_buffers[image_idx],
                                         NULL);
                }
            }
        }

        if (image_group->images)
        {
            free(image_group->images);
        }

        if (image_group->image_views)
        {
            free(image_group->image_views);
        }

        if (image_group->frame_buffers)
        {
            free(image_group->frame_buffers);
        }

        free(image_group);
    }
}


__UE_internal__ __UE_inline__ void
uDestroyOldSwapChain()
{
    uDestroyVulkanImageGroup();
    uDestroyVulkanCommandBuffers();
    uDestroyVulkanPipelineRenderObjects();
}


__UE_internal__ __UE_inline__ void
uRebuildVulkanSwapChain(VkDevice logical_device)
{
    // Wait for device idle; Clean up old data
    vkDeviceWaitIdle(logical_device);
    uDestroyOldSwapChain();


    // Recreate singletons that were delted by old swap chain clean up,
    // get references to those which were not.
    uVulkanInfo*        v_info       = (uVulkanInfo*)uGetVulkanInfo();
    uVulkanQueueInfo*   queue_info   = (uVulkanQueueInfo*)uGetVulkanQueueInfo();
    uVulkanSurfaceInfo* surface_info = (uVulkanSurfaceInfo*)uGetVulkanSurfaceInfo();
    uVulkanImageGroup*  image_group  = (uVulkanImageGroup*)uGetVulkanImageGroup();
    uVulkanRenderInfo*  render_info  = (uVulkanRenderInfo*)uGetVulkanRenderInfo();
    uVulkanCommandInfo* command_info = (uVulkanCommandInfo*)uGetVulkanCommandInfo();

    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance,       "[ vulkan ] VkInstance ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(queue_info,             "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info,           "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(image_group,            "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");
    uAssertMsg_v(render_info,            "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");
    uAssertMsg_v(command_info,           "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");


    // Rebuild swap chain
    uCreateVulkanSwapChain((uVulkanInfo*)v_info, surface_info, queue_info, image_group);
    uCreateVulkanImageViews(v_info, image_group, surface_info);

    // [ cfarvin::TODO ] When we are rendering w/ more than one attachment:
    //                   Update this function to take the number and type of attachments.
    uCreateVulkanRenderPass(v_info, surface_info, render_info);

    uCreateVulkanGraphicsPipeline(v_info, surface_info, render_info);
    uCreateVulkanFrameBuffers(v_info, surface_info, image_group, render_info);
    uCreateVulkanCommandBuffers(v_info, command_info, image_group, render_info, surface_info);
}


__UE_internal__ void __UE_call__
uCreateVulkanDrawSyncTools(const uVulkanInfo*      const restrict v_info,
                           _mut_ uVulkanDrawTools* const restrict draw_tools)
{
    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkDevice must be non zero.\n");
    uAssertMsg_v(draw_tools,             "[ vulkan ] uVulkanDrawTools ptr must be non null.\n");


    VkSemaphoreCreateInfo semaphore_create_info = { 0 };
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_create_info = { 0 };
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Done state

#if _WIN32
#pragma warning( push )
#pragma warning( disable : 4127 )
#endif // _WIN32
    uAssertMsg_v((uVULKAN_MAX_FRAMES_IN_FLIGHT == (uVULKAN_NUM_COMMAND_BUFFERS - 1)),
                 "[ api ] Max in-flight frames must be exactly one less than number command buffers.\n");
#if _WIN32
#pragma warning( pop )
#endif // _WIN32

    VkResult result = VK_SUCCESS;
    const char* sync_create_error_msg = "[ vulkan ] Unable to create draw syn objects.\n";

    for (u8 sync_obj_idx = 0; sync_obj_idx < uVULKAN_MAX_FRAMES_IN_FLIGHT; sync_obj_idx++)
    {
        // Create wait semaphores
        result = vkCreateSemaphore(v_info->logical_device,
                                   &semaphore_create_info,
                                   NULL,
                                   &(draw_tools->wait_semaphores[sync_obj_idx]));

        if (result != VK_SUCCESS)
        {
            uDestroyVulkan();
            uFatal(sync_create_error_msg);
        }

        // Create signal semaphores
        result = vkCreateSemaphore(v_info->logical_device,
                                   &semaphore_create_info,
                                   NULL,
                                   &(draw_tools->signal_semaphores[sync_obj_idx]));

        if (result != VK_SUCCESS)
        {
            uDestroyVulkan();
            uFatal(sync_create_error_msg);
        }

        // Create fences
        result = vkCreateFence(v_info->logical_device,
                                   &fence_create_info,
                                   NULL,
                                   &(draw_tools->fences[sync_obj_idx]));

        if (result != VK_SUCCESS)
        {
            uDestroyVulkan();
            uFatal(sync_create_error_msg);
        }
    }

// [ cfarvin::REVISIT ] These dont' get destroyed properly. See destruction in Understone.c::uDestroyDrawTools()
#if 0
    for (u8 fence_idx = 0; fence_idx < uVULKAN_NUM_COMMAND_BUFFERS; fence_idx++)
    {
        // Create swap chain image fences
        result = vkCreateFence(v_info->logical_device,
                               &fence_create_info,
                               NULL,
                               &(draw_tools->swap_chain_image_fences[fence_idx]));

        if (result != VK_SUCCESS)
        {
            uDestroyVulkan();
            uFatal(sync_create_error_msg);
        }
    }
#endif // 0
}


__UE_internal__ void __UE_call__
uCreateVulkanCommandBuffers(const uVulkanInfo*        const restrict v_info,
                            const uVulkanCommandInfo* const restrict command_info,
                            const uVulkanImageGroup*  const restrict image_group,
                            const uVulkanRenderInfo*  const restrict render_info,
                            const uVulkanSurfaceInfo* const restrict surface_info)
{
    uAssertMsg_v(v_info,                         "[ vulkan ] uVulkaninfo ptr must be non null.\n");
    uAssertMsg_v(v_info->physical_device,        "[ vulkan ] VkPhysicalDevice must be non zero.\n");
    uAssertMsg_v(command_info,                   "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");
    uAssertMsg_v(command_info->command_pool,     "[ vulkan ] VkCommandPool must be non zero.\n");
    uAssertMsg_v(image_group,                    "[ vulkan ] uVulkanImageGroup ptr must be non zero.\n");
    uAssertMsg_v(image_group->num_images,        "[ Vulkan ] Image count must be non zero.\n");
    uAssertMsg_v(render_info,                    "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");
    uAssertMsg_v(render_info->render_pass,       "[ vulkan ] VkRenderPass must be non zero.\n");
    uAssertMsg_v(render_info->graphics_pipeline, "[ vulkan ] VkPipeline ptr must be non zero.\n");
    uAssertMsg_v(surface_info,                   "[ vulkan ] uVulkanSurfaceInfo must be non null.\n");
    uAssertMsg_v(!command_info->command_buffers,
                 "[ vulkan ] VkCommandBuffer ptr must be null; will be overwritten.\n");


    // Create command buffers
    if (image_group->num_images > uVULKAN_NUM_COMMAND_BUFFERS)
    {
        uDestroyVulkan();
        uFatal("[ api ] More swap chain images than command buffers: (%d, %d).\n",
               image_group->num_images,
               uVULKAN_NUM_COMMAND_BUFFERS);
    }

    *(VkCommandBuffer**)&(command_info->command_buffers) = calloc(image_group->num_images,
                                                                  sizeof(VkCommandBuffer));

    if (!command_info->command_buffers)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to allocate command buffers.\n");
    }

    // Note: cmd buffer count set in struct for outside use; image number used internally.
    *(u32*)&(command_info->num_command_buffers) = image_group->num_images;

    VkCommandBufferAllocateInfo cb_alloc_create_info = { 0 };
    cb_alloc_create_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cb_alloc_create_info.commandPool        = command_info->command_pool;
    cb_alloc_create_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cb_alloc_create_info.commandBufferCount = image_group->num_images;

    VkResult result = vkAllocateCommandBuffers(v_info->logical_device,
                                               &cb_alloc_create_info,
                                               (VkCommandBuffer*)command_info->command_buffers);

    if (result != VK_SUCCESS)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create command buffers.\n");
    }

    for (u32 image_idx = 0; image_idx < image_group->num_images; image_idx++)
    {
        // Begin cb recording
        VkCommandBufferBeginInfo cb_begin_info = { 0 };
        cb_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cb_begin_info.flags            = 0;
        cb_begin_info.pInheritanceInfo = NULL;

        result = vkBeginCommandBuffer(command_info->command_buffers[image_idx],
                                      &cb_begin_info);

        if (result != VK_SUCCESS)
        {
            uDestroyVulkan();
            uFatal("[ vulkan ] Unable to begin command buffer at index: %d.\n",
                   image_idx);
        }

        // Begin render passs
        VkClearValue clear_value = { 0.0f };
        clear_value.color.float32[3]= 1.0f;

        VkRenderPassBeginInfo rp_begin_info = { 0 };
        rp_begin_info.sType               = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rp_begin_info.renderPass          = render_info->render_pass;
        rp_begin_info.framebuffer         = image_group->frame_buffers[image_idx];
        rp_begin_info.renderArea.offset.x = 0;
        rp_begin_info.renderArea.offset.y = 0;
        rp_begin_info.renderArea.extent   = surface_info->surface_extent;
        rp_begin_info.clearValueCount     = 1;
        rp_begin_info.pClearValues        = &clear_value;

        vkCmdBeginRenderPass(command_info->command_buffers[image_idx],
                             &rp_begin_info,
                             VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(command_info->command_buffers[image_idx],
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          render_info->graphics_pipeline);

        vkCmdDraw(command_info->command_buffers[image_idx],
                  3,  // vertex count
                  1,  // instance count
                  0,  // first vertex
                  0); // first instance

        vkCmdEndRenderPass(command_info->command_buffers[image_idx]);

        result = vkEndCommandBuffer(command_info->command_buffers[image_idx]);
        if (result != VK_SUCCESS)
        {
            uDestroyVulkan();
            uFatal("[ vulkan ] Unable to end command buffer.\n");
        }
    }
}


__UE_internal__ void __UE_call__
uCreateVulkanCommandPool(const uVulkanInfo*        const restrict v_info,
                         // [ cfarvin::REMOVE ]
                         /* const uVulkanQueueInfo*   const restrict queue_info, */
                         const uVulkanCommandInfo* const restrict command_info)
{
    uAssertMsg_v(v_info,                  "[ vulkan ] uVulkaninfo ptr must be non null.\n");
    uAssertMsg_v(v_info->physical_device, "[ vulkan ] VkPhysicalDevice must be non zero.\n");
    // [ cfarvin::REMOVE ]
    /* uAssertMsg_v(queue_info,              "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n"); */
    uAssertMsg_v(command_info,            "[ vulkan ] uVulkanCommandInfo ptr must be non zero.\n");


    // Create graphics command pool
    VkCommandPoolCreateInfo command_pool_create_info = { 0 };
    command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.queueFamilyIndex = uVULKAN_GRAPHICS_QUEUE_INDEX;
    command_pool_create_info.flags = 0;

    VkResult result = vkCreateCommandPool(v_info->logical_device,
                                          &command_pool_create_info,
                                          NULL,
                                          (VkCommandPool*)&(command_info->command_pool));
    if (result != VK_SUCCESS)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create command pool.\n");
    }
}


__UE_internal__ void __UE_call__
uCreateVulkanFrameBuffers(const uVulkanInfo*        const restrict v_info,
                          const uVulkanSurfaceInfo* const restrict surface_info,
                          const uVulkanImageGroup*  const restrict image_group,
                          const uVulkanRenderInfo*  const restrict render_info)
{
    uAssertMsg_v(v_info,                      "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device,      "[ vulkan ] VkPhysicalDevice must be non zero.\n");
    uAssertMsg_v(surface_info,                "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(image_group,                 "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");
    uAssertMsg_v(!image_group->frame_buffers, "[ vulkan ] VkFrameBuffer ptr must be null; will be overwritten.\n");
    uAssertMsg_v(render_info,                 "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");


    // Create a frame buffer for each image view
    *(VkFramebuffer**)&(image_group->frame_buffers) = calloc(image_group->num_images, sizeof(VkFramebuffer));
    if (!(image_group->frame_buffers))
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to allocate frame buffer array.\n");
    }

    for (u32 image_idx = 0; image_idx < image_group->num_images; image_idx++)
    {
        VkFramebufferCreateInfo frame_buffer_create_info = { 0 };
        frame_buffer_create_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frame_buffer_create_info.renderPass      = render_info->render_pass;
        frame_buffer_create_info.attachmentCount = 1;
        frame_buffer_create_info.pAttachments    = &(image_group->image_views[image_idx]);
        frame_buffer_create_info.width           = surface_info->surface_extent.width;
        frame_buffer_create_info.height          = surface_info->surface_extent.height;
        frame_buffer_create_info.layers          = 1; // Non stereoscopic

        VkResult result = vkCreateFramebuffer(v_info->logical_device,
                                              &frame_buffer_create_info,
                                              NULL,
                                              &(image_group->frame_buffers[image_idx]));

        if (result != VK_SUCCESS)
        {
            uDestroyVulkan();
            uFatal("[ vulkan ] Unable to create frame buffer.\n");
        }
    }
}


// [ cfarvin::TODO ] When we are rendering w/ more than one attachment:
//                   Update this function to take the number and type of attachments.
__UE_internal__ __UE_inline__ void
uCreateVulkanRenderPass(const uVulkanInfo*        const restrict v_info,
                        const uVulkanSurfaceInfo* const restrict surface_info,
                        _mut_ uVulkanRenderInfo*  const restrict render_info)
{
    uAssertMsg_v(v_info,                        "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info,                  "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info->surface_formats, "[ vulkan ] VkSurfaceFormatsKHR ptr must be non null.\n");
    uAssertMsg_v(render_info,                   "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");
    uAssertMsg_v(!render_info->attachment_descriptions,
                 "[ vulkan ] VkAttachmentDesctiption ptr must be null; will be overwritten.\n");
    uAssertMsg_v(!render_info->attachment_references,
                 "[ vulkan ] VkAttachmentReference ptr must be null; will be overwritten.\n");


    // [ cfarvin::TEMP ] Will determine number and type from arguments in the future,
    //                   hard coded for now.
    *(u32*)&render_info->num_attachments = 1;
    *(VkAttachmentDescription**)&(render_info->attachment_descriptions) = calloc(render_info->num_attachments,
                                                                                 sizeof(VkAttachmentDescription));
    if (!render_info->attachment_descriptions)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to allocate attachment descriptions.\n");
    }

    *(VkAttachmentReference**)&(render_info->attachment_references) = calloc(render_info->num_attachments,
                                                                             sizeof(VkAttachmentReference));
    if (!render_info->attachment_references)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to allocate attachment references.\n");
    }

    VkSurfaceFormatKHR surface_format =
        (surface_info->surface_formats)[surface_info->designated_format_index];

    // [ cfarvin::TODO ] This currently only handles color attachment references.
    for (u32 attachment_idx = 0; attachment_idx < render_info->num_attachments; attachment_idx++)
    {
        // Attachement description
        VkAttachmentDescription* attachment_descriptions =
            *(VkAttachmentDescription**)&(render_info->attachment_descriptions);

        attachment_descriptions[attachment_idx].format         = surface_format.format;
        attachment_descriptions[attachment_idx].samples        = VK_SAMPLE_COUNT_1_BIT;       // Note: 1 == No MSAA
        attachment_descriptions[attachment_idx].loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR; // [ cfarvin::PERF ] expensive or nominal?
        attachment_descriptions[attachment_idx].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        attachment_descriptions[attachment_idx].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment_descriptions[attachment_idx].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment_descriptions[attachment_idx].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment_descriptions[attachment_idx].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        // Attachment reference
        VkAttachmentReference* color_attachment_reference =
            *(VkAttachmentReference**)&(render_info->attachment_references);

        color_attachment_reference[attachment_idx].attachment = 0; // Read: "Attachment index"
        color_attachment_reference[attachment_idx].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    // Subpasses
    // Note: References layout(location = 0) in frag shader.
    // [ cfarvin::STUDY ] subpasses, subpass dependencies
    VkSubpassDependency subpass_dependency = { 0 };
    subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    subpass_dependency.dstSubpass = 0;
    subpass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.srcAccessMask = 0;
    subpass_dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpass_dependency.dstAccessMask = 0;

    VkSubpassDescription subpass_description = { 0 };
    subpass_description.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass_description.colorAttachmentCount = render_info->num_attachments;
    subpass_description.pColorAttachments    = render_info->attachment_references;

    // Render pass
    VkRenderPassCreateInfo render_pass_create_info = { 0 };
    render_pass_create_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.attachmentCount = render_info->num_attachments;
    render_pass_create_info.pAttachments    = render_info->attachment_descriptions;
    render_pass_create_info.subpassCount    = 1;
    render_pass_create_info.pSubpasses      = &subpass_description;
    render_pass_create_info.dependencyCount = 1;
    render_pass_create_info.pDependencies   = &subpass_dependency;

    VkResult result = vkCreateRenderPass(v_info->logical_device,
                                         &render_pass_create_info,
                                         NULL,
                                         (VkRenderPass*)&(render_info->render_pass));

    if (result != VK_SUCCESS)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create render pass.\n");
    }
}


__UE_internal__ void __UE_call__
uCreateVulkanShaderModule(const char*           const restrict spir_v_file_data,
                          const size_t                         spir_v_file_size,
                          _mut_ VkShaderModule* const restrict shader_module)
{
    const uVulkanInfo* const v_info = uGetVulkanInfo();

    uAssertMsg_v(v_info->logical_device, "[ vulkan ] SPIR-V file data must be non null.\n");
    uAssertMsg_v(spir_v_file_data,       "[ vulkan ] SPIR-V file data must be non null.\n");
    uAssertMsg_v(spir_v_file_size,       "[ vulkan ] SPIR-V file size must be non zero.\n");
    uAssertMsg_v(shader_module,          "[ vulkan ] VkShaderModule ptr must not be null.\n");


    VkShaderModuleCreateInfo smodule_create_info = { 0 };
    smodule_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    smodule_create_info.codeSize = spir_v_file_size;
    smodule_create_info.pCode    = (const u32*)spir_v_file_data;

    VkResult result = vkCreateShaderModule(v_info->logical_device,
                                           &smodule_create_info,
                                           NULL,
                                           shader_module);
    if (result != VK_SUCCESS)
    {
        free((void*)spir_v_file_data);
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create shader module.\n");
    }
}


__UE_internal__ void __UE_call__
uReadSpirvFile(const char*   const       restrict file_name,
               _mut_ char**  _mut_ const restrict return_file_data,
               _mut_ size_t*             restrict return_file_size)
{
    uAssertMsg_v(file_name,            "[ vulkan ] File name ptr must be non null.\n");
    uAssertMsg_v(!(*return_file_data), "[ vulkan ] Return file bytes ptr must be null; will be overwritten.\n");
    uAssertMsg_v(return_file_size,     "[ vulkan ] Return file size ptr must be non null.\n");


    FILE* spir_v_file = NULL;
    fopen_s(&spir_v_file, file_name, "rb");
    if (!spir_v_file)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to open SPIR-V file: %s\n", file_name);
    }

    s32 fseek_success = fseek(spir_v_file, 0L, SEEK_END);
    if (fseek_success) // Success == 0
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to seek within SPIR-V file: %s\n", file_name);
    }

    *return_file_size = (size_t)ftell(spir_v_file);
    if (!(*return_file_size))
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] SPIR-V file: %s has zero size.\n", file_name);
    }

    size_t allocation_size = *return_file_size * sizeof(char);
    *return_file_data = (char*)calloc(1, allocation_size);
    if (!(*return_file_data))
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to allocate host memory for SPIR-V file: %s.\n" , file_name);
    }

    rewind(spir_v_file);
    size_t items_read = fread_s((void*)(*return_file_data), // buffer
                                allocation_size,            // buffer size
                                1,                          // element size
                                (*return_file_size),        // element count
                                spir_v_file);               // FILE*

    if (items_read != *return_file_size)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to verify reading of SPIR-V file: %s.\n" , file_name);
    }

    fclose(spir_v_file);
    uVkVerbose("\tLoaded shader: %s.\n", file_name);
}


__UE_internal__ void __UE_call__
uCreateVulkanGraphicsPipeline(const uVulkanInfo*        const restrict v_info,
                              const uVulkanSurfaceInfo* const restrict surface_info,
                              _mut_ uVulkanRenderInfo*  const restrict render_info)
{
    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkPhysical device must be non zero.\n");
    uAssertMsg_v(surface_info,           "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(render_info,            "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");


    // [ cfarvin::TODO ] Modules below are hard coded for now
    uVkVerbose("Loading shader files...\n");

    // Vertex module
    VkShaderModule vkTriangle_vert_module = NULL;
    char*          vkTriangle_vert_data   = NULL;
    size_t         vkTriangle_vert_size   = 0;

    uReadSpirvFile(".\\shaders\\bin\\vkTriangle_vert.spv",
                   &vkTriangle_vert_data,
                   &vkTriangle_vert_size);

    uCreateVulkanShaderModule(vkTriangle_vert_data,
                              vkTriangle_vert_size,
                              &vkTriangle_vert_module);

    // Fragment module
    VkShaderModule vkTriangle_frag_module = NULL;
    char*          vkTriangle_frag_data   = NULL;
    size_t         vkTriangle_frag_size   = 0;

    uReadSpirvFile(".\\shaders\\bin\\vkTriangle_frag.spv",
                   &vkTriangle_frag_data,
                   &vkTriangle_frag_size);

    uCreateVulkanShaderModule(vkTriangle_frag_data,
                              vkTriangle_frag_size,
                              &vkTriangle_frag_module);

    // Create shader stages
    VkPipelineShaderStageCreateInfo shader_stage_array[2] = { 0 };

    // Vert
    shader_stage_array[0].sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_array[0].stage               = VK_SHADER_STAGE_VERTEX_BIT;
    shader_stage_array[0].module              = vkTriangle_vert_module;
    shader_stage_array[0].pName               = "main"; // entry point
    shader_stage_array[0].pSpecializationInfo = NULL;   // optional: set shader constants

    // Frag
    shader_stage_array[1].sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shader_stage_array[1].stage               = VK_SHADER_STAGE_FRAGMENT_BIT;
    shader_stage_array[1].module              = vkTriangle_frag_module;
    shader_stage_array[1].pName               = "main"; // entry point
    shader_stage_array[1].pSpecializationInfo = NULL;   // optional: set shader constants

    // Vertex input
    VkPipelineVertexInputStateCreateInfo vertex_input_create_info = { 0 }; // [ cfarvin::STUDY ]
    vertex_input_create_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_create_info.vertexBindingDescriptionCount   = 0;
    vertex_input_create_info.pVertexBindingDescriptions      = NULL;
    vertex_input_create_info.vertexAttributeDescriptionCount = 0;
    vertex_input_create_info.pVertexAttributeDescriptions    = NULL;

    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info = { 0 };
    input_assembly_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_create_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_create_info.primitiveRestartEnable = VK_FALSE; // [ cfarvin::STUDY ]

    // Viewport
    VkViewport frame_buffer_viewport = { 0 };
    frame_buffer_viewport.x        = 0.0f;
    frame_buffer_viewport.y        = 0.0f;
    frame_buffer_viewport.width    = (r32)surface_info->surface_extent.width;
    frame_buffer_viewport.height   = (r32)surface_info->surface_extent.height;
    frame_buffer_viewport.minDepth = 0.0f;
    frame_buffer_viewport.maxDepth = 1.0f;

    // Scissor
    VkRect2D frame_buffer_scissor = { 0 };
    frame_buffer_scissor.offset.x = 0;
    frame_buffer_scissor.offset.y = 0;
    frame_buffer_scissor.extent = surface_info->surface_extent;

    // Viewport state
    VkPipelineViewportStateCreateInfo frame_buffer_viewport_create_info = { 0 };
    frame_buffer_viewport_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    frame_buffer_viewport_create_info.viewportCount = 1;
    frame_buffer_viewport_create_info.pViewports    = &frame_buffer_viewport;
    frame_buffer_viewport_create_info.scissorCount  = 1;
    frame_buffer_viewport_create_info.pScissors     = &frame_buffer_scissor;

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo raster_create_info = { 0 };
    raster_create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    raster_create_info.depthClampEnable        = VK_FALSE;
    raster_create_info.rasterizerDiscardEnable = VK_FALSE;
    raster_create_info.polygonMode             = VK_POLYGON_MODE_FILL;
    raster_create_info.lineWidth               = 1.0f;
    raster_create_info.cullMode                = VK_CULL_MODE_BACK_BIT;
    raster_create_info.frontFace               = VK_FRONT_FACE_CLOCKWISE;
    raster_create_info.depthBiasEnable         = VK_FALSE;
    raster_create_info.depthBiasConstantFactor = 0.0f;
    raster_create_info.depthBiasClamp          = 0.0f;
    raster_create_info.depthBiasSlopeFactor    = 0.0f;

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multi_sample_create_info = { 0 };
    multi_sample_create_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multi_sample_create_info.sampleShadingEnable   = VK_FALSE; // [ cfarvin::STUDY ]
    multi_sample_create_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
    multi_sample_create_info.minSampleShading      = 1.0f;
    multi_sample_create_info.pSampleMask           = NULL;
    multi_sample_create_info.alphaToCoverageEnable = VK_FALSE;
    multi_sample_create_info.alphaToOneEnable      = VK_FALSE;

    // Depth stencil [ currently off ]
    VkPipelineDepthStencilStateCreateInfo* depth_stencil_create_info = NULL;

    // Color blending
    // Note: VkPipelineColorBlendStateCreateInfo builds global state.
    //       VkPipelineColorBlendAttachmentState sets local state per attached frame buffer.
    // [ cfarvin::STUDY ]
    VkPipelineColorBlendAttachmentState color_blend_attachment = { 0 };
    color_blend_attachment.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT |
        VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable         = VK_FALSE;
    color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_blend_attachment.colorBlendOp        = VK_BLEND_OP_ADD;
    color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment.alphaBlendOp        = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo color_blend_create_info = { 0 };
    color_blend_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_create_info.logicOpEnable = VK_FALSE; // [ cfarvin::STUDY ]
    color_blend_create_info.logicOp = VK_LOGIC_OP_COPY;
    color_blend_create_info.attachmentCount = 1;
    color_blend_create_info.pAttachments = &color_blend_attachment;
    color_blend_create_info.blendConstants[0] = 0.0f;
    color_blend_create_info.blendConstants[1] = 0.0f;
    color_blend_create_info.blendConstants[2] = 0.0f;
    color_blend_create_info.blendConstants[3] = 0.0f;

    // Dynamic state
    // Note: Causes previous configuration to be ignored, and information to be supplied
    //       at drawing time.
    VkDynamicState dynamic_states[2] = { 0 };
    dynamic_states[0] = VK_DYNAMIC_STATE_VIEWPORT;
    dynamic_states[1] = VK_DYNAMIC_STATE_LINE_WIDTH;

    VkPipelineDynamicStateCreateInfo dynamic_state_create_info = { 0 };
    dynamic_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_create_info.dynamicStateCount = 2;
    dynamic_state_create_info.pDynamicStates    = dynamic_states;

    // Pipeline layout
    VkPipelineLayoutCreateInfo pipeline_layout_create_info = { 0 };
    pipeline_layout_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.setLayoutCount         = 0;
    pipeline_layout_create_info.pSetLayouts            = NULL;
    pipeline_layout_create_info.pushConstantRangeCount = 0;
    pipeline_layout_create_info.pPushConstantRanges    = NULL;

    VkResult result = vkCreatePipelineLayout(v_info->logical_device,
                                             &pipeline_layout_create_info,
                                             NULL,
                                             (VkPipelineLayout*)&(render_info->pipeline_layout));

    if (result != VK_SUCCESS)
    {
        if (vkTriangle_vert_data)
        {
            free(vkTriangle_vert_data);
        }

        if (vkTriangle_vert_data)
        {
            free(vkTriangle_vert_data);
        }
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create pipeline layout.\n");
    }

    // Create graphics pipeline
    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = { 0 };
    graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_create_info.stageCount          = 2;
    graphics_pipeline_create_info.pStages             = shader_stage_array;
    graphics_pipeline_create_info.pVertexInputState   = &vertex_input_create_info;
    graphics_pipeline_create_info.pInputAssemblyState = &input_assembly_create_info;
    graphics_pipeline_create_info.pViewportState      = &frame_buffer_viewport_create_info;
    graphics_pipeline_create_info.pRasterizationState = &raster_create_info;
    graphics_pipeline_create_info.pMultisampleState   = &multi_sample_create_info;
    graphics_pipeline_create_info.pDepthStencilState  = depth_stencil_create_info; // Currently off
    graphics_pipeline_create_info.pColorBlendState    = &color_blend_create_info;
    graphics_pipeline_create_info.pDynamicState       = NULL;                      // Filled out, not using.
    graphics_pipeline_create_info.layout              = render_info->pipeline_layout;
    graphics_pipeline_create_info.renderPass          = render_info->render_pass;
    graphics_pipeline_create_info.subpass             = 0;                         // Read: "Subpass index"
    graphics_pipeline_create_info.basePipelineHandle  = NULL;
    graphics_pipeline_create_info.basePipelineIndex   = -1;

    result = vkCreateGraphicsPipelines(v_info->logical_device,
                                       VK_NULL_HANDLE,
                                       1,
                                       &graphics_pipeline_create_info,
                                       NULL,
                                       (VkPipeline*)&(render_info->graphics_pipeline));

    if (result != VK_SUCCESS)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create graphics pipeline.\n");
    }

    // Cleanup
    vkDestroyShaderModule(v_info->logical_device, vkTriangle_vert_module, NULL);
    vkDestroyShaderModule(v_info->logical_device, vkTriangle_frag_module, NULL);

    // [ cfarvin::REVISIT ] When is it safe to free these?
    if (vkTriangle_vert_data)
    {
        free(vkTriangle_vert_data);
    }

    if (vkTriangle_frag_data)
    {
        free(vkTriangle_frag_data);
    }
}


__UE_internal__ __UE_inline__ void
uCreateVulkanImageViews(const uVulkanInfo*        const restrict v_info,
                        _mut_ uVulkanImageGroup*  _mut_ restrict image_group,
                        const uVulkanSurfaceInfo* const restrict surface_info)
{
    uAssertMsg_v(v_info,                        "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device,        "[ vulkan ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(image_group,                   "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");
    uAssertMsg_v(image_group->images,           "[ vulkan ] VkImages ptr must be non null.\n");
    uAssertMsg_v(image_group->num_images,       "[ vulkan ] VkImage quantity must be non zero.\n");
    uAssertMsg_v(image_group->image_views,      "[ vulkan ] VkImages ptr must be non null.\n");
    uAssertMsg_v(surface_info,                  "[ vulkan ] uVulkanSurfaceInfo must be non null.\n");
    uAssertMsg_v(surface_info->surface_formats, "[ vulkan ] VkSurfaceFormatKHR ptr must be non null.\n");


    VkSurfaceFormatKHR* surface_formats = (VkSurfaceFormatKHR*)surface_info->surface_formats;
    for (u32 image_idx = 0; image_idx < image_group->num_images; image_idx++)
    {
        // Fill create info
        VkImageViewCreateInfo image_view_create_info = { 0 };

        image_view_create_info.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.image    = (image_group->images)[image_idx];
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format   = surface_formats[surface_info->designated_format_index].format;

        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;

        image_view_create_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel   = 0;
        image_view_create_info.subresourceRange.levelCount     = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount     = 1;

        // Create image view
        VkResult result = vkCreateImageView(v_info->logical_device,
                                            &image_view_create_info,
                                            NULL,
                                            &(image_group->image_views[image_idx]));

        if (result != VK_SUCCESS)
        {
            uDestroyVulkan();
            uFatal("[ vulkan ] Unable to create image view.\n");
        }
    }
}


__UE_internal__ void __UE_call__
uVulkanExtractUniqueQueueFamilies(const uVulkanQueueInfo* const restrict queue_info,
                                  _mut_ u32*              const restrict unique_queue_array,
                                  const u8                               num_possible_queues,
                                  _mut_ u8*               const restrict unique_queues_found)
{
    uAssertMsg_v(queue_info,          "[ vulkan ] The uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(unique_queue_array,  "[ vulkan ] The 'unique_queue_array' ptr must be non null.\n");
    uAssertMsg_v(num_possible_queues, "[ vulkan ] The number of possible queues must be non zero.\n");
    uAssertMsg_v(unique_queues_found, "[ vulkan ] The 'num_queues_found' ptr must be non null.\n");

    if (num_possible_queues)
    {
        for (u8 unchecked_queue_idx = 0; unchecked_queue_idx < num_possible_queues; unchecked_queue_idx++)
        {
            // Create graphics queue as unique by default.
            if (unchecked_queue_idx == 0)
            {
                unique_queue_array[(*unique_queues_found)] = queue_info->designated_graphics_index;
                (*unique_queues_found)++;
            }
            else
            {
                bool is_unique = true;

                // Check present queue uniqueness
                for (u8 unique_queue_idx = 0;
                     unique_queue_idx < (*unique_queues_found);
                     unique_queue_idx++)
                {
                    if (queue_info->designated_present_index == unique_queue_array[unique_queue_idx])
                    {
                        is_unique = false;
                        break;
                    }
                }

                if (is_unique)
                {
                    unique_queue_array[(*unique_queues_found)] = queue_info->designated_present_index;
                    (*unique_queues_found)++;
                }
            }
        }
    }
}


__UE_internal__ __UE_inline__ void
uCreateVulkanSwapChain(_mut_ uVulkanInfo*        const restrict v_info,
                       _mut_ uVulkanSurfaceInfo* const restrict surface_info,
                       const uVulkanQueueInfo*   const restrict queue_info,
                       _mut_ uVulkanImageGroup*  _mut_ restrict image_group)
{
    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkLogicalDevice must be non zero.\n");
    uAssertMsg_v(!v_info->swap_chain,    "[ vulkan ] VkSwapChainKHR must be zero; will be overwritten.\n");
    uAssertMsg_v(surface_info,           "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info->surface,  "[ vulkan ] VkSurfaceKHR ptr must be non null.\n");
    uAssertMsg_v(queue_info,             "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info->surface_formats,
                 "[ vulkan ] VkSurfaceFormatKHR ptr must be non null.\n");
    uAssertMsg_v(surface_info->present_modes,
                 "[ vulkan ] VkPresentModeKHR ptr must be non null.\n");


    // Select a suitable swapchain
    bool swap_chain_selected = uSelectVulkanSwapChain(surface_info);
    if (!swap_chain_selected)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to select a suitable swap chain.\n");
    }

    // Determine swap chain image capacity
    u32 min_image_count = surface_info->surface_capabilities.minImageCount;
    u32 max_image_count = surface_info->surface_capabilities.maxImageCount;

    // Request an additional image
    u32 designated_image_count = min_image_count + 1;

    // Ensure designated count is within bounds
    bool image_count_unlimited = (max_image_count == 0);
    if (!image_count_unlimited && (designated_image_count > max_image_count))
    {
        designated_image_count = max_image_count;
    }

    // Determine if queues are unique
    u32 unique_queue_array[uVULKAN_NUM_QUEUES] = { 0 };
    u8  unique_queues_found = 0;
    uVulkanExtractUniqueQueueFamilies(queue_info,
                                      &(unique_queue_array[0]),
                                      uVULKAN_NUM_QUEUES,
                                      &unique_queues_found);

    if (!unique_queues_found)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Was unable to find any queues.\n");
    }

    const VkSurfaceFormatKHR* surface_formats = surface_info->surface_formats;
    const VkPresentModeKHR*   present_modes = surface_info->present_modes;

    // File create info
    VkSwapchainCreateInfoKHR swap_chain_create_info = { 0 };
    swap_chain_create_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swap_chain_create_info.surface          = surface_info->surface;
    swap_chain_create_info.minImageCount    = designated_image_count;
    swap_chain_create_info.imageFormat      = surface_formats[surface_info->designated_format_index].format;
    swap_chain_create_info.imageColorSpace  = surface_formats[surface_info->designated_format_index].colorSpace;
    swap_chain_create_info.imageExtent      = surface_info->surface_extent;
    swap_chain_create_info.imageArrayLayers = 1;
    swap_chain_create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // Render directly to images
    swap_chain_create_info.preTransform     = surface_info->surface_capabilities.currentTransform;
    swap_chain_create_info.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Ignore alpha channel
    swap_chain_create_info.presentMode      = present_modes[surface_info->designated_present_index];
    swap_chain_create_info.clipped          = VK_TRUE;
    swap_chain_create_info.oldSwapchain     = VK_NULL_HANDLE; // [ cfarvin::TODO ] handle differently on window resize
    if (unique_queues_found == 1)
    {
        // Queues were unique/identical
        swap_chain_create_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        swap_chain_create_info.queueFamilyIndexCount = 0;
        swap_chain_create_info.pQueueFamilyIndices   = NULL;
    }
    else
    {
        // Queues were not unique/identical
        swap_chain_create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swap_chain_create_info.queueFamilyIndexCount = unique_queues_found;
        swap_chain_create_info.pQueueFamilyIndices   = &(unique_queue_array[0]);
    }

    // Create swap chain
    VkResult result = vkCreateSwapchainKHR(v_info->logical_device,
                                           &swap_chain_create_info,
                                           NULL,
                                           (VkSwapchainKHR*)&(v_info->swap_chain));

    if (result != VK_SUCCESS)
    {
        free((VkSurfaceFormatKHR*)surface_info->surface_formats);
        free((VkPresentModeKHR*)surface_info->present_modes);
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create swap chain.\n");
    }

    // Get handles to swap chain images
    u32 reported_image_count = designated_image_count;
    result = vkGetSwapchainImagesKHR(v_info->logical_device,
                                     v_info->swap_chain,
                                     &designated_image_count,
                                     NULL);

    if (result != VK_SUCCESS)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to obtain swap chain image count.\n");
    }

    if (!reported_image_count)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Swap chain reports no images.\n");
    }

    if (designated_image_count != reported_image_count)
    {
        uWarning("[ vulkan ] Swap chain images count != designated count.\n");
        designated_image_count = reported_image_count;
    }

    image_group->images      =     (VkImage*)calloc(designated_image_count, sizeof(VkImage));
    image_group->image_views = (VkImageView*)calloc(designated_image_count, sizeof(VkImageView));
    result = vkGetSwapchainImagesKHR(v_info->logical_device,
                                     v_info->swap_chain,
                                     &designated_image_count,
                                     image_group->images);

    if (result != VK_SUCCESS)
    {
        uDestroyVulkan();
        free(image_group->images);
        uFatal("[ vulkan ] Unable to set swap chain image count handle(s).\n");
    }

    // Set image count
    *(u32*)&(image_group->num_images) = designated_image_count;
}


__UE_internal__ void __UE_call__
uCreateVulkanLogicalDevice(_mut_ uVulkanInfo*      const       restrict v_info,
                           const uVulkanQueueInfo* const       restrict queue_info,
                           const s8**              const const restrict instance_validation_layer_names,
                           const u16                                    num_instance_validation_layer_names,
                           const s8**              const const restrict user_device_extension_names,
                           const u16                                    num_user_device_extension_names)
{
    uAssertMsg_v(v_info,                   "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->physical_device,  "[ vulkan ] VkPhysicalDevice must be non zero.\n");
    uAssertMsg_v(!v_info->logical_device,  "[ vulkan ] VkDevice must be zero; will be overwritten.\n");
    uAssertMsg_v(queue_info,               "[ vulkan ] Queue indices ptr must be non null\n");
    uAssertMsg_v(queue_info->queues,       "[ vulkan ] VkQueue ptr must be non null.\n");
    if (num_instance_validation_layer_names)
    {
        uAssertMsg_v(instance_validation_layer_names && *instance_validation_layer_names,
                     "[ vulkan ] If the instance validation layer quanitity is non zero, the names ptr must be non null\n");
    }
    if (num_user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, the names ptr must be non null\n");
    }


    // Extract unique queue family index values from uVulkanQueueInfo
    u8  num_unique_queues = 0;
    u32 unique_queues[uVULKAN_NUM_QUEUES] = { 0 };
    uVulkanExtractUniqueQueueFamilies(queue_info,
                                      &(unique_queues[0]),
                                      uVULKAN_NUM_QUEUES,
                                      &num_unique_queues);

    // Create logical device create info structure(s)
    VkDeviceQueueCreateInfo* device_queue_create_infos =
        (VkDeviceQueueCreateInfo*) calloc(num_unique_queues, sizeof(VkDeviceQueueCreateInfo));
    if (!device_queue_create_infos)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create logical device.\n");
    }

    for (u32 queue_family_idx = 0; queue_family_idx < num_unique_queues; queue_family_idx++)
    {
        r32 device_queue_priorities = 1.0f;
        device_queue_create_infos[queue_family_idx].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device_queue_create_infos[queue_family_idx].queueFamilyIndex = queue_family_idx;
        device_queue_create_infos[queue_family_idx].queueCount = 1;

        // All queues have equal priority.
        device_queue_create_infos[queue_family_idx].pQueuePriorities = &device_queue_priorities;
    }

    // Note: on modification, update compatibility checks to uSelectVulkanPhysicalDevice();
    VkPhysicalDeviceFeatures physical_device_features = { 0 };

    VkDeviceCreateInfo logical_device_create_info = { 0 };
    logical_device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    logical_device_create_info.pQueueCreateInfos       = device_queue_create_infos;
    logical_device_create_info.queueCreateInfoCount    = num_unique_queues;
    logical_device_create_info.pEnabledFeatures        = &physical_device_features;
    logical_device_create_info.enabledLayerCount       = num_instance_validation_layer_names;
    logical_device_create_info.ppEnabledLayerNames     = instance_validation_layer_names;
    logical_device_create_info.enabledExtensionCount   = num_user_device_extension_names;
    logical_device_create_info.ppEnabledExtensionNames = user_device_extension_names;

    VkResult device_creation_success = vkCreateDevice(v_info->physical_device,
                                                      &logical_device_create_info,
                                                      NULL,
                                                      (VkDevice*)&v_info->logical_device);

    if (device_creation_success != VK_SUCCESS)
    {
        if (device_queue_create_infos)
        {
            free(device_queue_create_infos);
        }

        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create logical device.\n");
    }

    vkGetDeviceQueue(v_info->logical_device,
                     queue_info->designated_graphics_index,
                     0,
                     (VkQueue*)(&(queue_info->queues)[uVULKAN_GRAPHICS_QUEUE_INDEX]));

    vkGetDeviceQueue(v_info->logical_device,
                     queue_info->designated_present_index,
                     0,
                     (VkQueue*)(&(queue_info->queues)[uVULKAN_PRESENT_QUEUE_INDEX]));

    if (device_queue_create_infos)
    {
        free(device_queue_create_infos);
    }
}


__UE_internal__ bool __UE_call__
uValidateVulkanDeviceFeaturesReqruirement(const VkPhysicalDevice physical_device)
{
    if (physical_device) {}

    // Aquire physical device features
    /* VkPhysicalDeviceFeatures device_features; */
    /* vkGetPhysicalDeviceFeatures(physical_device, */
    /*                             &device_features); */

    return true;
}


__UE_internal__ bool __UE_call__
uValidateVulkanDevicePropertiesReqruirement(const VkPhysicalDevice physical_device)
{
    if (physical_device) {}

    // Aquire physical device properties
    /* VkPhysicalDeviceProperties device_properties; */
    /* vkGetPhysicalDeviceProperties(physical_device, */
    /*                               &device_properties); */

    return true;
}


__UE_internal__ bool __UE_call__
uValidateVulkanDeviceExtensionsReqruirement(const VkPhysicalDevice                      physical_device,
                                            const s8**             const const restrict user_device_extension_names,
                                            const u16                                   num_user_device_extension_names)
{
    if (user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, the names ptr must be non null\n");
    }


    u16 num_validated_extension_names = 0;
    uQueryVulkanDeviceExtensions(&physical_device,
                                 user_device_extension_names,
                                 num_user_device_extension_names,
                                 &num_validated_extension_names);

    const char* extension_name_err_msg = "[ vulkan ] Unable to verify user extension names.\n";
    uAssertMsg_v(num_validated_extension_names == num_user_device_extension_names, extension_name_err_msg);
    if (num_validated_extension_names != num_user_device_extension_names)
    {
        uVkVerbose(extension_name_err_msg);
        return false;
    }

    return true;
}


__UE_internal__ bool __UE_call__
uValidateVulkanDeviceQueueRequirement(const VkPhysicalDevice                 physical_device,
                                      const VkSurfaceKHR*     const restrict surface,
                                      _mut_ uVulkanQueueInfo* const restrict queue_info)
{
    uAssertMsg_v(surface,    "[ vulkan ] The VKSurfaceKHR ptr must be non null.\n");
    uAssertMsg_v(queue_info, "[ vulkan ] The uVulkanQueueInfo ptr must be non null.\n");


    typedef struct
    {
        u32  index;
        bool validated;
    } uQueueFamilyPair;
    u32 num_queues = 0;

    // Get queue family count
    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, NULL);
    if (!queue_family_count)
    {
        return false;
    }

    // Get queue families
    VkQueueFamilyProperties* queue_family_props = (VkQueueFamilyProperties*)calloc(queue_family_count,
                                                                                   sizeof(VkQueueFamilyProperties));
    if (!queue_family_props)
    {
        return false;
    }

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_family_props);

    // Require graphics family
    uQueueFamilyPair graphics_pair = { 0 };
    num_queues++;

    // Require present family
    uQueueFamilyPair present_pair  = { 0 };
    num_queues++;

    bool queues_satisfied = false;
    for (u32 queue_idx = 0; queue_idx < queue_family_count; queue_idx++)
    {
        // Check grahpics capability for this family
        if (queue_family_props[queue_idx].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphics_pair.index = queue_idx;
            graphics_pair.validated = true;
        }

        // Check present capability for this family
        VkBool32 present_capability = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device,
                                             queue_idx,
                                             *surface,
                                             &present_capability);

        if (present_capability)
        {
            present_pair.index = queue_idx;
            present_pair.validated = true;
        }

        // Break when all conditions are met
        if (graphics_pair.validated &&
            present_pair.validated)
        {
            // Store family indices for each required queue.
            *(u32*)&queue_info->designated_graphics_index = graphics_pair.index;
            *(u32*)&queue_info->designated_present_index  = present_pair.index;

            // Issue perf warning for different graphics/presentation queue
            if (graphics_pair.index != present_pair.index)
            {
                uWarning("[ vulkan ] [ perf ] Different graphics and prsent queue families chosen.\n");
            }

            queues_satisfied = true;
            break;
        }
    }

    if (queue_family_props)
    {
        free(queue_family_props);
    }

    // Issue engine level warning to update uVULKAN_NUM_QUEUES
    if (uVULKAN_NUM_QUEUES != num_queues)
    {
        uFatal("[ engine ][ vulkan ] uVulkanInfo.queues length: %d. %d queues were checked during physical device creation.\n",
               uVULKAN_NUM_QUEUES,
               num_queues);
    }

    return queues_satisfied;
}


__UE_internal__ __UE_inline__ void
uSelectVulkanSwapChainExtent(_mut_ uVulkanSurfaceInfo* const restrict surface_info)
{
    uAssertMsg_v(surface_info,       "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
#if _WIN32
    uAssertMsg_v(win32_info,         "[ win32 ] [ vulkan ] uWin32Info must be non null.\n");
    uAssertMsg_v(win32_info->window, "[ win32 ] [ vulkan ] uWin32Info window must be non null.\n");
#endif // _WIN32

    VkSurfaceCapabilitiesKHR surface_capabilities = surface_info->surface_capabilities;
    VkExtent2D* non_const_extent = (VkExtent2D*)&(surface_info->surface_extent);

    // Note: Some window managers use u32_MAX as a magic value which indicates that the extent
    // must be manually set up.
    u32 u32_MAX = ~(u32)0;
    if (surface_capabilities.currentExtent.width != u32_MAX)
    {

        non_const_extent->width  = surface_capabilities.currentExtent.width;
        non_const_extent->height = surface_capabilities.currentExtent.height;
    }
    else
    {
        uGetWindowSize(&non_const_extent->width, (u32*)&non_const_extent->height);
    }

    // Ensure we do not exceed maximums
    if (non_const_extent->width > surface_capabilities.maxImageExtent.width)
    {
        uVkVerbose("[ vulkan ] Calculated extent width exceeded surface capabiility; capped.\n");
        non_const_extent->width = surface_capabilities.maxImageExtent.width;
    }

    if (non_const_extent->height > surface_capabilities.maxImageExtent.height)
    {
        uVkVerbose("[ vulkan ] Calculated extent height exceeded surface capabiility; capped.\n");
        non_const_extent->height = surface_capabilities.maxImageExtent.height;
    }

    // Ensure we do not fall below minimums
    if (non_const_extent->width < surface_capabilities.minImageExtent.width)
    {
        uVkVerbose("[ vulkan ] Calculated extent width fell below surface capabiility; capped.\n");
        non_const_extent->width = surface_capabilities.minImageExtent.width;
    }

    if (non_const_extent->height < surface_capabilities.minImageExtent.height)
    {
        uVkVerbose("[ vulkan ] Calculated extent height fell below surface capabiility; capped.\n");
        non_const_extent->height = surface_capabilities.minImageExtent.height;
    }
}


__UE_internal__ __UE_inline__ bool
uSelectVulkanSwapChain(_mut_ uVulkanSurfaceInfo* const restrict surface_info)
{
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    if (surface_info->num_surface_formats)
    {
        uAssertMsg_v(surface_info->surface_formats, "[ vulkan ] VkSurfaceFormatKHR ptr must be non null.\n");
    }
    if (surface_info->num_present_modes)
    {
        uAssertMsg_v(surface_info->present_modes, "[ vulkan ] VkPresentModeKHR ptr must be non null.\n");
    }

    // Determine best available surface format
    u32*  non_const_designated_format_index = (u32*)&(surface_info->designated_format_index);
    bool optimal_format_found = false;
    for (u16 format_idx = 0; format_idx < surface_info->num_surface_formats; format_idx++)
    {
        if (surface_info->surface_formats[format_idx].format == VK_FORMAT_B8G8R8A8_SRGB &&
            surface_info->surface_formats[format_idx].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            *non_const_designated_format_index = format_idx;
            optimal_format_found    = true;
            break;
        }
    }

    if (!optimal_format_found)
    {
        // Issue perf warning
        uWarning("[ vulkan ] [ perf ] 8bpp sRGB swap chain format unavailble. Defaulting to first format index.\n");
    }

    // Determine best available presentation mode
    u32* non_const_designated_present_index = (u32*)&(surface_info->designated_present_index);
    bool suitable_present_found   = false;
    bool optimal_present_found    = false;
    u16  fifo_present_index       = 0;
    for (u16 present_idx = 0; present_idx < surface_info->num_present_modes; present_idx++)
    {
        VkPresentModeKHR present_mode = surface_info->present_modes[present_idx];
        if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            *non_const_designated_present_index = present_idx;
            optimal_present_found  = true;
            suitable_present_found = true;
            break;
        }
        else if (present_mode == VK_PRESENT_MODE_FIFO_KHR)
        {
            fifo_present_index     = present_idx;
            suitable_present_found = true;
        }
    }

    if (!suitable_present_found)
    {
        uFatal("[ vulkan ] Unable to find a suitable present mode.\n");
    }

    if (!optimal_present_found)
    {
        // Issue perf warning
        uWarning("[ vulkan ] [ perf ] Tripple buffering present mode unavailble. Defaulting to FIFO.\n");
        *non_const_designated_present_index = fifo_present_index;
    }

    // Determine best avialable swap extent
    uSelectVulkanSwapChainExtent(surface_info);

    // Note: require FIFO present mode; accept any format
    return suitable_present_found;
}


__UE_internal__ bool __UE_call__
uValidateVulkanSwapChainAndSurfaceCompatibility(const VkPhysicalDevice                   physical_device,
                                                const VkSurfaceKHR*       const restrict surface,
                                                _mut_ uVulkanSurfaceInfo* const restrict return_surface_info)
{
    uAssertMsg_v(surface,             "[ vulkan ] The VKSurfaceKHR ptr must be non null.\n");
    uAssertMsg_v(return_surface_info, "[ vulkan ] The uVulkanSurfaceInfo ptr must be non null.\n");


    // Get surface capabilities
    VkSurfaceCapabilitiesKHR* surface_capabilities = (VkSurfaceCapabilitiesKHR*)&(return_surface_info->surface_capabilities);
    VkResult success = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device,
                                                                 *surface,
                                                                 surface_capabilities);

    uAssert(success == VK_SUCCESS);
    if (success != VK_SUCCESS)
    {
        uVkVerbose("Could not attain surface capabilities.\n");
        return false;
    }

    // Get surface formats
    success = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,
                                                   *surface,
                                                   (u32*)&(return_surface_info->num_surface_formats),
                                                   NULL);

    uAssert(success == VK_SUCCESS);
    if (success != VK_SUCCESS)
    {
        uVkVerbose("Could not attain surface format quantity.\n");
        return false;
    }


    if (return_surface_info->num_surface_formats)
    {
        // Note: does not free in this scope.
        return_surface_info->surface_formats = (VkSurfaceFormatKHR*)malloc(return_surface_info->num_surface_formats *
                                                                           sizeof(VkSurfaceFormatKHR));
        success = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,
                                                       *surface,
                                                       (u32*)&(return_surface_info->num_surface_formats),
                                                       (VkSurfaceFormatKHR*)return_surface_info->surface_formats);

        uAssert(success == VK_SUCCESS);
        if (success != VK_SUCCESS)
        {
            uVkVerbose("Could not attain surface formats.\n");
            return false;
        }
    }
    else
    {
        return false;
    }

    // Get surface present modes
    success = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
                                                        *surface,
                                                        (u32*)&(return_surface_info->num_present_modes),
                                                        NULL);
    uAssert(success == VK_SUCCESS);
    if (success != VK_SUCCESS)
    {
        uVkVerbose("Could not attain present mode quantity.\n");
        return false;
    }

    if (return_surface_info->num_present_modes)
    {
        // Note: does not free in this scope
        return_surface_info->present_modes = (VkPresentModeKHR*)malloc(return_surface_info->num_present_modes *
                                                                       sizeof(VkPresentModeKHR));

        success = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
                                                            *surface,
                                                            (u32*)&(return_surface_info->num_present_modes),
                                                            (VkPresentModeKHR*)return_surface_info->present_modes);
        uAssert(success == VK_SUCCESS);
        if (success != VK_SUCCESS)
        {
            uVkVerbose("Could not attain present modes.\n");
            return false;
        }
    }

    return (return_surface_info->num_surface_formats &&
            return_surface_info->num_present_modes);
}


__UE_internal__ void __UE_call__
uSelectVulkanPhysicalDevice(const VkPhysicalDevice**  const const restrict physical_device_list,
                            _mut_ VkPhysicalDevice*   const       restrict return_device,
                            _mut_ uVulkanQueueInfo*   const       restrict queue_info,
                            const u32                                      num_physical_devices,
                            const VkSurfaceKHR*       const       restrict surface,
                            const s8**                const const restrict user_device_extension_names,
                            const u16                                      num_user_device_extension_names,
                            _mut_ uVulkanSurfaceInfo* const       restrict return_surface_info)
{
    uAssertMsg_v(physical_device_list, "[ vulkan ] VkPhysicalDevice ptr ptr must be non null.\n");
    uAssertMsg_v(!(*return_device),    "[ vulkan ] VkPhysicalDevice ptr must be null; will be overwritten\n");
    uAssertMsg_v(queue_info,           "[ vulkan ] uVulkanQueueInfo ptr must be non null\n");
    uAssertMsg_v(num_physical_devices, "[ vulkan ] Physical devices count must be non zero.\n");
    uAssertMsg_v(surface,              "[ vulkan ] VkSurfaceKHR ptr must be non null.\n");
    uAssertMsg_v(return_surface_info,  "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    for (u32 device_idx = 0; device_idx < num_physical_devices; device_idx++)
    {
        uAssertMsg_v(physical_device_list[device_idx],
                     "[ vulkan ] Indices of physical_device_list must be non-null.\n");
    }
    if (user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, the names ptr must be non null\n");
    }


    bool selection_complete = false;
    for (u32 device_idx = 0; device_idx < num_physical_devices; device_idx++)
    {
        const VkPhysicalDevice physical_device = *physical_device_list[device_idx];
        if (!physical_device)
        {
            continue;
        }

        selection_complete =
            uValidateVulkanDeviceFeaturesReqruirement(physical_device) &&
            uValidateVulkanDevicePropertiesReqruirement(physical_device) &&
            uValidateVulkanDeviceQueueRequirement(physical_device,
                                                  surface,
                                                  queue_info) &&
            uValidateVulkanDeviceExtensionsReqruirement(physical_device,
                                                        user_device_extension_names,
                                                        num_user_device_extension_names) &&
            // Must query for surface and extension support before swap chains support.
            uValidateVulkanSwapChainAndSurfaceCompatibility(physical_device,
                                                            surface,
                                                            return_surface_info);

        // Set the physical device
        if (selection_complete)
        {
            *return_device = physical_device;
            break;
        }
    }

    if (!selection_complete)
    {
        uFatal("[ vulkan ] Unable to select a suitable physical device.\n");
    }
}


__UE_internal__ void __UE_call__
uCreateVulkanPhysicalDevice(_mut_ uVulkanInfo*        const       restrict v_info,
                            _mut_ uVulkanQueueInfo*   const       restrict queue_info,
                            const s8**                const const restrict user_device_extension_names,
                            const u16                                      num_user_device_extension_names,
                            _mut_ uVulkanSurfaceInfo* const       restrict return_surface_info)
{
    uAssertMsg_v(v_info,                       "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(!v_info->physical_device,     "[ vulkan ] VkPhysicalDevice must be zero; will be overwritten.\n");
    uAssertMsg_v(!v_info->logical_device,      "[ vulkan ] VkDevice must be zero; will be overwritten.\n");
    uAssertMsg_v(queue_info,                   "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(return_surface_info,          "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(return_surface_info->surface, "[ vulkan ] VkSurfaceKHR must be zero; will be overwritten.\n");
    if (num_user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, the names ptr must be non null\n");
    }


    u32 num_physical_devices = 0;
    vkEnumeratePhysicalDevices(v_info->instance, &num_physical_devices, NULL);

    uAssertMsg_v(num_physical_devices, "[ vulkan ] No physical devices found.\n");
    if (!num_physical_devices)
    {
        return;
    }

    VkPhysicalDevice* physical_device_list = (VkPhysicalDevice*)calloc(num_physical_devices,
                                                                       sizeof(VkPhysicalDevice));
    uAssertMsg_v(physical_device_list, "[ vulkan ] Unable to allocate physical device list.\n");

    vkEnumeratePhysicalDevices(v_info->instance, &num_physical_devices, physical_device_list);
    uVkVerbose("Found %d physical devices.\n", num_physical_devices);

    VkPhysicalDevice candidate_device = NULL;
    uSelectVulkanPhysicalDevice(&physical_device_list,
                                &candidate_device,
                                queue_info,
                                num_physical_devices,
                                (VkSurfaceKHR*)&(return_surface_info->surface),
                                user_device_extension_names,
                                num_user_device_extension_names,
                                return_surface_info);

    if (!candidate_device)
    {
        if (physical_device_list)
        {
            free(physical_device_list);
        }

        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to find suitable device.\n");
        return;
    }

    VkPhysicalDevice* non_const_physical_device = (VkPhysicalDevice*)&(v_info->physical_device);
    *non_const_physical_device = candidate_device;

    if (physical_device_list)
    {
        free(physical_device_list);
    }
}


#if __UE_debug__ == 1
// Note: no function/argument decorations to conform w/ Vulkan spec.
static VKAPI_ATTR VkBool32 VKAPI_CALL
uVkDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity_bits,
    VkDebugUtilsMessageTypeFlagsEXT             message_type_bits,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data)
{
    VkBool32 should_abort_calling_process = VK_TRUE;
    if (user_data) {} // Silence warnings

    if (message_severity_bits >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ||
        message_type_bits >= VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
    {
        printf("[ vulkan validation ] %s.\n", callback_data->pMessage);
        fflush(stdout);

        /* uFatal("[ vulkan validation ] %s.\n", callback_data->pMessage); */
    }
#if __UE_VK_VERBOSE__ == 1
    else
    {
        printf("[ vulkan verbose validation ] %s.\n", callback_data->pMessage);
        fflush(stdout);
    }
#endif // __UE_VK_VERBOSE__== 1

    return should_abort_calling_process;
}


__UE_internal__ void __UE_call__
uCreateVulkanDebugMessengerInfo(_mut_ VkDebugUtilsMessengerCreateInfoEXT* const restrict debug_message_create_info)
{
    uAssertMsg_v(debug_message_create_info, "[ vulkan ] VkDebugUtilsMessengerCreateInfoEXT ptr must be non null.\n");


    debug_message_create_info->messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    debug_message_create_info->messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    debug_message_create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_message_create_info->pfnUserCallback = uVkDebugCallback;
    debug_message_create_info->pUserData = NULL;
}


__UE_internal__ void __UE_call__
uCreateVulkanDebugMessenger(const uVulkanInfo*                        const restrict v_info,
                            _mut_ VkDebugUtilsMessengerCreateInfoEXT* const restrict debug_message_create_info,
                            _mut_ VkDebugUtilsMessengerEXT*           const restrict debug_messenger)
{
    uAssertMsg_v(v_info,                    "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance,          "[ vulkan ] VkInstance ptr must be non zero.\n");
    uAssertMsg_v(debug_message_create_info, "[ vulkan ] VkDebugUtilsMessengerCreateInfoEXT ptr must be non null.\n");


    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(v_info->instance,
                                                                  "vkCreateDebugUtilsMessengerEXT");

    uAssertMsg_v(vkCreateDebugUtilsMessengerEXT,
                 "[ vulkan ] Failed to acquire pfn: vkCreateDebugUtilsMessengerEXT\n");

    if (vkCreateDebugUtilsMessengerEXT)
    {
        uCreateVulkanDebugMessengerInfo((VkDebugUtilsMessengerCreateInfoEXT*)debug_message_create_info);
        VkResult success = vkCreateDebugUtilsMessengerEXT(v_info->instance,
                                                          debug_message_create_info,
                                                          NULL,
                                                          debug_messenger);

        if (success != VK_SUCCESS)
        {
            uDestroyVulkan();
            uFatal("[ vulkan ] Failed to create debug messenger callback.\n");
        }
    }
}
#endif // __UE_debug__ == 1


#if _WIN32
__UE_internal__ void __UE_call__
uCreateWin32Surface(_mut_ uVulkanInfo*        const restrict v_info,
                    _mut_ uVulkanSurfaceInfo* const restrict surface_info)
{
    uAssertMsg_v(v_info,                "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance,      "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(surface_info,          "[ vulkan ] uVulkanSurfaceInfo ptr must be non null");
    uAssertMsg_v(!surface_info->surface,
                 "[ vulkan ] uVulkanSurfaceInfo ptr must be null; will be overwritten.\n");


    // [ cfarvin::REVISIT ]
    uWin32Info** non_const_win32_info = (uWin32Info**)&win32_info;
    *non_const_win32_info = (uWin32Info*)uWin32CreateWindow();
    uAssertMsg_v(win32_info, "[ vulkan ] [ win32 ] Unable to create Win32 window.\n");
    if (!win32_info)
    {
        return;
    }

    VkWin32SurfaceCreateInfoKHR win32_surface_info = { 0 };
    win32_surface_info.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    win32_surface_info.hwnd      = win32_info->window;
    win32_surface_info.hinstance = GetModuleHandle(NULL);

    VkResult win32_surface_result = vkCreateWin32SurfaceKHR(v_info->instance,
                                                            &win32_surface_info,
                                                            NULL,
                                                            (VkSurfaceKHR*)&surface_info->surface);

    if (win32_surface_result != VK_SUCCESS)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create win32 surface.\n");
    }
}
#endif // _WIN32


__UE_internal__ void __UE_call__
uCreateVulkanSurface(_mut_ uVulkanInfo*        const restrict v_info,
                     _mut_ uVulkanSurfaceInfo* const restrict surface_info)
{
    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance,       "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(surface_info,           "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(!surface_info->surface, "[ vulkan ] VkSurface must be zero; will be overwritten.\n");


#if _WIN32
    uCreateWin32Surface(v_info, surface_info);
#endif // _WIN32
}


__UE_internal__ void __UE_call__
uQueryVulkanDeviceExtensions(const VkPhysicalDevice* const       restrict physical_device,
                             const s8**              const const restrict user_device_extension_names,
                             const u16                                    num_user_device_extension_names,
                             _mut_ u16*              const       restrict num_verified_extension_names)
{
    uAssertMsg_v(physical_device,              "[ vulkan ] VkPhysicalDevice ptr must be non null.\n");
    uAssertMsg_v(num_verified_extension_names, "[ vulkan ] Verified extension count ptr must be non null.\n");
    if (num_user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, the names ptr must be non null.\n");
    }


    if (!num_user_device_extension_names)
    {
        return;
    }

    // Query Extension Count
    u32 num_available_device_extensions = 0;
    VkResult success = vkEnumerateDeviceExtensionProperties(*physical_device,
                                                            NULL,
                                                            &num_available_device_extensions,
                                                            NULL);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to enumerate extension properties.\n");
    }

    VkExtensionProperties* device_extension_properties =
        (VkExtensionProperties*)malloc(num_available_device_extensions * sizeof(VkExtensionProperties));

    // Query Extension Names
    success = vkEnumerateDeviceExtensionProperties(*physical_device,
                                                   NULL,
                                                   &num_available_device_extensions,
                                                   device_extension_properties);

    if (success != VK_SUCCESS)
    {
        if(device_extension_properties)
        {
            free(device_extension_properties);
        }

        uFatal("[ vulkan ] Unable to enumerate extension properties.\n");
    }

    // Verify user/device extensions match
    *num_verified_extension_names = 0;
    for (u32 device_extension_name_idx = 0;
         device_extension_name_idx < num_available_device_extensions;
         device_extension_name_idx++)
    {
        for (u32 user_device_extension_name_idx = 0;
             user_device_extension_name_idx < num_user_device_extension_names;
             user_device_extension_name_idx++)
        {
            if (strcmp((device_extension_properties[device_extension_name_idx]).extensionName,
                       user_device_extension_names[user_device_extension_name_idx]) == 0)
            {
                (*num_verified_extension_names)++;
            }
        }
    }

    if(device_extension_properties)
    {
        free(device_extension_properties);
    }
}


__UE_internal__ void __UE_call__
uQueryVulkanInstanceLayers(_mut_ s8***                 _mut_ _mut_ const restrict instance_validation_layer_names,
                           _mut_ VkLayerProperties**   const _mut_       restrict instance_validation_layer_properties,
                           _mut_ VkInstanceCreateInfo* const             restrict instance_create_info,
                           const s8**                  const       const restrict user_instance_validation_layer_names,
                           const u32                                              num_user_instance_validation_layer_names)
{
    uAssertMsg_v(instance_create_info,                     "[ vulkan ] VkInstanceCreateInfo ptr must be non null.\n");
    uAssertMsg_v(!(*instance_validation_layer_names),      "[ vulkan ] Layer names ptr must be null; will be overwritten.\n");
    uAssertMsg_v(!(*instance_validation_layer_properties), "[ vulkan ] VkLayerProperties ptr must be null; will be overwritten.\n");
    uAssertMsg_v(user_instance_validation_layer_names,     "[ vulkan ] Validation layer names ptr must be non null.\n");
    if (num_user_instance_validation_layer_names)
    {
        uAssertMsg_v(user_instance_validation_layer_names && *user_instance_validation_layer_names,
                     "[ vulkan ] If the instance layer quanitity is non zero, the names ptr must be non null.\n");
    }


    if (!num_user_instance_validation_layer_names)
    {
        return;
    }

    // Query Layer Count
    VkResult success = VK_SUCCESS;
    size_t num_available_layers = 0;
    success = vkEnumerateInstanceLayerProperties(&((u32)num_available_layers), NULL);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to enumerate layers.\n");
    }

    uAssertMsg_v((num_available_layers >= num_user_instance_validation_layer_names),
                 "[ vulkan ] Number of requested validation layers [ %d ] exceeds total avaialbe count [ %zd ].\n",
                 num_user_instance_validation_layer_names,
                 num_available_layers);

    *instance_validation_layer_properties =
        (VkLayerProperties*)malloc(num_available_layers * sizeof(VkLayerProperties));

    // Query Layer Names
    success = vkEnumerateInstanceLayerProperties(&(u32)num_available_layers,
                                                 *instance_validation_layer_properties);
    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to enumerate layers.\n");
    }


    // Set Layer Names
    uVkVerbose("Searching for validation layers...\n");
    u32 num_added_layers = 0;
    *instance_validation_layer_names = (s8**)malloc(num_available_layers * sizeof(s8**));
    for (u32 available_layer_idx = 0;
         available_layer_idx < num_available_layers;
         available_layer_idx++)
    {
        for (u32 user_layer_idx = 0;
             user_layer_idx < num_user_instance_validation_layer_names;
             user_layer_idx++)
        {
            uVkVerbose("\tLayer found: %s\n", (*instance_validation_layer_properties)[available_layer_idx].layerName);
            if (strcmp((const char*)user_instance_validation_layer_names[user_layer_idx],
                       (const char*)(*instance_validation_layer_properties)[available_layer_idx].layerName) == 0)
            {
                (*instance_validation_layer_names)[num_added_layers] =
                    (*instance_validation_layer_properties)[available_layer_idx].layerName;
                num_added_layers++;
            }
        }
    }

    if (num_added_layers != num_user_instance_validation_layer_names)
    {
        uFatal("[ vulkan ] Unable to load all requested layers.\n");
    }

    instance_create_info->enabledLayerCount   = num_added_layers;
    instance_create_info->ppEnabledLayerNames = *instance_validation_layer_names;
}


__UE_internal__ void __UE_call__
uQueryVulkanInstanceExtensions(const s8***                   _mut_ _mut_ const restrict instance_extension_names,
                               _mut_ VkExtensionProperties** _mut_ const       restrict instance_extension_properties,
                               _mut_ VkInstanceCreateInfo*   const             restrict instance_create_info,
                               const s8**                    const       const restrict user_instance_extension_names,
                               const u16                                                num_user_instance_extension_names)
{
    uAssertMsg_v(instance_create_info,              "[ vulkan ] VkInstanceCreateInfo ptr must be non null.\n");
    uAssertMsg_v(!(*instance_extension_names),      "[ vulkan ] Extension names ptr must be null; will be overwritten.\n");
    uAssertMsg_v(!(*instance_extension_properties), "[ vulkan ] VkExtensionProperties ptr must be null; will be overwritten.\n");
    if (num_user_instance_extension_names)
    {
        uAssertMsg_v(user_instance_extension_names && *user_instance_extension_names,
                     "[ vulkan ] If the instance extension quanitity is non zero, the names ptr must be non null\n");
    }


    if (!user_instance_extension_names)
    {
        return;
    }

    // Query Extension Count
    VkResult success = VK_SUCCESS;
    success = vkEnumerateInstanceExtensionProperties(NULL,
                                                     &instance_create_info->enabledExtensionCount,
                                                     NULL);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to enumerate layer properties.\n");
    }

    *instance_extension_properties =
        (VkExtensionProperties*)malloc(instance_create_info->enabledExtensionCount * sizeof(VkExtensionProperties));

    // Query Extension Names
    success = vkEnumerateInstanceExtensionProperties(NULL,
                                                     &instance_create_info->enabledExtensionCount,
                                                     *instance_extension_properties);

    if (success != VK_SUCCESS)
    {
        if(instance_extension_properties)
        {
            free(instance_extension_properties);
        }

        uFatal("[ vulkan ] Unable to enumerate layer properties.\n");
    }

    // Set Extension Names
    uVkVerbose("Searching for extensions...\n");
    u32 num_added_extensions = 0;
    *instance_extension_names = (s8**)malloc(instance_create_info->enabledExtensionCount * sizeof(s8**));
    for (u32 ext_idx = 0; ext_idx < instance_create_info->enabledExtensionCount; ext_idx++)
    {
        uVkVerbose("\tExtension found: %s\n", (*instance_extension_properties)[ext_idx].extensionName);
        for (u32 user_ext_idx = 0;
             user_ext_idx < num_user_instance_extension_names;
             user_ext_idx++)
        {
            if (strcmp((const char*)user_instance_extension_names[user_ext_idx],
                       (const char*)(*instance_extension_properties)[ext_idx].extensionName) == 0)
            {
                (*instance_extension_names)[num_added_extensions] = (*instance_extension_properties)[ext_idx].extensionName;
                num_added_extensions++;
            }
        }
    }

    if (num_added_extensions != num_user_instance_extension_names)
    {
        if(instance_extension_properties)
        {
            free(instance_extension_properties);
        }

        uFatal("[ vulkan ] Unable to load all requested extensions.\n");
    }

    instance_create_info->enabledExtensionCount   = num_added_extensions;
    instance_create_info->ppEnabledExtensionNames = *instance_extension_names;
}


__UE_internal__ void __UE_call__
uCreateVulkanInstance(const uVulkanInfo*       const       restrict v_info,
                      const VkApplicationInfo* const       restrict application_info,
                      const s8**               const const restrict user_instance_validation_layer_names,
                      const u16                                     num_user_instance_validation_layer_names,
                      const s8**               const const restrict user_instance_extension_names,
                      const u16                                     num_user_instance_extension_names)

{
    uAssertMsg_v(v_info,           "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(application_info, "[ vulkan ] VkApplicationInfo ptr must be non null.\n");
    if (num_user_instance_validation_layer_names)
    {
        uAssertMsg_v(user_instance_validation_layer_names && *user_instance_validation_layer_names,
                     "[ vulkan ] If the instance layer quanitity is non zero, the names ptr must be non null.\n");
    }
    if (num_user_instance_extension_names)
    {
        uAssertMsg_v(user_instance_extension_names && *user_instance_extension_names,
                     "[ vulkan ] If the instance extension quanitity is non zero, the names ptr must be non null.\n");
    }


    VkExtensionProperties* instance_extension_properties        = NULL;
    VkLayerProperties*     instance_validation_layer_properties = NULL;
    s8**                   instance_extension_names             = NULL;
    s8**                   instance_validation_layer_names      = NULL;

#if __UE_debug__ == 1
    uCreateVulkanDebugMessengerInfo(&vulkan_setup_debug_messenger_info);
#endif // __UE_debug__ == 1

    VkResult success = VK_SUCCESS;
    VkInstanceCreateInfo instance_create_info = { 0 };

    // Query extensions; store in VkInstanceCreateInfo
    uQueryVulkanInstanceExtensions(&instance_extension_names,
                                   &instance_extension_properties,
                                   &instance_create_info,
                                   user_instance_extension_names,
                                   num_user_instance_extension_names);

    // Query layers; store in VkInstanceCreateInfo
    uQueryVulkanInstanceLayers(&instance_validation_layer_names,
                               &instance_validation_layer_properties,
                               &instance_create_info,
                               user_instance_validation_layer_names,
                               num_user_instance_validation_layer_names);

    instance_create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = application_info;
#if __UE_debug__ == 1
    instance_create_info.pNext            = &vulkan_setup_debug_messenger_info;
#endif // __UE_debug__ == 1
    success = vkCreateInstance(&instance_create_info,
                               NULL,
                               (VkInstance*)&v_info->instance);

    if (success != VK_SUCCESS)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create vulkan instance.\n");
    }

#if __UE_debug__ == 1
    uCreateVulkanDebugMessenger(v_info,
                                (VkDebugUtilsMessengerCreateInfoEXT*)&vulkan_main_debug_messenger_info,
                                (VkDebugUtilsMessengerEXT*)&vulkan_main_debug_messenger);
#endif // __UE_debug__ == 1

    if (instance_extension_names)
    {
        free(instance_extension_names);
    }

    if (instance_validation_layer_names)
    {
        free(instance_validation_layer_names);
    }

    if (instance_extension_properties)
    {
        free(instance_extension_properties);
    }

    if (instance_validation_layer_properties)
    {
        free(instance_validation_layer_properties);
    }
}


__UE_internal__ void __UE_call__
uCreateVulkanApplicationInfo(const s8*                const restrict application_name,
                             _mut_ VkApplicationInfo* const restrict application_info)
{
    uAssertMsg_v(application_name, "[ vulkan ] Application names ptr must be non null.\n");
    uAssertMsg_v(application_info, "[ vulkan ] Application info ptr must be non null.\n");


    application_info->sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info->pApplicationName   = application_name;
    application_info->applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    application_info->pEngineName        = uGetEngineName();
    application_info->apiVersion         = VK_API_VERSION_1_0;
}


__UE_internal__ void __UE_call__
uInitializeVulkan(const uVulkanDrawTools* const       restrict return_draw_tools,
                  const s8**              const const restrict user_instance_validation_layer_names,
                  const u16                                    num_user_instance_validation_layer_names ,
                  const s8**              const const restrict user_instance_extension_names,
                  const u16                                    num_user_instance_extension_names,
                  const s8**              const const restrict user_device_extension_names,
                  const u16                                    num_user_device_extension_names)
{
    if (num_user_instance_validation_layer_names)
    {
        uAssertMsg_v(user_instance_validation_layer_names && *user_instance_validation_layer_names,
                     "[ vulkan ] If the instance layer quanitity is non zero, the names ptr must be non null.\n");
    }
    if (num_user_instance_extension_names)
    {
        uAssertMsg_v(user_instance_extension_names && *user_instance_extension_names,
                     "[ vulkan ] If the instance extension quanitity is non zero, the names ptr must be non null.\n");
    }
    if (num_user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, the names ptr must be non null.\n");
    }

    VkApplicationInfo   application_info = { 0 };
    uVulkanInfo*        v_info           = (uVulkanInfo*)uGetVulkanInfo();
    uVulkanQueueInfo*   queue_info       = (uVulkanQueueInfo*)uGetVulkanQueueInfo();
    uVulkanSurfaceInfo* surface_info     = (uVulkanSurfaceInfo*)uGetVulkanSurfaceInfo();
    uVulkanImageGroup*  image_group      = (uVulkanImageGroup*)uGetVulkanImageGroup();
    uVulkanRenderInfo*  render_info      = (uVulkanRenderInfo*)uGetVulkanRenderInfo();
    uVulkanCommandInfo* command_info     = (uVulkanCommandInfo*)uGetVulkanCommandInfo();

    uAssertMsg_v(v_info,       "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(queue_info,   "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(image_group,  "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");
    uAssertMsg_v(render_info,  "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");
    uAssertMsg_v(command_info, "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");


    uCreateVulkanApplicationInfo(uGetGameTitle(), &application_info);

    uCreateVulkanInstance(v_info,
                          &application_info,
                          user_instance_validation_layer_names,
                          num_user_instance_validation_layer_names,
                          user_instance_extension_names,
                          num_user_instance_extension_names);

    uCreateVulkanSurface(v_info, surface_info);

    // queue_info built
    // surface_info_built
    uCreateVulkanPhysicalDevice(v_info,
                                queue_info,
                                user_device_extension_names,
                                num_user_device_extension_names,
                                surface_info);

    uCreateVulkanLogicalDevice(v_info,
                               queue_info,
                               user_instance_validation_layer_names,
                               num_user_instance_validation_layer_names,
                               user_device_extension_names,
                               num_user_device_extension_names);

    // image_group partially built
    uCreateVulkanSwapChain(v_info,
                           surface_info,
                           queue_info,
                           image_group);

    // image_group partially built
    uCreateVulkanImageViews(v_info, image_group, surface_info);

    // render_info partially built
    uCreateVulkanRenderPass(v_info, surface_info, render_info);

    // render_info partially built
    uCreateVulkanGraphicsPipeline(v_info, surface_info, render_info);

    uCreateVulkanFrameBuffers(v_info, surface_info, image_group, render_info);

    // [ cfarvin::REMOVE ]
    /* uCreateVulkanCommandPool(v_info, queue_info, command_info); */
    uCreateVulkanCommandPool(v_info, command_info);

    uCreateVulkanCommandBuffers(v_info,
                                command_info,
                                image_group,
                                render_info,
                                surface_info);

    uCreateVulkanDrawSyncTools(v_info, (uVulkanDrawTools*)return_draw_tools);

    // DrawTool creation
    uVulkanDrawTools* non_const_draw_tools = *(uVulkanDrawTools**)&return_draw_tools;

    for (u8 frame_idx = 0; frame_idx < uVULKAN_MAX_FRAMES_IN_FLIGHT; frame_idx++)
    {
        non_const_draw_tools->stage_flags[frame_idx] =
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }

    non_const_draw_tools->logical_device  = v_info->logical_device;
    non_const_draw_tools->swap_chain      = v_info->swap_chain;
    non_const_draw_tools->graphics_queue  = queue_info->queues[uVULKAN_GRAPHICS_QUEUE_INDEX];
    non_const_draw_tools->present_queue   = queue_info->queues[uVULKAN_PRESENT_QUEUE_INDEX];

    VkSubmitInfo submit_info = { 0 };
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.waitSemaphoreCount   = 1;
    submit_info.pWaitDstStageMask    = non_const_draw_tools->stage_flags;
    submit_info.commandBufferCount   = 1;
    submit_info.signalSemaphoreCount = 1;

    non_const_draw_tools->submit_info = submit_info;
    memcpy(&(non_const_draw_tools->command_buffers[0]),
           command_info->command_buffers,
           uVULKAN_NUM_COMMAND_BUFFERS * sizeof(VkCommandBuffer));

    VkPresentInfoKHR present_info = { 0 };
    present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.waitSemaphoreCount = 1;
    present_info.swapchainCount     = 1;
    present_info.pSwapchains        = &(non_const_draw_tools->swap_chain);
    present_info.pResults           = NULL;

    non_const_draw_tools->present_info = present_info;
}


__UE_internal__ void __UE_call__
uDestroyVulkan()
{
    uVulkanInfo* v_info = (uVulkanInfo*)uGetVulkanInfo();
    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");

    // Wait for device to be idle
    if (v_info->logical_device)
    {
        vkDeviceWaitIdle(v_info->logical_device);
    }

    uDestroyVulkanImageGroup();
    uDestroyVulkanSurfaceInfo();
    uDestroyVulkanCommandInfo();
    uDestroyVulkanQueueInfo();
    uDestroyVulkanRenderInfo();
    uDestroyVulkanInfo();
}

#endif // __UE_VULKAN_TOOLS_H__

#if 0
/*

  To Draw A Triangle:
  - Create a VkInstance
  - Select a supported graphics card (VkPhysicalDevice)
  - Create a VkDevice and VkQueue for drawing and presentation
  - Create a window, window surface and swap chain
  - Wrap the swap chain images into VkImageView
  - Create a render pass that specifies the render targets and usage
  - Create framebuffers for the render pass
  - Set up the graphics pipeline
  - Allocate and record a command buffer with the draw commands for every possible swap chain image
  - Draw frames by acquiring images, submitting the right draw command buffer and returning the images back to the swap chain

  NOTES:
  - Skip window manager (or build a cusom one) with VK_KHR_display && VK_KHR_display_swapchain
  This will render fullscreen.

*/
#endif // if 0
