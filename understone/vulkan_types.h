#ifndef __UE_VULKAN_TYPES_H__
#define __UE_VULKAN_TYPES_H__

#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif // _WIN32

#include "debug_tools.h"
#include "type_tools.h"

#include <vulkan/vulkan.h>

#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
VkDebugUtilsMessengerEXT           vulkan_main_debug_messenger;
VkDebugUtilsMessengerCreateInfoEXT vulkan_main_debug_messenger_info  = {};
VkDebugUtilsMessengerCreateInfoEXT vulkan_setup_debug_messenger_info = {};
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

typedef struct
{
    const VkCommandBuffer* command_buffers;
    const VkCommandPool    command_pool;
    const u32              num_command_buffers;
} uVulkanCommandInfo;
const uVulkanCommandInfo* uAPI_PRIME_VULKAN_COMMAND_INFO = NULL;

__UE_inline__ const uVulkanCommandInfo*
                    uGetVulkanCommandInfo()
{
    if (!uAPI_PRIME_VULKAN_COMMAND_INFO)
    {
        *( uVulkanCommandInfo** )&uAPI_PRIME_VULKAN_COMMAND_INFO =
          ( uVulkanCommandInfo* )calloc(1, sizeof(uVulkanCommandInfo));
    }

    return uAPI_PRIME_VULKAN_COMMAND_INFO;
}

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

__UE_inline__ const uVulkanSurfaceInfo*
                    uGetVulkanSurfaceInfo()
{
    if (!uAPI_PRIME_VULKAN_SURFACE_INFO)
    {
        *( uVulkanSurfaceInfo** )&uAPI_PRIME_VULKAN_SURFACE_INFO =
          ( uVulkanSurfaceInfo* )calloc(1, sizeof(uVulkanSurfaceInfo));
    }

    return uAPI_PRIME_VULKAN_SURFACE_INFO;
}

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

__UE_inline__ const uVulkanQueueInfo*
                    uGetVulkanQueueInfo()
{
    if (!uAPI_PRIME_VULKAN_QUEUE_INFO)
    {
        *( uVulkanQueueInfo** )&uAPI_PRIME_VULKAN_QUEUE_INFO = ( uVulkanQueueInfo* )calloc(1, sizeof(uVulkanQueueInfo));
    }

    return uAPI_PRIME_VULKAN_QUEUE_INFO;
}

typedef struct
{
    const VkInstance       instance;
    const VkPhysicalDevice physical_device;
    const VkDevice         logical_device;
} uVulkanInfo;
const uVulkanInfo* uAPI_PRIME_VULKAN_INFO = NULL;

__UE_inline__ const uVulkanInfo*
                    uGetVulkanInfo()
{
    if (!uAPI_PRIME_VULKAN_INFO)
    {
        *( uVulkanInfo** )&uAPI_PRIME_VULKAN_INFO = ( uVulkanInfo* )calloc(1, sizeof(uVulkanInfo));
    }

    return uAPI_PRIME_VULKAN_INFO;
}

typedef struct
{
    VkImage*             images;
    VkImageView*         image_views;   // Note: To have num_images elements
    VkFramebuffer*       frame_buffers; // Note: To have num_images elements
    const u32            num_images;
    const VkSwapchainKHR swap_chain;
} uVulkanImageGroup;
const uVulkanImageGroup* uAPI_PRIME_VULKAN_IMAGE_GROUP = NULL;

__UE_inline__ const uVulkanImageGroup*
                    uGetVulkanImageGroup()
{
    if (!uAPI_PRIME_VULKAN_IMAGE_GROUP)
    {
        *( uVulkanImageGroup** )&uAPI_PRIME_VULKAN_IMAGE_GROUP =
          ( uVulkanImageGroup* )calloc(1, sizeof(uVulkanImageGroup));
    }

    return uAPI_PRIME_VULKAN_IMAGE_GROUP;
}

