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
// [ begin ] Prime uVulkanSwapChainInfo
typedef struct
{
    const VkSurfaceFormatKHR*      surface_formats;
    const VkPresentModeKHR*        present_modes;
    const VkSurfaceCapabilitiesKHR surface_capabilities;
    const VkExtent2D               swap_extent;
    const u32                      num_surface_formats;
    const u32                      designated_format_index;
    const u32                      designated_present_index;
    const u32                      num_present_modes;
} uVulkanSwapChainInfo;
__UE_singleton__ uVulkanSwapChainInfo* uAPI_PRIME_VULKAN_SWAP_CHAIN_INFO = NULL;
__UE_internal__ __UE_inline__ const uVulkanSwapChainInfo* const
uGetVulkanSwapChainInfo()
{
    if (!uAPI_PRIME_VULKAN_SWAP_CHAIN_INFO)
    {
        *(uVulkanSwapChainInfo**)&uAPI_PRIME_VULKAN_SWAP_CHAIN_INFO =
            calloc(1, sizeof(uVulkanSwapChainInfo));
    }

    return uAPI_PRIME_VULKAN_SWAP_CHAIN_INFO;
}
// [ end ] Prime uVulkanSwapChainInfo
//

//
// [ begin ] Prime uVulkanQueueInfo
// Note: as queue indices are added, ensure that unique value extraction
//       in uCreateVulkanLogicalDevice() is updated as well.
typedef struct
{
    u32 graphics_index;
    u32 present_index;
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
#define uVULKAN_NUM_QUEUES           2
#define uVULKAN_GRAPHICS_QUEUE_INDEX 0
#define uVULKAN_PRESENT_QUEUE_INDEX  1
typedef struct
{
    const VkInstance       instance;
    const VkPhysicalDevice physical_device;
    const VkDevice         logical_device;
    const VkQueue          queues[uVULKAN_NUM_QUEUES];
    const VkSurfaceKHR     surface;
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
typedef struct
{
    _mut_ VkImage*     images;
    _mut_ VkImageView* image_views;
    const u32          num_images;
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


VkDebugUtilsMessengerEXT           vulkan_main_debug_messenger;
VkDebugUtilsMessengerCreateInfoEXT vulkan_main_debug_messenger_info  = { 0 };
VkDebugUtilsMessengerCreateInfoEXT vulkan_setup_debug_messenger_info = { 0 };


//
// [ begin ] Forward decls
__UE_internal__ __UE_call__ void
uDestroyVulkan();

__UE_internal__ __UE_call__ void
uQueryVulkanDeviceExtensions(const VkPhysicalDevice*             restrict physical_device,
                             const s8**              const const restrict user_device_extension_names,
                             const u16                                    num_user_device_extension_names,
                             _mut_ u16*              const       restrict num_verified_extension_names);

__UE_internal__ __UE_call__ bool
uSelectVulkanSwapChain(_mut_ uVulkanSwapChainInfo* const restrict swap_chain_info);
// [ end ] Forward decls
//


__UE_internal__ __UE_call__ void
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

__UE_internal__ __UE_call__ void
uReadSpirvFile(const char*   const       restrict file_name,
               _mut_ char**  _mut_ const restrict return_file_data,
               _mut_ size_t*             restrict return_file_size)
{
    uAssertMsg_v(file_name,             "[ vulkan ] File name ptr must be non null.\n");
    uAssertMsg_v(!(*return_file_data),  "[ vulkan ] Return file bytes ptr must be null; will be overwritten.\n");
    uAssertMsg_v(return_file_size,      "[ vulkan ] Return file size ptr must be non null");


    FILE* spir_v_file = NULL;
    fopen_s(&spir_v_file, file_name, "r");
    if (!spir_v_file)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to open SPIR-V file: %s.\n", file_name);
    }

    s32 fseek_success = fseek(spir_v_file, 0L, SEEK_END);
    if (fseek_success) // Success == 0
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to seek within SPIR-V file: %s.\n", file_name);
    }

    *return_file_size = (size_t)ftell(spir_v_file);
    if (!(*return_file_size))
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] SPIR-V file: %s has zero size.\n", file_name);
    }

    *return_file_data = (char*)calloc(*return_file_size, sizeof(char));
    if (!(*return_file_data))
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to allocate host memory for SPIR-V file: %s.\n" , file_name);
    }

    size_t items_read = fread_s((void*)(*return_file_data), // buffer
                                (*return_file_size),        // buffer size
                                1,                          // element size
                                (*return_file_size),        // element count
                                spir_v_file);               // FILE*

    if (items_read == 0)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to verify reading of SPIR-V file: %s.\n" , file_name);
    }

    fclose(spir_v_file);
}

