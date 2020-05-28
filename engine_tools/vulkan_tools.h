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
#endif // _WIN32

#include <vulkan/vulkan.h>
#include <engine_tools/memory_tools.h>
#include <data_structures/data_structures.h>


typedef struct
{
    u32 graphics_index;
    u32 present_index;
} uVulkanQueueInfo;

#define uVULKAN_NUM_QUEUES           2
#define uVULKAN_GRAPHICS_QUEUE_INDEX 0
#define uVULKAN_PRESENT_QUEUE_INDEX  1
typedef struct
{
    const VkInstance       instance;
    const VkPhysicalDevice physical_device;
    const VkDevice         logical_device;
    const VkQueue          queues[uVULKAN_NUM_QUEUES];
    const int          t[uVULKAN_NUM_QUEUES];
    const VkSurfaceKHR     surface;
} uVulkanInfo;


// Forward decls
__UE_internal__ __UE_call__ void
uDestroyVulkan(const uVulkanInfo* const restrict v_info);


VkDebugUtilsMessengerEXT           vulkan_main_debug_messenger;
VkDebugUtilsMessengerCreateInfoEXT vulkan_main_debug_messenger_info  = { 0 };
VkDebugUtilsMessengerCreateInfoEXT vulkan_setup_debug_messenger_info = { 0 };