// Note: Completely destroys and frees the uVulkanQueueInfo pointer and it's applicable members.
__UE_inline__ void
uDestroyVulkanQueueInfo()
{
    uVulkanQueueInfo* queue_info = ( uVulkanQueueInfo* )uGetVulkanQueueInfo();
    uAssertMsg_v(queue_info, "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");

    if (queue_info)
    {
        free(queue_info);
    }
}

// Note: Frees the uVulkanQueueInfo pointer's applicable members and memsets the top-level handle.
__UE_inline__ void
uResetVulkanQueueInfo()
{
    uVulkanQueueInfo* queue_info = ( uVulkanQueueInfo* )uGetVulkanQueueInfo();
    uAssertMsg_v(queue_info, "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");

    if (queue_info)
    {
        memset(queue_info, 0, sizeof(uVulkanQueueInfo));
    }
}

__UE_inline__ void
uDestroyVulkanCommandBuffers()
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanCommandInfo* command_info = ( uVulkanCommandInfo* )uGetVulkanCommandInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(command_info, "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");

    if (command_info)
    {
        if (v_info)
        {
            vkFreeCommandBuffers(v_info->logical_device,
                                 command_info->command_pool,
                                 command_info->num_command_buffers,
                                 command_info->command_buffers);
        }

        if (command_info->command_buffers)
        {
            free(( void* )command_info->command_buffers);
        }
    }
}

// Note: Does not free or memset uVulkanCommandInfo pointer, but does free it's applicable members.
__UE_inline__ void
uClearVulkanCommandInfo()
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanCommandInfo* command_info = ( uVulkanCommandInfo* )uGetVulkanCommandInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(command_info, "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");

    if (command_info)
    {
        uDestroyVulkanCommandBuffers();

        if (v_info)
        {
            vkDestroyCommandPool(v_info->logical_device, command_info->command_pool, NULL);
        }
    }
}

// Note: Completely destroys and frees the uCommandRenderInfo pointer and it's applicable members.
__UE_inline__ void
uDestroyVulkanCommandInfo()
{
    uVulkanCommandInfo* command_info = ( uVulkanCommandInfo* )uGetVulkanCommandInfo();
    uAssertMsg_v(command_info, "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");

    uClearVulkanCommandInfo();

    if (command_info)
    {
        free(command_info);
    }
}

// Note: Frees the uVulkanCommandInfo pointer's applicable members and memsets the top-level handle.
__UE_inline__ void
uResetVulkanCommandInfo()
{
    uVulkanCommandInfo* command_info = ( uVulkanCommandInfo* )uGetVulkanCommandInfo();
    uAssertMsg_v(command_info, "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");

    uClearVulkanCommandInfo();

    if (command_info)
    {
        memset(command_info, 0, sizeof(uVulkanCommandInfo));
    }
}

// Note: Does not free or memset uVulkanSurfaceInfo pointer, but does free it's applicable members.
__UE_inline__ void
uClearVulkanSurfaceInfo()
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");

    if (surface_info)
    {
        if (v_info->instance)
        {
            vkDestroySurfaceKHR(v_info->instance, surface_info->surface, NULL);
        }

        if (surface_info->surface_formats)
        {
            free(( VkSurfaceFormatKHR* )surface_info->surface_formats);
        }

        if (surface_info->present_modes)
        {
            free(( VkPresentModeKHR* )surface_info->present_modes);
        }
    }
}

// Note: Completely destroys and frees the uVulkanSurfaceInfo pointer and it's applicable members.
__UE_inline__ void
uDestroyVulkanSurfaceInfo()
{
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");

    uClearVulkanSurfaceInfo();

    if (surface_info)
    {
        free(surface_info);
    }
}

// Note: Frees the uVulkanSurfaceInfo pointer's applicable members and memsets the top-level handle.
__UE_inline__ void
uResetVulkanSurfaceInfo()
{
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");

    uClearVulkanSurfaceInfo();

    if (surface_info)
    {
        memset(surface_info, 0, sizeof(uVulkanSurfaceInfo));
    }
}