__UE_internal__ __UE_call__ void
uCreateVulkanGraphicsPipeline(const uVulkanInfo* const restrict v_info)
{
    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkPhysical device must be non zero");


    // [ cfarvin::TODO ] Modules below are hard coded for now
    uVkVerbose("[ vulkan ] Loading shader files...\n");

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

    // Cleanup
    vkDestroyShaderModule(v_info->logical_device, vkTriangle_vert_module, NULL);
    vkDestroyShaderModule(v_info->logical_device, vkTriangle_frag_module, NULL);
}


__UE_internal__ __UE_call__ void
uCreateVulkanImageViews(const uVulkanInfo*          const restrict v_info,
                        _mut_ uVulkanImageGroup*    _mut_ restrict image_group,
                        const uVulkanSwapChainInfo* const restrict swap_chain_info)
{
    uAssertMsg_v(v_info,                           "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device,           "[ vulkan ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(image_group,                      "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");
    uAssertMsg_v(image_group->images,              "[ vulkan ] VkImages ptr must be non null.\n");
    uAssertMsg_v(image_group->num_images,          "[ vulkan ] VkImage quantity must be non zero.\n");
    uAssertMsg_v(image_group->image_views,         "[ vulkan ] VkImages ptr must be non null.\n");
    uAssertMsg_v(swap_chain_info,                  "[ vulkan ] uVulkanSwapChainInfo must be non null.\n");
    uAssertMsg_v(swap_chain_info->surface_formats, "[ vulkan ] VkSurfaceFormatKHR ptr must be non null.\n");


    VkSurfaceFormatKHR* surface_formats = (VkSurfaceFormatKHR*)swap_chain_info->surface_formats;
    for (u32 image_idx = 0; image_idx < image_group->num_images; image_idx++)
    {
        // Fill create info
        VkImageViewCreateInfo image_view_create_info = { 0 };

        image_view_create_info.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.image    = (image_group->images)[image_idx];
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format   = surface_formats[swap_chain_info->designated_format_index].format;

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


__UE_internal__ __UE_call__ void
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
                unique_queue_array[(*unique_queues_found)] = queue_info->graphics_index;
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
                    if (queue_info->present_index == unique_queue_array[unique_queue_idx])
                    {
                        is_unique = false;
                        break;
                    }
                }

                if (is_unique)
                {
                    unique_queue_array[(*unique_queues_found)] = queue_info->present_index;
                    (*unique_queues_found)++;
                }
            }
        }
    }
}