__UE_internal__ __UE_call__ void
uCreateVulkanLogicalDevice(_mut_ uVulkanInfo*      const restrict v_info,
                           const uVulkanQueueInfo* const restrict queue_info)
{
    uAssertMsg_v(v_info,                  "[ vulkan ] Null vulkan info ptr provided.\n");
    uAssertMsg_v(v_info->physical_device, "[ vulkan ] Physical device must be non null.\n");
    uAssertMsg_v(!v_info->logical_device, "[ vulkan ] Logical device must be null; will be overwritten.\n");
    uAssertMsg_v(v_info->queues,          "[ vulkan ] Queue array must be non null.\n");
    uAssertMsg_v(queue_info,              "[ vulkan ] Queue indices ptr must be non null\n");


    // [ cfarvin::PERF ] Find number of unique values of uVulkanQueueInfo members only allocate
    //                   that many VkDeviceQueueCreateInfos, and only call vkGetDeviceQueue
    //                   that many times. Will need to temporarily store information from
    //                   uSelectVulkanPhysicalDevice: { QueueIndex, QueueType }
    const char* device_create_fail_msg = "[ vulkan ] Unable to create logical device.\n";
    VkDeviceQueueCreateInfo* device_queue_create_infos =
        (VkDeviceQueueCreateInfo*) calloc(uVULKAN_NUM_QUEUES, sizeof(VkDeviceQueueCreateInfo));
    uAssertMsg_v(device_queue_create_infos, device_create_fail_msg);
    if (!device_queue_create_infos)
    {
        uDestroyVulkan(v_info);
        uFatal(device_create_fail_msg);
    }

    for (u32 queue_family_idx = 0; queue_family_idx < uVULKAN_NUM_QUEUES; queue_family_idx++)
    {
        r32 device_queue_priorities = 1.0f;
        device_queue_create_infos[queue_family_idx].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device_queue_create_infos[queue_family_idx].queueFamilyIndex = queue_family_idx;
        device_queue_create_infos[queue_family_idx].queueCount = 1;
        device_queue_create_infos[queue_family_idx].pQueuePriorities = &device_queue_priorities;
    }

    // [ cfarvin::TODO ] When modified, don't forget to add checks to uSelectVulkanPhysicalDevice();
    VkPhysicalDeviceFeatures physical_device_features = { 0 };

    VkDeviceCreateInfo logical_device_create_info = { 0 };
    logical_device_create_info.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    logical_device_create_info.pQueueCreateInfos    = device_queue_create_infos;
    logical_device_create_info.queueCreateInfoCount = uVULKAN_NUM_QUEUES;
    logical_device_create_info.pEnabledFeatures     = &physical_device_features;

    // Note: modern Vulkan implementations no longer separate instance and device layers.
    VkResult device_creation_success = vkCreateDevice(v_info->physical_device,
                                                      &logical_device_create_info,
                                                      NULL,
                                                      (VkDevice*)&v_info->logical_device);

    uAssertMsg_v(device_creation_success == VK_SUCCESS, device_create_fail_msg);
    if (device_creation_success != VK_SUCCESS)
    {
        uDestroyVulkan(v_info);
        uFatal(device_create_fail_msg);
    }

    // [ cfarvin::REVISIT ]
    // Get queue handles
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


__UE_internal__ __UE_call__ void
uSelectVulkanPhysicalDevice(const VkPhysicalDevice** const const restrict physical_device_list,
                            _mut_ VkPhysicalDevice*  const       restrict return_device,
                            _mut_ uVulkanQueueInfo*  const       restrict queue_info,
                            const u32                                     num_physical_devices,
                            const VkSurfaceKHR*      const       restrict surface)
{
    uAssertMsg_v(physical_device_list, "[ vulkan ] Null vulkan device list pointer provided.\n");
    uAssertMsg_v(!(*return_device),    "[ vulkan ] Return device ptr must be null; will be overwritten\n");
    uAssertMsg_v(queue_info,           "[ vulkan ] Queue family indices ptr must be non null\n");
    uAssertMsg_v(num_physical_devices, "[ vulkan ] A minimum of one physical devices is required.\n");
    uAssertMsg_v(surface,              "[ vulkan ] Surface must be non null.\n");
    for (u32 device_idx = 0; device_idx < num_physical_devices; device_idx++)
    {
        uAssertMsg_v(physical_device_list[device_idx],
                     "[ vulkan ] Indices of physical_device_list must be non-null.\n");
    }


    bool selection_complete = false;
    for (u32 device_idx = 0; device_idx < num_physical_devices; device_idx++)
    {
        // Physical device
        VkPhysicalDevice physical_device = *physical_device_list[device_idx];

        if (!physical_device)
        {
            continue;
        }

        // [ cfarvin::TODO ]
        // Aquire physical device features
        /* VkPhysicalDeviceFeatures device_features; */
        /* vkGetPhysicalDeviceFeatures(physical_device, */
        /*                             &device_features); */

        // [ cfarvin::TODO ]
        // Aquire physical device properties
        /* VkPhysicalDeviceProperties device_properties; */
        /* vkGetPhysicalDeviceProperties(physical_device, */
        /*                               &device_properties); */

        // Get queue family count
        u32 queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, NULL);
        if (!queue_family_count)
        {
            continue;
        }

        // Get queue families
        VkQueueFamilyProperties* queue_family_props = (VkQueueFamilyProperties*)calloc(queue_family_count,
                                                                                       sizeof(VkQueueFamilyProperties));
        const char* queue_family_alloc_err_msg = "[ vulkan ] Unable to allocate queue family property array.\n";
        uAssertMsg_v(queue_family_props, queue_family_alloc_err_msg);
        if (!queue_family_props)
        {
            uFatal(queue_family_alloc_err_msg);
        }
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_family_props);

        typedef struct
        {
            u32  index;
            bool validated;
        } uQueueFamilyPair;
        u32 num_queues = 0;

        // Require graphics family
        uQueueFamilyPair graphics_pair = { 0 };
        num_queues++;

        // Require present family
        uQueueFamilyPair present_pair  = { 0 };
        num_queues++;

        for (u32 queue_idx = 0; queue_idx < queue_family_count; queue_idx++)
        {
            // Check grahpics family
            if (queue_family_props[queue_idx].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                graphics_pair.index = queue_idx;
                graphics_pair.validated = true;
            }

            // Check present family
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
                // Helper data for uCreateVulkanLogicalDevice()
                queue_info->graphics_index = graphics_pair.index;
                queue_info->present_index  = present_pair.index;

                // Set the physical device;
                *return_device = physical_device;

                // Issue perf warning for different graphics/presentation queue
                if (graphics_pair.index != present_pair.index)
                {
                    uWarning("[ vulkan ] [ perf ] Different graphics and prsent queue families chosen.\n");
                }

                selection_complete = true;
                break;
            }
        }

        if (queue_family_props)
        {
            free(queue_family_props);
        }

        // [ cfarvin::PERF ] If one VkQueue can do all we need, we are needlessly
        // passing around more in a commonly referenced structure (uVulkanInfo->queues).
        // We must update uVULKAN_NUM_QUEUES as we check for more queue capabilities.
        const char* queue_count_error_msg =
            "[ vulkan ] uVulkanInfo.queues length: %d. %d queues were checked during physical device creation.\n";
        uAssertMsg_v(uVULKAN_NUM_QUEUES == num_queues,
                     queue_count_error_msg,
                     uVULKAN_NUM_QUEUES,
                     num_queues);
        if (uVULKAN_NUM_QUEUES != num_queues)
        {
            uFatal(queue_count_error_msg,
                   uVULKAN_NUM_QUEUES,
                   num_queues);
        }

        if (selection_complete)
        {
            break;
        }
    }

    const char* no_selection_error_msg = "[ vulkan ] Unable to verify all queue family requirements.\n";
    uAssertMsg_v(selection_complete, no_selection_error_msg);
    if (!selection_complete)
    {
        uFatal(no_selection_error_msg);
    }
}