// Note: Does not free or memset uVulkanInfo pointer, but does free it's applicable members.
__UE_inline__ void
uClearVulkanInfo()
{
    uVulkanInfo* v_info = ( uVulkanInfo* )uGetVulkanInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] VkInstance must be non zero.\n");

    if (v_info)
    {
        if (v_info->instance)
        {
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
            // Destroy debug messenger
            PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT =
              ( PFN_vkDestroyDebugUtilsMessengerEXT )vkGetInstanceProcAddr(v_info->instance,
                                                                           "vkDestroyDebugUtilsMessengerEXT");
            uAssertMsg_v(vkDestroyDebugUtilsMessengerEXT,
                         "[ vulkan ] Unable to acquire fnptr: "
                         "vkDestroyDebugUtilsMessengerEXT().\n");
            if (vkDestroyDebugUtilsMessengerEXT && vulkan_main_debug_messenger)
            {
                vkDestroyDebugUtilsMessengerEXT(v_info->instance, vulkan_main_debug_messenger, NULL);
            }
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
        }

        if (v_info->logical_device)
        {
            vkDestroyDevice(v_info->logical_device, NULL);
        }

        if (v_info->instance)
        {
            vkDestroyInstance(v_info->instance, NULL);
        }
    }
}

// Note: Completely destroys and frees the uVulkanInfo pointer and it's applicable members.
__UE_inline__ void
uDestroyVulkanInfo()
{
    uVulkanInfo* v_info = ( uVulkanInfo* )uGetVulkanInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] VkInstance must be non zero.\n");

    uClearVulkanInfo();

    if (v_info)
    {
        free(v_info);
    }
}

// Note: Frees the uVulkanInfo pointer's applicable members and memsets the top-level handle.
__UE_inline__ void
uResetVulkanInfo()
{
    uVulkanInfo* v_info = ( uVulkanInfo* )uGetVulkanInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] VkInstance must be non zero.\n");

    uClearVulkanInfo();

    if (v_info)
    {
        memset(v_info, 0, sizeof(uVulkanInfo));
    }
}

// Note: Does not free or memset uVulkanImageGroup pointer, but does free it's applicable members.
__UE_inline__ void
uClearVulkanImageGroup()
{
    uVulkanInfo*       v_info      = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanImageGroup* image_group = ( uVulkanImageGroup* )uGetVulkanImageGroup();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(image_group, "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");

    uDebugPrint("Destroying image group components...\n");
    if (image_group)
    {
        if (v_info && v_info->logical_device)
        {
            // Destroy image views, frame buffers
            for (u32 image_idx = 0; image_idx < image_group->num_images; image_idx++)
            {
                if (image_group->image_views)
                {
                    vkDestroyImageView(v_info->logical_device, image_group->image_views[image_idx], NULL);
                }

                if (image_group->frame_buffers)
                {
                    vkDestroyFramebuffer(v_info->logical_device, image_group->frame_buffers[image_idx], NULL);
                }
            }
        }

        if (image_group->swap_chain)
        {
            uDebugPrint("Destroying old swap chain...\n");
            vkDestroySwapchainKHR(v_info->logical_device, image_group->swap_chain, NULL);
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
    }
}

// Note: Completely destroys and frees the uVulkanImageGroup pointer and it's applicable members.
__UE_inline__ void
uDestroyVulkanImageGroup()
{
    uVulkanImageGroup* image_group = ( uVulkanImageGroup* )uGetVulkanImageGroup();
    uAssertMsg_v(image_group, "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");

    uClearVulkanImageGroup();

    if (image_group)
    {
        free(image_group);
    }
}

// Note: Frees the uVulkanImageGroup pointer's applicable members and memsets the top-level handle.
__UE_inline__ void
uResetVulkanImageGroup()
{
    uVulkanImageGroup* image_group = ( uVulkanImageGroup* )uGetVulkanImageGroup();
    uAssertMsg_v(image_group, "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");

    uClearVulkanImageGroup();

    if (image_group)
    {
        memset(image_group, 0, sizeof(uVulkanImageGroup));
    }
}

#endif // __UE_VULKAN_TYPES_H__