__UE_internal__ __UE_inline__ void
uCreateVulkanSwapChain(_mut_ uVulkanInfo*          const restrict v_info,
                       _mut_ uVulkanSwapChainInfo* const restrict swap_chain_info,
                       const uVulkanQueueInfo*     const restrict queue_info,
                       _mut_ uVulkanImageGroup*    _mut_ restrict image_group)
{
    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->surface,        "[ vulkan ] VkSurfaceKHR ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkLogicalDevice must be non zero.\n");
    uAssertMsg_v(!v_info->swap_chain,    "[ vulkan ] VkSwapChainKHR must be zero; will be overwritten.\n");
    uAssertMsg_v(swap_chain_info,        "[ vulkan ] uVulkanSwapChainInfo ptr must be non null.\n");
    uAssertMsg_v(queue_info,             "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(swap_chain_info->surface_formats,
                 "[ vulkan ] VkSurfaceFormatKHR ptr must be non null.\n");
    uAssertMsg_v(swap_chain_info->present_modes,
                 "[ vulkan ] VkPresentModeKHR ptr must be non null.\n");


    // Select a suitable swapchain
    bool swap_chain_selected = uSelectVulkanSwapChain(swap_chain_info);
    if (!swap_chain_selected)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to select a suitable swap chain.\n");
    }

    // Determine swap chain image capacity
    u32 min_image_count = swap_chain_info->surface_capabilities.minImageCount;
    u32 max_image_count = swap_chain_info->surface_capabilities.maxImageCount;

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

    const VkSurfaceFormatKHR* surface_formats = swap_chain_info->surface_formats;
    const VkPresentModeKHR*   present_modes = swap_chain_info->present_modes;

    // File create info
    VkSwapchainCreateInfoKHR swap_chain_create_info = { 0 };
    swap_chain_create_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swap_chain_create_info.surface          = v_info->surface;
    swap_chain_create_info.minImageCount    = designated_image_count;
    swap_chain_create_info.imageFormat      = surface_formats[swap_chain_info->designated_format_index].format;
    swap_chain_create_info.imageColorSpace  = surface_formats[swap_chain_info->designated_format_index].colorSpace;
    swap_chain_create_info.imageExtent      = swap_chain_info->swap_extent;
    swap_chain_create_info.imageArrayLayers = 1;
    swap_chain_create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // Render directly to images
    swap_chain_create_info.preTransform     = swap_chain_info->surface_capabilities.currentTransform;
    swap_chain_create_info.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Ignore alpha channel
    swap_chain_create_info.presentMode      = present_modes[swap_chain_info->designated_present_index];
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
        free((VkSurfaceFormatKHR*)swap_chain_info->surface_formats);
        free((VkPresentModeKHR*)swap_chain_info->present_modes);
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


__UE_internal__ __UE_call__ void
uCreateVulkanLogicalDevice(_mut_ uVulkanInfo*      const       restrict v_info,
                           const uVulkanQueueInfo* const       restrict queue_info,
                           const s8**              const const restrict instance_validation_layer_names,
                           const u16                                    num_instance_validation_layer_names,
                           const s8**              const const restrict user_device_extension_names,
                           const u16                                    num_user_device_extension_names)
{
    uAssertMsg_v(v_info,                   "[ vulkan ] Null vulkan info ptr provided.\n");
    uAssertMsg_v(v_info->physical_device,  "[ vulkan ] Physical device must be non null.\n");
    uAssertMsg_v(!v_info->logical_device,  "[ vulkan ] Logical device must be null; will be overwritten.\n");
    uAssertMsg_v(v_info->queues,           "[ vulkan ] Queue array must be non null.\n");
    uAssertMsg_v(queue_info,               "[ vulkan ] Queue indices ptr must be non null\n");
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
                     queue_info->graphics_index,
                     0,
                     (VkQueue*)(&(v_info->queues)[uVULKAN_GRAPHICS_QUEUE_INDEX]));

    vkGetDeviceQueue(v_info->logical_device,
                     queue_info->present_index,
                     0,
                     (VkQueue*)(&(v_info->queues)[uVULKAN_PRESENT_QUEUE_INDEX]));

    if (device_queue_create_infos)
    {
        free(device_queue_create_infos);
    }
}


__UE_internal__ __UE_call__ bool
uValidateVulkanDeviceFeaturesReqruirement(const VkPhysicalDevice physical_device)
{
    if (physical_device) {}

    // Aquire physical device features
    /* VkPhysicalDeviceFeatures device_features; */
    /* vkGetPhysicalDeviceFeatures(physical_device, */
    /*                             &device_features); */

    return true;
}


__UE_internal__ __UE_call__ bool
uValidateVulkanDevicePropertiesReqruirement(const VkPhysicalDevice physical_device)
{
    if (physical_device) {}

    // Aquire physical device properties
    /* VkPhysicalDeviceProperties device_properties; */
    /* vkGetPhysicalDeviceProperties(physical_device, */
    /*                               &device_properties); */

    return true;
}


__UE_internal__ __UE_call__ bool
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


