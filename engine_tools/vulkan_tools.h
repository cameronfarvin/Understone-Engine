#ifndef __UE_VULKAN_TOOLS_H__
#define __UE_VULKAN_TOOLS_H__

#ifndef __UE_VK_VERBOSE__
#define __UE_VK_VERBOSE__ 1
#endif // __UE_VK_VERBOSE__

#include <vulkan/vulkan.h>
#include <engine_tools/memory_tools.h>
#include <data_structures/data_structures.h>



typedef struct
{
    const VkInstance   instance;
    const char** const active_extensions;

} uVulkanInfo;
uVulkanInfo vulkan_info;


__UE_internal__ __UE_call__ void
uQueryVulkanLayers(_mut_ s8**                  _mut_       restrict layer_names,
                   _mut_ VkLayerProperties*    _mut_       restrict layer_properties,
                   _mut_ VkInstanceCreateInfo* const       restrict instance_create_info,
                   const s8**                  const const restrict user_validation_layers,
                   const u32 num_user_layers)
{
    uAssertMsg_v(instance_create_info, "[ vulkan ] Null InstanceCreateInfo ptr provided.\n");
    uAssertMsg_v((layer_names == NULL),
                 "[ vulkan ] Layer names ptr ptr must be null; will be overwritten.\n");
    uAssertMsg_v((layer_properties == NULL),
                 "[ vulkan ] VkLayerProperties ptr must be null; will be overwritten.\n");
    uAssertMsg_v(user_validation_layers, "[ vulkan ] Null requested validation layer ptr.\n");

    if (num_user_layers == 0)
    {
        uDebugPrint_v("[ vulkan ] NOTE: No layers requested\n.");
        return;
    }

    // Query Layer Count
    VkResult success = VK_SUCCESS;
    size_t num_available_layers = 0;
    success = vkEnumerateInstanceLayerProperties(&(u32)num_available_layers,
                                                 NULL);
    uAssertMsg_v((success == VK_SUCCESS), "[ vulkan ] Unable to enumerate layers.\n");
    uAssertMsg_v((num_available_layers >= num_user_layers),
                 "[ vulkan ] Number of requested validation layers [ %d ] exceeds total avaialbe count [ %zd ].\n",
                 num_user_layers,
                 num_available_layers);

    layer_properties =
        (VkLayerProperties*)malloc(num_available_layers * sizeof(VkLayerProperties));

    // Query Layer Names
    success = vkEnumerateInstanceLayerProperties(&(u32)num_available_layers,
                                                 layer_properties);
    uAssertMsg_v((success == VK_SUCCESS), "[ vulkan ] Unable to enumerate layers.\n");


    // Set Layer Names
//
#if __UE_VK_VERBOSE__
//
    puts("[ vulkan ] Searching for validation layers...");
//
#endif // __UE_VK_VERBOSE__
//

    u32 num_added_layers = 0;
    layer_names = (s8**)malloc(num_available_layers * sizeof(s8**));
    for (u8 available_layer_idx = 0;
         available_layer_idx < num_available_layers;
         available_layer_idx++)
    {
        for (u8 user_layer_idx = 0;
             user_layer_idx < num_user_layers;
             user_layer_idx++)
        {
//
#if __UE_VK_VERBOSE__
//
            printf("[ vulkan ]\tLayer found: %s\n", layer_properties[available_layer_idx].layerName);
            fflush(stdout);
//
#endif // __UE_VK_VERBOSE__
//

            if (strcmp((const char*)user_validation_layers[user_layer_idx],
                       (const char*)layer_properties[available_layer_idx].layerName) == 0)
            {
                layer_names[num_added_layers] = (s8*)layer_properties[available_layer_idx].layerName;
                num_added_layers++;
            }
        }
    }

    uAssertMsg_v((num_added_layers == num_user_layers), "[ vulkan ] Unable to load all requested layers.\n");
    instance_create_info->enabledLayerCount   = num_added_layers;
    instance_create_info->ppEnabledLayerNames = (const char**)layer_names;
}