__UE_internal__ __UE_call__ void
uCreateVulkanPysicalDevice(_mut_ uVulkanInfo*       const restrict v_info,
                           _mut_ uVulkanQueueInfo*  const restrict queue_info)
{
    uAssertMsg_v(v_info,                   "[ vulkan ] Null vulkan info ptr provided.\n");
    uAssertMsg_v(!v_info->physical_device, "[ vulkan ] Physical device must be null; will be overwritten.\n");
    uAssertMsg_v(!v_info->logical_device,  "[ vulkan ] Logical device must be null; will be overwritten.\n");
    uAssertMsg_v(queue_info,               "[ vulkan ] Queue family indices ptr must be non null.\n");


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
                                (VkSurfaceKHR*)&v_info->surface);
    uAssertMsg_v(candidate_device != NULL, "[ vulkan ] Unable to select candidate device.\n");
    if (!candidate_device)
    {
        uDestroyVulkan(v_info);
        uFatal("[ vulkan ] Unable to find suitable device.\n");
        return;
    }

    VkPhysicalDevice* non_const_physical_device = (VkPhysicalDevice*)&(v_info->physical_device);
    *non_const_physical_device = candidate_device;
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
    uAssertMsg_v(debug_message_create_info,
                 "[ vulkan ] Null VkDebugUtilsMessengerCreateInfoEXT ptr provided.\n");


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
    uAssertMsg_v(v_info,           "[ vulkan ] Null uVulkanInfo ptr provided.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] Null uVulkanInfo->instance ptr provided.\n");
    uAssertMsg_v(debug_message_create_info,
                 "[ vulkan ] Null VkDebugUtilsMessengerCreateInfoEXT ptr provided.\n");


    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT =
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(v_info->instance,
                                                                  "vkCreateDebugUtilsMessengerEXT");

    uAssertMsg_v(vkCreateDebugUtilsMessengerEXT,
                 "[ vulkan ] Failed to acquire pfn: vkCreateDebugUtilsMessengerEXT\n");

    if (vkCreateDebugUtilsMessengerEXT)
    {
        uCreateVulkanDebugMessengerInfo((VkDebugUtilsMessengerCreateInfoEXT*)debug_message_create_info);

        const char* debug_create_fail_msg = "[ vulkan ] Failed to create debug messenger callback.\n";
        VkResult success =
            vkCreateDebugUtilsMessengerEXT(v_info->instance,
                                           debug_message_create_info,
                                           NULL,
                                           debug_messenger);
        uAssertMsg_v(((success == VK_SUCCESS) && debug_messenger), debug_create_fail_msg);
        if (success != VK_SUCCESS)
        {
            uDestroyVulkan(v_info);
            uFatal(debug_create_fail_msg);
        }
    }
}