__UE_internal__ __UE_call__ bool
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
            queue_info->graphics_index = graphics_pair.index;
            queue_info->present_index  = present_pair.index;

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
uSelectVulkanSwapChainExtent(_mut_ uVulkanSwapChainInfo* const restrict swap_chain_info)
{
    uAssertMsg_v(swap_chain_info,    "[ vulkan ] uVulkanSwapChainInfo ptr must be non null.\n");
#if _WIN32
    uAssertMsg_v(win32_info,         "[ win32 ] [ vulkan ] uWin32Info must be non null.\n");
    uAssertMsg_v(win32_info->window, "[ win32 ] [ vulkan ] uWin32Info window must be non null.\n");
#endif // _WIN32

    VkSurfaceCapabilitiesKHR surface_capabilities = swap_chain_info->surface_capabilities;
    VkExtent2D* non_const_extent = (VkExtent2D*)&(swap_chain_info->swap_extent);

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


__UE_internal__ __UE_call__ bool
uSelectVulkanSwapChain(_mut_ uVulkanSwapChainInfo* const restrict swap_chain_info)
{
    uAssertMsg_v(swap_chain_info,                "[ vulkan ] uVulkanSwapChainInfo ptr must be non null.\n");
    if (swap_chain_info->num_surface_formats)
    {
        uAssertMsg_v(swap_chain_info->surface_formats, "[ vulkan ] VkSurfaceFormatKHR ptr must be non null.\n");
    }
    if (swap_chain_info->num_present_modes)
    {
        uAssertMsg_v(swap_chain_info->present_modes,"[ vulkan ] VkPresentModeKHR ptr must be non null.\n");
    }

    // Determine best available surface format
    u32*  non_const_designated_format_index = (u32*)&(swap_chain_info->designated_format_index);
    bool optimal_format_found = false;
    for (u16 format_idx = 0; format_idx < swap_chain_info->num_surface_formats; format_idx++)
    {
        if (swap_chain_info->surface_formats[format_idx].format == VK_FORMAT_B8G8R8A8_SRGB &&
            swap_chain_info->surface_formats[format_idx].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
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
    u32* non_const_designated_present_index = (u32*)&(swap_chain_info->designated_present_index);
    bool suitable_present_found   = false;
    bool optimal_present_found    = false;
    u16  fifo_present_index       = 0;
    for (u16 present_idx = 0; present_idx < swap_chain_info->num_present_modes; present_idx++)
    {
        VkPresentModeKHR present_mode = swap_chain_info->present_modes[present_idx];
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
    uSelectVulkanSwapChainExtent(swap_chain_info);

    // Note: require FIFO present mode; accept any format
    return suitable_present_found;
}


__UE_internal__ __UE_call__ bool
uValidateVulkanSwapChainAndSurfaceCompatibility(const VkPhysicalDevice                     physical_device,
                                                const VkSurfaceKHR*         const restrict surface,
                                                _mut_ uVulkanSwapChainInfo* const restrict return_swap_chain_info)
{
    uAssertMsg_v(surface,                "[ vulkan ] The VKSurfaceKHR ptr must be non null.\n");
    uAssertMsg_v(return_swap_chain_info, "[ vulkan ] The uVulkanSwapChainInfo ptr must be non null.\n");


    // Get surface capabilities
    VkSurfaceCapabilitiesKHR* surface_capabilities = (VkSurfaceCapabilitiesKHR*)&(return_swap_chain_info->surface_capabilities);
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
                                                   (u32*)&(return_swap_chain_info->num_surface_formats),
                                                   NULL);

    uAssert(success == VK_SUCCESS);
    if (success != VK_SUCCESS)
    {
        uVkVerbose("Could not attain surface format quantity.\n");
        return false;
    }


    if (return_swap_chain_info->num_surface_formats)
    {
        // Note: does not free in this scope.
        return_swap_chain_info->surface_formats = (VkSurfaceFormatKHR*)malloc(return_swap_chain_info->num_surface_formats *
                                                                              sizeof(VkSurfaceFormatKHR));
        success = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device,
                                                       *surface,
                                                       (u32*)&(return_swap_chain_info->num_surface_formats),
                                                       (VkSurfaceFormatKHR*)return_swap_chain_info->surface_formats);

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
                                                        (u32*)&(return_swap_chain_info->num_present_modes),
                                                        NULL);
    uAssert(success == VK_SUCCESS);
    if (success != VK_SUCCESS)
    {
        uVkVerbose("Could not attain present mode quantity.\n");
        return false;
    }

    if (return_swap_chain_info->num_present_modes)
    {
        // Note: does not free in this scope
        return_swap_chain_info->present_modes = (VkPresentModeKHR*)malloc(return_swap_chain_info->num_present_modes *
                                                                          sizeof(VkPresentModeKHR));

        success = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
                                                            *surface,
                                                            (u32*)&(return_swap_chain_info->num_present_modes),
                                                            (VkPresentModeKHR*)return_swap_chain_info->present_modes);
        uAssert(success == VK_SUCCESS);
        if (success != VK_SUCCESS)
        {
            uVkVerbose("Could not attain present modes.\n");
            return false;
        }
    }

    return (return_swap_chain_info->num_surface_formats &&
            return_swap_chain_info->num_present_modes);
}


__UE_internal__ __UE_call__ void
uSelectVulkanPhysicalDevice(const VkPhysicalDevice**    const const restrict physical_device_list,
                            _mut_ VkPhysicalDevice*     const       restrict return_device,
                            _mut_ uVulkanQueueInfo*     const       restrict queue_info,
                            const u32                                        num_physical_devices,
                            const VkSurfaceKHR*         const       restrict surface,
                            const s8**                  const const restrict user_device_extension_names,
                            const u16                                        num_user_device_extension_names,
                            _mut_ uVulkanSwapChainInfo* const       restrict return_swap_chain_info)
{
    uAssertMsg_v(physical_device_list,   "[ vulkan ] Null vulkan device list pointer provided.\n");
    uAssertMsg_v(!(*return_device),      "[ vulkan ] Return device must be null; will be overwritten\n");
    uAssertMsg_v(queue_info,             "[ vulkan ] Queue family indices ptr must be non null\n");
    uAssertMsg_v(num_physical_devices,   "[ vulkan ] A minimum of one physical devices is required.\n");
    uAssertMsg_v(surface,                "[ vulkan ] Surface must be non null.\n");
    uAssertMsg_v(return_swap_chain_info, "[ vulkan ] Swap chain info ptr must be non null.\n");
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
                                                            return_swap_chain_info);

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


__UE_internal__ __UE_call__ void
uCreateVulkanPhysicalDevice(_mut_ uVulkanInfo*          const       restrict v_info,
                            _mut_ uVulkanQueueInfo*     const       restrict queue_info,
                            const s8**                  const const restrict user_device_extension_names,
                            const u16                                        num_user_device_extension_names,
                            _mut_ uVulkanSwapChainInfo* const       restrict return_swap_chain_info)
{
    uAssertMsg_v(v_info,                   "[ vulkan ] Null vulkan info ptr provided.\n");
    uAssertMsg_v(!v_info->physical_device, "[ vulkan ] Physical device must be null; will be overwritten.\n");
    uAssertMsg_v(!v_info->logical_device,  "[ vulkan ] Logical device must be null; will be overwritten.\n");
    uAssertMsg_v(queue_info,               "[ vulkan ] Queue family indices ptr must be non null.\n");
    uAssertMsg_v(return_swap_chain_info,   "[ vulkan ] Swap chain info ptr must be non null.\n");

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
                                (VkSurfaceKHR*)&v_info->surface,
                                user_device_extension_names,
                                num_user_device_extension_names,
                                return_swap_chain_info);

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


// Note: no function/argument decorations to conform w/ Vulkan spec.
static VKAPI_ATTR VkBool32 VKAPI_CALL
uVkDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity_bits,
                 VkDebugUtilsMessageTypeFlagsEXT             message_type_bits,
                 const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                 void*                                       user_data)
{
    VkBool32 should_abort_calling_process = VK_FALSE;
    if(user_data) {} // Silence warnings

    if (message_severity_bits >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ||
        message_type_bits     >= VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
    {
        printf("[ vulkan ] [ validation ] %s\n", callback_data->pMessage);
        fflush(stdout);
    }

    return should_abort_calling_process;
}


__UE_internal__ __UE_call__ void
uCreateVulkanDebugMessengerInfo(_mut_ VkDebugUtilsMessengerCreateInfoEXT* const restrict debug_message_create_info)
{
    uAssertMsg_v(debug_message_create_info, "[ vulkan ] Null VkDebugUtilsMessengerCreateInfoEXT ptr provided.\n");


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


__UE_internal__ __UE_call__ void
uCreateVulkanDebugMessenger(const uVulkanInfo*                        const restrict v_info,
                            _mut_ VkDebugUtilsMessengerCreateInfoEXT* const restrict debug_message_create_info,
                            _mut_ VkDebugUtilsMessengerEXT*           const restrict debug_messenger)
{
    uAssertMsg_v(v_info,                    "[ vulkan ] Null uVulkanInfo ptr provided.\n");
    uAssertMsg_v(v_info->instance,          "[ vulkan ] Null uVulkanInfo->instance ptr provided.\n");
    uAssertMsg_v(debug_message_create_info, "[ vulkan ] Null VkDebugUtilsMessengerCreateInfoEXT ptr provided.\n");


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


#if _WIN32
__UE_internal__ __UE_call__ void
uCreateWin32Surface(_mut_ uVulkanInfo* const restrict v_info)
{
    uAssertMsg_v(v_info,           "[ vulkan ] Null uVulkanInfo ptr provided.\n");
    uAssertMsg_v(v_info,           "[ vulkan ] Null uVulkanInfo ptr provided.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] Null uVulkanInfo->instance ptr provided.\n");


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
                                                            (VkSurfaceKHR*)&v_info->surface);

    if (win32_surface_result != VK_SUCCESS)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create win32 surface.\n");
    }
}
#endif // _WIN32


__UE_internal__ __UE_call__ void
uCreateVulkanSurface(_mut_ uVulkanInfo* const restrict v_info)
{
    uAssertMsg_v(v_info,           "[ vulkan ] Null uVulkanInfo ptr provided.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] Null uVulkanInfo->instance ptr provided.\n");

#if _WIN32
    uCreateWin32Surface(v_info);
#endif // _WIN32
}


__UE_internal__ __UE_call__ void
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


__UE_internal__ __UE_call__ void
uQueryVulkanInstanceLayers(_mut_ s8***                 _mut_ _mut_ const restrict instance_validation_layer_names,
                           _mut_ VkLayerProperties**   const _mut_       restrict instance_validation_layer_properties,
                           _mut_ VkInstanceCreateInfo* const             restrict instance_create_info,
                           const s8**                  const       const restrict user_instance_validation_layer_names,
                           const u32                                              num_user_instance_validation_layer_names)
{
    uAssertMsg_v(instance_create_info,                  "[ vulkan ] Null InstanceCreateInfo ptr provided.\n");
    uAssertMsg_v(!(*instance_validation_layer_names),   "[ vulkan ] Layer names ptr ptr must be null; will be overwritten.\n");
    uAssertMsg_v(!(*instance_validation_layer_properties), "[ vulkan ] VkLayerProperties ptr must be null; will be overwritten.\n");
    uAssertMsg_v(user_instance_validation_layer_names,  "[ vulkan ] Null requested validation layer ptr.\n");
    if (num_user_instance_validation_layer_names)
    {
        uAssertMsg_v(user_instance_validation_layer_names && *user_instance_validation_layer_names,
                     "[ vulkan ] If the instance layer quanitity is non zero, the names ptr must be non null\n");
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


__UE_internal__ __UE_call__ void
uQueryVulkanInstanceExtensions(const s8***                   _mut_ _mut_ const restrict instance_extension_names,
                               _mut_ VkExtensionProperties** _mut_ const       restrict instance_extension_properties,
                               _mut_ VkInstanceCreateInfo*   const             restrict instance_create_info,
                               const s8**                    const       const restrict user_instance_extension_names,
                               const u16                                               num_user_instance_extension_names)
{
    uAssertMsg_v(instance_create_info,              "[ vulkan ] Null InstanceCreateInfo ptr provided.\n");
    uAssertMsg_v(!(*instance_extension_names),      "[ vulkan ] Extension names ptr ptr must be null; will be overwritten.\n");
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


__UE_internal__ __UE_call__ void
uCreateVulkanInstance(const uVulkanInfo*       const       restrict v_info,
                      const VkApplicationInfo* const       restrict application_info,
                      const s8**               const const restrict user_instance_validation_layer_names,
                      const u16                                     num_user_instance_validation_layer_names,
                      const s8**               const const restrict user_instance_extension_names,
                      const u16                                     num_user_instance_extension_names)

{
    uAssertMsg_v(v_info,           "[ vulkan ] Null Vulkan info ptr provided.\n");
    uAssertMsg_v(application_info, "[ vulkan ] Null application info ptr provided.\n");
    if (num_user_instance_validation_layer_names)
    {
        uAssertMsg_v(user_instance_validation_layer_names && *user_instance_validation_layer_names,
                     "[ vulkan ] If the instance layer quanitity is non zero, the names ptr must be non null\n");
    }
    if (num_user_instance_extension_names)
    {
        uAssertMsg_v(user_instance_extension_names && *user_instance_extension_names,
                     "[ vulkan ] If the instance extension quanitity is non zero, the names ptr must be non null\n");
    }


    VkExtensionProperties* instance_extension_properties        = NULL;
    VkLayerProperties*     instance_validation_layer_properties = NULL;
    s8**                   instance_extension_names             = NULL;
    s8**                   instance_validation_layer_names      = NULL;

    uCreateVulkanDebugMessengerInfo(&vulkan_setup_debug_messenger_info);

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
    instance_create_info.pNext            = &vulkan_setup_debug_messenger_info;
    success = vkCreateInstance(&instance_create_info,
                               NULL,
                               (VkInstance*)&v_info->instance);

    if (success != VK_SUCCESS)
    {
        uDestroyVulkan();
        uFatal("[ vulkan ] Unable to create vulkan instance.\n");
    }

    uCreateVulkanDebugMessenger(v_info,
                                (VkDebugUtilsMessengerCreateInfoEXT*)&vulkan_main_debug_messenger_info,
                                (VkDebugUtilsMessengerEXT*)&vulkan_main_debug_messenger);

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


__UE_internal__ __UE_call__ void
uCreateVulkanApplicationInfo(const s8*                const restrict application_name,
                             _mut_ VkApplicationInfo* const restrict application_info)
{
    uAssertMsg_v(application_name, "[ vulkan ] Null application names ptr provided.\n");
    uAssertMsg_v(application_info, "[ vulkan ] Null application info ptr provided.\n");


    application_info->sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info->pApplicationName   = application_name;
    application_info->applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    application_info->pEngineName        = uGetEngineName();
    application_info->apiVersion         = VK_API_VERSION_1_0;
}


__UE_internal__ __UE_call__ void
uInitializeVulkan(const s8** const const restrict user_instance_validation_layer_names,
                  const u16                       num_user_instance_validation_layer_names ,
                  const s8** const const restrict user_instance_extension_names,
                  const u16                       num_user_instance_extension_names,
                  const s8** const const restrict user_device_extension_names,
                  const u16                       num_user_device_extension_names)
{
    if (num_user_instance_validation_layer_names)
    {
        uAssertMsg_v(user_instance_validation_layer_names && *user_instance_validation_layer_names,
                     "[ vulkan ] If the instance layer quanitity is non zero, the names ptr must be non null\n");
    }
    if (num_user_instance_extension_names)
    {
        uAssertMsg_v(user_instance_extension_names && *user_instance_extension_names,
                     "[ vulkan ] If the instance extension quanitity is non zero, the names ptr must be non null\n");
    }
    if (num_user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, the names ptr must be non null\n");
    }


    VkApplicationInfo    application_info = { 0 };
    uVulkanInfo*          v_info           = (uVulkanInfo*)uGetVulkanInfo();
    uVulkanQueueInfo*     queue_info       = (uVulkanQueueInfo*)uGetVulkanQueueInfo();
    uVulkanSwapChainInfo* swap_chain_info  = (uVulkanSwapChainInfo*)uGetVulkanSwapChainInfo();
    uVulkanImageGroup*    image_group      = (uVulkanImageGroup*)uGetVulkanImageGroup();

    uCreateVulkanApplicationInfo(uGetGameTitle(), &application_info);

    uCreateVulkanInstance(v_info,
                          &application_info,
                          user_instance_validation_layer_names,
                          num_user_instance_validation_layer_names,
                          user_instance_extension_names,
                          num_user_instance_extension_names);

    uCreateVulkanSurface(v_info);

    // queue_info built
    // swap_chain_info_built
    uCreateVulkanPhysicalDevice(v_info,
                                queue_info,
                                user_device_extension_names,
                                num_user_device_extension_names,
                                swap_chain_info);

    // queue_info consumed
    uCreateVulkanLogicalDevice(v_info,
                               queue_info,
                               user_instance_validation_layer_names,
                               num_user_instance_validation_layer_names,
                               user_device_extension_names,
                               num_user_device_extension_names);

    // swap_chain_info consumed
    // queue_info consumed
    // image_group partially built
    uCreateVulkanSwapChain(v_info,
                           swap_chain_info,
                           queue_info,
                           image_group);

    // image_group partially built
    // swap_chain_info consumed
    uCreateVulkanImageViews(v_info,
                            image_group,
                            swap_chain_info);

    uCreateVulkanGraphicsPipeline(v_info);
}


__UE_internal__ __UE_call__ void
uDestroyVulkan()
{
    uVulkanInfo*          v_info           = (uVulkanInfo*)uGetVulkanInfo();
    uVulkanQueueInfo*     queue_info       = (uVulkanQueueInfo*)uGetVulkanQueueInfo();
    uVulkanSwapChainInfo* swap_chain_info  = (uVulkanSwapChainInfo*)uGetVulkanSwapChainInfo();
    uVulkanImageGroup*    image_group      = (uVulkanImageGroup*)uGetVulkanImageGroup();

    uAssertMsg_v(v_info,                 "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance,       "[ vulkan ] VkInstance ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(queue_info,             "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(swap_chain_info,        "[ vulkan ] uVulkanSwapChainInfo ptr must be non null.\n");
    uAssertMsg_v(image_group,            "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");

    // uVulkanImageGroup data
    if (image_group)
    {
        if (v_info->logical_device && image_group->image_views)
        {
            for (u32 image_view_idx = 0;
                 image_view_idx < image_group->num_images;
                 image_view_idx++)
            {
                VkImageView image_view = image_group->image_views[image_view_idx];
                if (image_view)
                {
                    vkDestroyImageView(v_info->logical_device,
                                       image_view,
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

        free(image_group);
    }

    // uVulkanInfo data
    if (v_info)
    {
        if (v_info->instance)
        {
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

            // Destroy swap chain
            vkDestroySwapchainKHR(v_info->logical_device, v_info->swap_chain, NULL);

            // Destroy surface khr
            vkDestroySurfaceKHR(v_info->instance, v_info->surface, NULL);
        }

        if (v_info->logical_device)
        {
            vkDestroyDevice(v_info->logical_device, NULL);

        }

        if (v_info->instance)
        {
            vkDestroyInstance(v_info->instance, NULL);
        }

        free(v_info);
    }

    // uVulkanQueueInfo data
    if (queue_info)
    {
        free(queue_info);
    }

    // uVulkanSwapChainInfo data
    if (swap_chain_info)
    {
        if (swap_chain_info->surface_formats)
        {
            free((VkSurfaceFormatKHR*)swap_chain_info->surface_formats);
        }

        if (swap_chain_info->present_modes)
        {
            free((VkPresentModeKHR*)swap_chain_info->present_modes);
        }

        free(swap_chain_info);
    }

    uDestroyWin32((uWin32Info* const)win32_info);
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