__UE_internal__ __UE_call__ void
uQueryVulkanExtensions(_mut_ s8**                   _mut_       restrict extension_names,
                       _mut_ VkExtensionProperties* _mut_       restrict extension_properties,
                       _mut_ VkInstanceCreateInfo*  const       restrict instance_create_info,
                       const s8**                   const const restrict user_extensions,
                       const u16                                         num_user_extensions)
{
    uAssertMsg_v(instance_create_info, "[ vulkan ] Null InstanceCreateInfo ptr provided.\n");
    uAssertMsg_v((extension_names == NULL),
                 "[ vulkan ] Extension names ptr ptr must be null; will be overwritten.\n");
    uAssertMsg_v((extension_properties == NULL),
                 "[ vulkan ] VkExtensionProperties ptr must be null; will be overwritten.\n");

    // Query Extension Count
    VkResult success = VK_SUCCESS;
    success = vkEnumerateInstanceExtensionProperties(NULL,
                                                     &instance_create_info->enabledExtensionCount,
                                                     NULL);
    uAssertMsg_v((success == VK_SUCCESS), "[ vulkan ] Unable to enumerate extensions.\n");

    extension_properties =
        (VkExtensionProperties*)malloc(instance_create_info->enabledExtensionCount * sizeof(VkExtensionProperties));

    // Query Extension Names
    success = vkEnumerateInstanceExtensionProperties(NULL,
                                                     &instance_create_info->enabledExtensionCount,
                                                     extension_properties);
    uAssertMsg_v((success == VK_SUCCESS), "[ vulkan ] Unable to enumerate extensions.\n");


    // Set Extension Names
//
#if __UE_VK_VERBOSE__
//
    puts("[ vulkan ] Searching for extensions...");
//
#endif // __UE_VK_VERBOSE__
//
    extension_names = (s8**)malloc(instance_create_info->enabledExtensionCount * sizeof(s8**));
    for (u8 ext_idx = 0; ext_idx < instance_create_info->enabledExtensionCount; ext_idx++)
    {
//
#if __UE_VK_VERBOSE__
//
            printf("[ vulkan ]\tExtension found: %s\n", extension_properties[ext_idx].extensionName);
            fflush(stdout);
//
#endif // __UE_VK_VERBOSE__
//
        extension_names[ext_idx] = (s8*)extension_properties[ext_idx].extensionName;
    }
    instance_create_info->ppEnabledExtensionNames = (const char**)extension_names;
}


__UE_internal__ __UE_call__ void
uCreateVulkanInstance(const VkApplicationInfo* const       restrict application_info,
                      const s8**               const const restrict user_validation_layers,
                      const u16                                     num_user_layers,
                      const s8**               const const restrict user_extensions,
                      const u16                                     num_user_extensions)

{
    // Note (error checking): It is legal to request no required validation layers and no requried extensions.
    uAssertMsg_v(application_info, "[ vulkan ] Null application info ptr provided.\n");
    VkExtensionProperties* extension_properties = NULL;
    VkLayerProperties*     layer_properties     = NULL;
    s8**                   extension_names      = NULL;
    s8**                   layer_names          = NULL;


    VkResult success = VK_SUCCESS;
    VkInstanceCreateInfo instance_create_info = { 0 };
    uQueryVulkanExtensions(extension_names,
                           extension_properties,
                           &instance_create_info,
                           user_extensions,
                           num_user_extensions);
    uQueryVulkanLayers(layer_names,
                       layer_properties,
                       &instance_create_info,
                       user_validation_layers,
                       num_user_layers);

    instance_create_info.sType                = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo     = application_info;
    success = vkCreateInstance(&instance_create_info,
                               NULL,
                               &(VkInstance)vulkan_info.instance);
    uAssertMsg_v((success == VK_SUCCESS), "[ vulkan ] Unable to create vulkan instance.\n");

    if (extension_names)
    {
        free(extension_names);
    }

    if (extension_properties)
    {
        free(extension_properties);
    }

}


__UE_internal__ __UE_call__ void
uCreateVulkanApplicationInfo(const s8*                const restrict application_name,
                             _mut_ VkApplicationInfo* const restrict application_info)
{
    uAssertMsg_v(application_name, "[ vulkan ] Null application name ptr provided.\n");
    uAssertMsg_v(application_info, "[ vulkan ] Null application info ptr provided.\n");

    application_info->sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info->pApplicationName   = application_name;
    application_info->applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    application_info->pEngineName        = "Understone Engine";
    application_info->apiVersion         = VK_API_VERSION_1_0;
}


__UE_internal__ __UE_call__ void
uInitializeVulkan(const s8*  const       restrict application_name,
                  const s8** const const restrict validation_layers,
                  const u16                       num_layers,
                  const s8** const const restrict extensions,
                  const u16                       num_extensions)
{
    // Note (error checking): It is legal to request no required validation layers and no requried extensions.
    uAssertMsg_v(application_name,  "[ vulkan ] Null application name ptr provided.\n");
    VkApplicationInfo application_info = { 0 };

    uCreateVulkanApplicationInfo(application_name, &application_info);
    uCreateVulkanInstance(&application_info,
                          validation_layers,
                          num_layers,
                          extensions,
                          num_extensions);
}


__UE_internal__ __UE_call__ void
uDestroyVulkan(const uVulkanInfo* const restrict v_info)
{
    uAssertMsg_v(v_info->instance, "Null instance ptr provided.\n");

    if (v_info->instance)
    {
        vkDestroyInstance(v_info->instance, NULL);
    }
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
