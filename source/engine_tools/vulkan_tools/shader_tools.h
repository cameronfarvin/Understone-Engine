#ifndef __UE_SHADER_TOOLS__
#define __UE_SHADER_TOOLS__

#include "debug_tools.h"
#include "macro_tools.h"
#include "type_tools.h"
#include "vulkan_macros.h"
#include "vulkan_types.h"

#include <stdlib.h>
#include <vulkan/vulkan.h>

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
    VkShaderModule          module; // unique ID
} uVulkanShader;

//
// [ cfarvin::TEMP ] --begin this temporary block
// [ cfarvin::TODO ] Figure out a good way to load and keep shaders without
//                   this information living on the "global stack".
typedef struct
{
    // [ cfarvin::TODO ] This needs a graphics pipeline unique ID member, if
    //                   it is kept. In that case, the rest of these members
    //                   should be const.
    VkPipelineShaderStageCreateInfo* shader_stage_create_infos;
    u32                              num_shaders;
    u8                               num_stages;
} uVulkanGraphicsPipelineRecreateInfo;
uVulkanGraphicsPipelineRecreateInfo kGraphicsPipelineRecreateInfo = {};
// [ cfarvin::TEMP ] --end this temporary block
//

#if __UE_debug__ == 1
static bool
uValidateVulkanShaderType(uVulkanShaderType shader_type)
{
    bool retVal = false;
    if (shader_type == uVK_SHADER_TYPE_VERTEX || shader_type == uVK_SHADER_TYPE_FRAGMENT) { retVal = true; }

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
uCreateVulkanShaderModules(const uVulkanShader* const restrict shaders, const u32 num_shaders, VkPipelineShaderStageCreateInfo* const restrict pipeline_shader_stage_create_infos)
{
    const uVulkanInfo* const v_info = uGetVulkanInfo();

    uAssertMsg_v(v_info->logical_device, "[ shaders ] SPIR-V file data must be non null.\n");
    uAssertMsg_v(shaders, "[ shaders ] uVulkanShader ptr ptr must be non null\n.");
    uAssertMsg_v(pipeline_shader_stage_create_infos, "[ shaders ] VkPipelineShaderStageCreateInfo ptr must be non null\n.");

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

        VkResult result = vkCreateShaderModule(v_info->logical_device, &smodule_create_info, NULL, ( VkShaderModule* )&(shaders[shader_idx].module));

        if (result != VK_SUCCESS)
        {
            free(( void* )shaders[shader_idx].data);
            uFatal("[ shaders ] Unable to create shader module.\n");
        }

        pipeline_shader_stage_create_infos[shader_idx].sType               = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        pipeline_shader_stage_create_infos[shader_idx].stage               = uVulkanShaderTypeToStageBit(shaders[shader_idx].type);
        pipeline_shader_stage_create_infos[shader_idx].module              = shaders[shader_idx].module;
        pipeline_shader_stage_create_infos[shader_idx].pName               = "main"; // entry point
        pipeline_shader_stage_create_infos[shader_idx].pSpecializationInfo = NULL;   // optional: set shader constants
    }

    return true;
}

#endif // __UE_SHADER_TOOLS__
