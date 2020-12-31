#ifndef __UE_VULKAN_MACROS_H__
#define __UE_VULKAN_MACROS_H__

#ifndef __UE_VK_VERBOSE__
#define __UE_VK_VERBOSE__ 1
#endif // __UE_VK_VERBOSE__

#ifndef __UE_VkForceValidation__
#define __UE_vkForceValidation__ 1
#endif // __UE_VkForceValidation__ 1

#if __UE_debug__ == 1
#define uVULKAN_MAX_NANOSECOND_WAIT ~(( u64 )0)
#else
#define uVULKAN_MAX_NANOSECOND_WAIT 100000000 // 0.1 seconds
#endif                                        // __UE_debug__ == 1

#if __UE_VK_VERBOSE__
#define MAX_VKVERBOSE_LEN 256
char _vkverbose_buffer[MAX_VKVERBOSE_LEN];
char _vkMessage_buffer[MAX_VKVERBOSE_LEN];
#define uVkVerbose(...)                                                                 \
    snprintf(_vkMessage_buffer, MAX_VKVERBOSE_LEN, __VA_ARGS__);                        \
    snprintf(_vkverbose_buffer, MAX_VKVERBOSE_LEN, "[ vulkan ] %s", _vkMessage_buffer); \
    fputs(_vkverbose_buffer, stdout);                                                   \
    fflush(stdout);
#else
#define uVkVerbose(...) /* uVKVerbose() REMOVED */
#endif // __UE_VK_VERBOSE__

#endif // __UE_VULKAN_MACROS_H__
