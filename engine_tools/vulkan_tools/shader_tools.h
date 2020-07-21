#ifndef __UE_SHADER_TOOLS__
#define __UE_SHADER_TOOLS__


#include <vulkan/vulkan.h>
#include "vulkan_macros.h"
#include "vulkan_types.h"
#include "macro_tools.h"
#include "debug_tools.h"


typedef enum
{
    uVK_VERTEX_SHADER,
    uVK_FRAGMENT_SHADER,
    uVK_SHADER_COUNT
} uVulkanShaderType;


typedef struct
{
    const char*             shader_file_path;
    const uVulkanShaderType shader_type;
} uVulkanShaderInfo;


typedef struct
{
    const VkShaderModule    shader_module; // unique ID
    const uVulkanShaderType shader_type;
    const char*             shader_data;
    const u32               num_shader_data_bytes;
} uVulkanShader;


//
// [ begin ] [ cfarvin::TEMP ]
// [ cfarvin::TODO ] Figure out a good way to load and keep shaders without
//                   this information living on the "global stack".
typedef struct
{
    // [ cfarvin::TODO ] This needs a graphics pipeline unique ID member, if
    //                   it is kept. In that case, the rest of these members
    //                   should be const.
    VkPipelineShaderStageCreateInfo* shader_stage_create_infos;
    uVulkanShader*                   shaders;
    u32                              num_shaders;
    u8                               num_stages;
} uVulkanGraphicsPipelineRecreateInfo;
__UE_global__ uVulkanGraphicsPipelineRecreateInfo REMOVE_ME;

// Note: loaded by uLoadSpirvModules()
//       consumed by uCreateVulkanGraphicsPipeline()
__UE_global__ uVulkanShaderInfo shaders_to_load[] =
{
    {
        .shader_file_path = ".\\shaders\\bin\\vkTriangle_vert.spv",
        .shader_type      = uVK_VERTEX_SHADER
    },
    {
        .shader_file_path = ".\\shaders\\bin\\vkTriangle_frag.spv",
        .shader_type      = uVK_FRAGMENT_SHADER
    }
};
// [ end ] [ cfarvin::TEMP ]
//


__UE_internal__ bool __UE_call__
uValidateVulkanShaderType(uVulkanShaderType shader_type)
{
    bool retVal = false;
    switch(shader_type)
    {
        case uVK_VERTEX_SHADER:
        {
            retVal = true;
            break;
        }
        case uVK_FRAGMENT_SHADER:
        {
            retVal = true;
            break;
        }
    };

    return retVal;
}


__UE_internal__ VkShaderStageFlagBits __UE_call__
uVulkanShaderTypeToStageBit(uVulkanShaderType shader_type)
{
    // Note: As of this writing, 0 is an invalid stage bit.
    VkShaderStageFlagBits retVal = 0;
    switch(shader_type)
    {
        case uVK_VERTEX_SHADER:
        {
            retVal = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        }
        case uVK_FRAGMENT_SHADER:
        {
            retVal = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        }
    };

    return retVal;
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
        uFatal("[ vulkan ] Unable to open SPIR-V file: %s\n", file_name);
    }

    s32 fseek_success = fseek(spir_v_file, 0L, SEEK_END);
    if (fseek_success) // Success == 0
    {
        uFatal("[ vulkan ] Unable to seek within SPIR-V file: %s\n", file_name);
    }

    *return_file_size = (size_t)ftell(spir_v_file);
    if (!(*return_file_size))
    {
        uFatal("[ vulkan ] SPIR-V file: %s has zero size.\n", file_name);
    }

    // Note: not freed in this scope
    size_t allocation_size = *return_file_size * sizeof(char);
    *return_file_data = (char*)calloc(1, allocation_size);
    if (!(*return_file_data))
    {
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
        uFatal("[ vulkan ] Unable to verify reading of SPIR-V file: %s.\n" , file_name);
    }

    fclose(spir_v_file);
}


__UE_internal__ void __UE_call__
uCreateVulkanShaderModule(const uVulkanShader* const shader)

