#ifndef __UE_SHADER_TOOLS__
#define __UE_SHADER_TOOLS__

#include <stdlib.h>

#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif // _WIN32

#include "debug_tools.h"
#include "macro_tools.h"
#include "maths_tools.h"
#include "type_tools.h"
#include "vulkan_macros.h"
#include "vulkan_types.h"

#include <vulkan/vulkan.h>

typedef struct
{
    v2 position;
    v3 color;
} uShaderVertex;

typedef enum
{
    uVK_SHADER_TYPE_NONE = 0,
    uVK_SHADER_TYPE_VERTEX,
    uVK_SHADER_TYPE_FRAGMENT,
    uVK_SHADER_TYPE_COUNT
} uVulkanShaderType;

typedef struct
{
    const char*             name;
    const u32*              data;
    const size_t            data_size;
    const uVulkanShaderType type;
    VkShaderModule          module; // mutable
} uVulkanShader;

#if __UE_debug__ == 1
static bool
uValidateVulkanShaderType(uVulkanShaderType shader_type)
{
    bool retVal = false;
    if (shader_type == uVK_SHADER_TYPE_VERTEX || shader_type == uVK_SHADER_TYPE_FRAGMENT)
    {
        retVal = true;
    }

    return retVal;
}
#endif

VkShaderStageFlagBits
uVulkanShaderTypeToStageBit(uVulkanShaderType shader_type)
{
    // Note: As of this writing, 0 is an invalid stage bit.
    VkShaderStageFlagBits retVal = ( VkShaderStageFlagBits )0;
    switch (shader_type)
    {
        case uVK_SHADER_TYPE_VERTEX:
        {
            retVal = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        }
        case uVK_SHADER_TYPE_FRAGMENT:
        {
            retVal = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        }
        default:
        {
            retVal = ( VkShaderStageFlagBits )0;
            break;
        }
    };

    return retVal;
}

bool
uCreateVulkanShaderModules(const uVulkanShader* const restrict             shaders,
                           const u32                                       num_shaders,
                           VkPipelineShaderStageCreateInfo* const restrict shader_stage_create_infos)
{
    const uVulkanInfo* const v_info = uGetVulkanInfo();

    // [ cfarvin::TODO ] We're assuming that the user will pass two pointers
    // containing the same number of elements, both of shaders and of pipeline
    // stage create infos. Is there a way we can ensure that the number of
    // pipelien shader create infos is == num shaders?
    uAssertMsg_v(v_info->logical_device, "[ shaders ] SPIR-V file data must be non null.\n");
    uAssertMsg_v(shaders, "[ shaders ] uVulkanShader ptr ptr must be non null\n.");
    uAssertMsg_v(shader_stage_create_infos, "[ shaders ] VkPipelineShaderStageCreateInfo ptr must be non null\n.");

    for (u32 shader_idx = 0; shader_idx < num_shaders; shader_idx++)
    {
        uAssertMsg_v(!shaders[shader_idx].module, "[ shaders ] VkShaderModule must be zero; will be overwritten\n.");
        uAssertMsg_v(shaders[shader_idx].data, "[ shaders ] Shader data ptr must be non null\n.");
        uAssertMsg_v(shaders[shader_idx].data_size, "[ shaders ] Shader byte count must be non zero\n.");
        uAssertMsg_v(uValidateVulkanShaderType(shaders[shader_idx].type), "[ shaders ] Shader type must be valid.\n");

        VkShaderModuleCreateInfo smodule_create_info = {};
        smodule_create_info.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        smodule_create_info.codeSize                 = shaders[shader_idx].data_size;
        ;
        smodule_create_info.pCode = shaders[shader_idx].data;

        VkResult result = vkCreateShaderModule(v_info->logical_device,
                                               &smodule_create_info,
                                               NULL,
                                               ( VkShaderModule* )&(shaders[shader_idx].module));

        if (result != VK_SUCCESS)
        {
            uFatal("[ shaders ] Unable to create shader module.\n");
        }

        shader_stage_create_infos[shader_idx].sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shader_stage_create_infos[shader_idx].stage  = uVulkanShaderTypeToStageBit(shaders[shader_idx].type);
        shader_stage_create_infos[shader_idx].module = shaders[shader_idx].module;
        shader_stage_create_infos[shader_idx].pName  = "main";            // entry point
        shader_stage_create_infos[shader_idx].pSpecializationInfo = NULL; // optional: set shader constants
    }

    return true;
}

void
uDestroyShaderModules(uVulkanShader* const restrict shaders, const size_t num_shaders)
{
    uVulkanInfo* v_info = ( uVulkanInfo* )uGetVulkanInfo();
    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(shaders, "[ vulkan ] uVulkanShader ptr must be non null.\n");
    uAssertMsg_v(num_shaders, "[ vulkan ] num_shaders must be non zero.\n");

    for (size_t shader_idx = 0; shader_idx < num_shaders; shader_idx++)
    {
        VkShaderModule& shader_module = shaders[shader_idx].module;
        if (nullptr != shader_module)
        {
            vkDestroyShaderModule(v_info->logical_device, shader_module, NULL);
            shader_module = NULL;
        }
    }
}

#endif // __UE_SHADER_TOOLS__