#if _WIN32
__UE_internal__ __UE_call__ void
uCreateWin32Surface(_mut_ uVulkanInfo* const restrict v_info)
{
    uAssertMsg_v(v_info,           "[ vulkan ] Null uVulkanInfo ptr provided.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] Null uVulkanInfo->instance ptr provided.\n");

    uWin32Info* win32_info = uWin32CreateWindow("Understone Engine");
    uAssertMsg_v(win32_info, "[ vulkan ] [ win32 surface ] Unable to create Win32 surface.\n");
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

    const char* win32_surface_err_msg = "[ vulkan ] Failed to create Win32Surface.\n";
    uAssertMsg_v(win32_surface_result == VK_SUCCESS, win32_surface_err_msg)
    if (win32_surface_result != VK_SUCCESS)
    {
        uDestroyVulkan(v_info);
        uFatal(win32_surface_err_msg);
    }

    // [ cfarvin::NOTE ]
    // May want to revisit deleing should this info be necessary.
    uDestroyWin32(win32_info);
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
uQueryVulkanLayers(_mut_ s8***                 _mut_ const restrict layer_names,
                   _mut_ VkLayerProperties*    _mut_       restrict layer_properties,
                   _mut_ VkInstanceCreateInfo* const       restrict instance_create_info,
                   const s8**                  const const restrict user_validation_layers,
                   const u32 num_user_layers)
{
    uAssertMsg_v(instance_create_info,   "[ vulkan ] Null InstanceCreateInfo ptr provided.\n");
    uAssertMsg_v(!(*layer_names),        "[ vulkan ] Layer names ptr ptr must be null; will be overwritten.\n");
    uAssertMsg_v(!layer_properties,      "[ vulkan ] VkLayerProperties ptr must be null; will be overwritten.\n");
    uAssertMsg_v(user_validation_layers, "[ vulkan ] Null requested validation layer ptr.\n");


    if (num_user_layers == 0)
    {
        return;
    }

    // Query Layer Count
    VkResult success = VK_SUCCESS;
    size_t num_available_layers = 0;
    success = vkEnumerateInstanceLayerProperties(&((u32)num_available_layers), NULL);

    const char* enumerate_layers_fail_msg = "[ vulkan ] Unable to enumerate layers.\n";
    uAssertMsg_v((success == VK_SUCCESS), enumerate_layers_fail_msg);
    uAssertMsg_v((num_available_layers >= num_user_layers),
                 "[ vulkan ] Number of requested validation layers [ %d ] exceeds total avaialbe count [ %zd ].\n",
                 num_user_layers,
                 num_available_layers);
    if (success != VK_SUCCESS)
    {
        uFatal(enumerate_layers_fail_msg);
    }


    layer_properties =
        (VkLayerProperties*)malloc(num_available_layers * sizeof(VkLayerProperties));

    // Query Layer Names
    success = vkEnumerateInstanceLayerProperties(&(u32)num_available_layers,
                                                 layer_properties);
    uAssertMsg_v((success == VK_SUCCESS), enumerate_layers_fail_msg);
    if (success != VK_SUCCESS)
    {
        uFatal(enumerate_layers_fail_msg);
    }


    // Set Layer Names
    uVkVerbose("Searching for validation layers...\n");
    u32 num_added_layers = 0;
    *layer_names = (s8**)malloc(num_available_layers * sizeof(s8**));
    for (u32 available_layer_idx = 0;
         available_layer_idx < num_available_layers;
         available_layer_idx++)
    {
        for (u32 user_layer_idx = 0;
             user_layer_idx < num_user_layers;
             user_layer_idx++)
        {
            uVkVerbose("\tLayer found: %s\n", layer_properties[available_layer_idx].layerName);
            if (strcmp((const char*)user_validation_layers[user_layer_idx],
                       (const char*)layer_properties[available_layer_idx].layerName) == 0)
            {
                (*layer_names)[num_added_layers] = (s8*)layer_properties[available_layer_idx].layerName;
                num_added_layers++;
            }
        }
    }

    const char* layer_load_fail_msg = "[ vulkan ] Unable to load all requested layers.\n";
    uAssertMsg_v((num_added_layers == num_user_layers), layer_load_fail_msg);
    if (num_added_layers != num_user_layers)
    {
        uFatal(layer_load_fail_msg);
    }

    instance_create_info->enabledLayerCount   = num_added_layers;
    instance_create_info->ppEnabledLayerNames = *layer_names;
}



__UE_internal__ __UE_call__ void
uQueryVulkanExtensions(_mut_ s8***                  _mut_       restrict extension_names,
                       _mut_ VkExtensionProperties* _mut_       restrict extension_properties,
                       _mut_ VkInstanceCreateInfo*  const       restrict instance_create_info,
                       const s8**                   const const restrict user_extensions,
                       const u16                                         num_user_extensions)
{
    // Note (error checking): It is legal to request no required validation layers and no requried extensions.
    uAssertMsg_v(instance_create_info,  "[ vulkan ] Null InstanceCreateInfo ptr provided.\n");
    uAssertMsg_v(!(*extension_names),   "[ vulkan ] Extension names ptr ptr must be null; will be overwritten.\n");
    uAssertMsg_v(!extension_properties, "[ vulkan ] VkExtensionProperties ptr must be null; will be overwritten.\n");


    // Query Extension Count
    VkResult success = VK_SUCCESS;
    success = vkEnumerateInstanceExtensionProperties(NULL,
                                                     &instance_create_info->enabledExtensionCount,
                                                     NULL);

    const char* enumerate_properties_fail_msg = "[ vulkan ] Unable to enumerate layer properties.\n";
    uAssertMsg_v((success == VK_SUCCESS), enumerate_properties_fail_msg);
    if (success != VK_SUCCESS)
    {
        uFatal(enumerate_properties_fail_msg);
    }

    extension_properties =
        (VkExtensionProperties*)malloc(instance_create_info->enabledExtensionCount * sizeof(VkExtensionProperties));

    // Query Extension Names
    success = vkEnumerateInstanceExtensionProperties(NULL,
                                                     &instance_create_info->enabledExtensionCount,
                                                     extension_properties);

    uAssertMsg_v((success == VK_SUCCESS), enumerate_properties_fail_msg);
    if (success != VK_SUCCESS)
    {
        uFatal(enumerate_properties_fail_msg);
    }


    // Set Extension Names
    uVkVerbose("Searching for extensions...\n");
    u32 num_added_extensions = 0;
    *extension_names = (s8**)malloc(instance_create_info->enabledExtensionCount * sizeof(s8**));
    for (u32 ext_idx = 0; ext_idx < instance_create_info->enabledExtensionCount; ext_idx++)
    {
        uVkVerbose("\tExtension found: %s\n", extension_properties[ext_idx].extensionName);
        for (u32 user_ext_idx = 0;
             user_ext_idx < num_user_extensions;
             user_ext_idx++)
        {
            if (strcmp((const char*)user_extensions[user_ext_idx],
                       (const char*)extension_properties[ext_idx].extensionName) == 0)
            {
                (*extension_names)[num_added_extensions] = (s8*)extension_properties[ext_idx].extensionName;
                num_added_extensions++;
            }
        }
    }

    const char* load_extensions_fail_msg = "[ vulkan ] Unable to load all requested extensions.\n";
    uAssertMsg_v((num_added_extensions == num_user_extensions), load_extensions_fail_msg);
    if (num_added_extensions != num_user_extensions)
    {
        uFatal(load_extensions_fail_msg);
    }

    instance_create_info->enabledExtensionCount   = num_added_extensions;
    instance_create_info->ppEnabledExtensionNames = *extension_names;
}


__UE_internal__ __UE_call__ void
uCreateVulkanInstance(const uVulkanInfo*       const       restrict v_info,
                      const VkApplicationInfo* const       restrict application_info,
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


    uCreateVulkanDebugMessengerInfo(&vulkan_setup_debug_messenger_info);

    VkResult success = VK_SUCCESS;
    VkInstanceCreateInfo instance_create_info = { 0 };

    uQueryVulkanExtensions(&extension_names,
                           extension_properties,
                           &instance_create_info,
                           user_extensions,
                           num_user_extensions);

    uQueryVulkanLayers(&layer_names,
                       layer_properties,
                       &instance_create_info,
                       user_validation_layers,
                       num_user_layers);

    instance_create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = application_info;
    instance_create_info.pNext            = &vulkan_setup_debug_messenger_info;
    success = vkCreateInstance(&instance_create_info,
                               NULL,
                               (VkInstance*)&v_info->instance);

    const char* instance_create_fail_msg = "[ vulkan ] Unable to create vulkan instance.\n";
    uAssertMsg_v((success == VK_SUCCESS), instance_create_fail_msg);
    if (success != VK_SUCCESS)
    {
        uDestroyVulkan(v_info);
        uFatal(instance_create_fail_msg);
    }

    uCreateVulkanDebugMessenger(v_info,
                                (VkDebugUtilsMessengerCreateInfoEXT*)&vulkan_main_debug_messenger_info,
                                (VkDebugUtilsMessengerEXT*)&vulkan_main_debug_messenger);

    if (extension_names)
    {
        free(extension_names);
    }

    if (layer_names)
    {
        free(layer_names);
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
uInitializeVulkan(_mut_ uVulkanInfo* const       restrict v_info,
                  const s8*          const       restrict application_name,
                  const s8**         const const restrict validation_layers,
                  const u16                               num_layers,
                  const s8**         const const restrict extensions,
                  const u16                               num_extensions)
{
    // Note (error checking): It is legal to request no required validation layers and no requried extensions.
    uAssertMsg_v(v_info,                   "[ vulkan ] Null v_info ptr provided.\n");
    uAssertMsg_v(!v_info->instance,        "[ vulkan ] Instance must be null; will be overwritten.\n");
    uAssertMsg_v(!v_info->physical_device, "[ vulkan ] Physical device must be null; will be overwritten.\n");
    uAssertMsg_v(!v_info->logical_device,  "[ vulkan ] Logical device must be null; will be overwritten.\n");
    uAssertMsg_v(application_name,         "[ vulkan ] Null application name ptr provided.\n");
    VkApplicationInfo application_info = { 0 };


    uCreateVulkanApplicationInfo(application_name, &application_info);
    uCreateVulkanInstance(v_info,
                          &application_info,
                          validation_layers,
                          num_layers,
                          extensions,
                          num_extensions);

    uCreateVulkanSurface(v_info);
    uVulkanQueueInfo queue_info = { 0 };
    uCreateVulkanPysicalDevice(v_info, &queue_info); // queue_info built
    uCreateVulkanLogicalDevice(v_info, &queue_info); // queue_info consumed
}


__UE_internal__ __UE_call__ void
uDestroyVulkan(const uVulkanInfo* const restrict v_info)
{
    uAssertMsg_v(v_info,           "Null uVulkanInfo ptr provided.\n");
    uAssertMsg_v(v_info->instance, "Null instance ptr provided.\n");


    if (v_info && v_info->instance)
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

        // Destroy surface khr
        vkDestroySurfaceKHR(v_info->instance, v_info->surface, NULL);
    }

    if (v_info && v_info->logical_device)
    {
        vkDestroyDevice(v_info->logical_device, NULL);

    }

    if (v_info && v_info->instance)
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