{
    const uVulkanInfo* const v_info = uGetVulkanInfo();

    uAssertMsg_v(v_info->logical_device,        "[ vulkan ] SPIR-V file data must be non null.\n");
    uAssertMsg_v(shader,                        "[ vulkan ] uVulkanShader ptr must be non null\n.");
    uAssertMsg_v(!(shader->shader_module),      "[ vulkan ] VkShaderModule must be zero; will be overwritten\n.");
    uAssertMsg_v(shader->shader_data,           "[ vulkan ] Shader data ptr must be non null\n.");
    uAssertMsg_v(shader->num_shader_data_bytes, "[ vulkan ] Shader byte count must be non zero\n.");
    uAssertMsg_v(uValidateVulkanShaderType(shader->shader_type),
                 "[ vulkan ] Shader type must be valid.\n");


    VkShaderModuleCreateInfo smodule_create_info = { 0 };
    smodule_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    smodule_create_info.codeSize = shader->num_shader_data_bytes;
    smodule_create_info.pCode    = (const u32*)shader->shader_data;

    VkResult result = vkCreateShaderModule(v_info->logical_device,
                                           &smodule_create_info,
                                           NULL,
                                           (VkShaderModule*)&shader->shader_module);

    if (result != VK_SUCCESS)
    {
        free((void*)shader->shader_data);
        uFatal("[ vulkan ] Unable to create shader module.\n");
    }
}


// [ cfarvin::TODO ] Load glslangvalidator lib and validate spirv's
__UE_internal__ void __UE_call__
uLoadSpirvModules(_mut_ uVulkanShader** const const restrict return_shaders,
                  _mut_ u32*            const       restrict num_return_shaders)
{
    const size_t num_shader_infos = sizeof(shaders_to_load)/sizeof(uVulkanShaderInfo);

    uAssertMsg_v(num_shader_infos,       "[ shaders ] Number of shaders must be non zero.\n");
    uAssertMsg_v(return_shaders,         "[ shaders ] uVulkanShader ptr must be non null.\n");
    uAssertMsg_v(!(*return_shaders),     "[ shaders ] uVulkanShader ptr ptr must be null; will be overwritten.\n");
    uAssertMsg_v(num_return_shaders,     "[ shaders ] Number of return shaders ptr must be non null.\n");
    uAssertMsg_v(!(*num_return_shaders), "[ shaders ] Number of return shaders must be zero; will be overwritten.\n");


    // Note: not freed in this scope
    *return_shaders = (uVulkanShader*)calloc(num_shader_infos, sizeof(uVulkanShader));
    const char* return_shader_alloc_err = "[ shaders ] Unable to allocate uVulkanShader array.\n";
    uAssertMsg_v(return_shaders, return_shader_alloc_err);
    if (!return_shaders)
    {
        uFatal(return_shader_alloc_err);
    }

    // [ cfarvin::REMOVE ]
    REMOVE_ME.num_stages = 0;
    uDebugPrint("Loading %zd SPIR-V files...\n", num_shader_infos);
    for (u32 shader_idx = 0; shader_idx < num_shader_infos; shader_idx++)
    {
        // Validate incoming shader info
        const uVulkanShaderInfo shader_info = shaders_to_load[shader_idx];
        uAssertMsg_v(shader_info.shader_file_path, "[ shaders ] Shader file path must not be null.\n");
        uAssertMsg_v(uValidateVulkanShaderType(shader_info.shader_type),
                     "[ shaders ] Shader type must be valid.\n");

        //Copy shader info to return shaders
        const uVulkanShader* return_shader = &((*return_shaders)[shader_idx]);
        *(uVulkanShaderType*)&(return_shader->shader_type) = shader_info.shader_type;
        uReadSpirvFile((const char*)(shader_info.shader_file_path),             // File path (disk)
                       (_mut_ char**)&(return_shader->shader_data),             // Data bytes
                       (_mut_ size_t*)&(return_shader->num_shader_data_bytes)); // Num data bytes read


        // Create vulkan shader module
        uCreateVulkanShaderModule(return_shader);

        // [ cfarvin::RESTORE
        /* (*num_return_shaders)++; */
        // [ cfarvin::REMOVE ]
        REMOVE_ME.num_shaders++;

        // [ cfarvin::REVISIT ] Stage count needs to be accounted for outside of this test code,
        //                      and updated as futher stages are added.
        if (return_shader->shader_type == uVK_VERTEX_SHADER ||
            return_shader->shader_type == uVK_FRAGMENT_SHADER)
        {
            REMOVE_ME.num_stages++;
        }
    }
}


#endif // __UE_SHADER_TOOLS__
