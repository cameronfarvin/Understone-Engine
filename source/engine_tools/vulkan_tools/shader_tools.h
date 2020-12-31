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
    const char*             spv_path;
    const VkShaderModule    module; // unique ID
    const uVulkanShaderType type;
    const char*             data;
    const size_t            data_size;
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

// Note: loaded by uLoadSpirvModules()
//       consumed by uCreateVulkanGraphicsPipeline()
uVulkanShader kShadersToLoad[] = { { .spv_path = "shaders/vkTriangle_vert.spv", .module = {}, .type = uVK_SHADER_TYPE_VERTEX, .data = {} },
                                   { .spv_path = "shaders/vkTriangle_frag.spv", .module = {}, .type = uVK_SHADER_TYPE_FRAGMENT, .data = {} } };
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

static VkShaderStageFlagBits
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

static void
uReadSpirvFile(uVulkanShader* const restrict shader)
{
    uAssertMsg_v(shader->spv_path, "[ shader ] SPIR-V module path ptr must be non null.\n");
    uAssertMsg_v(!shader->data_size, "[ shader ] SPIR-V module data size must be zero; will be overwritten.\n");
    uAssertMsg_v(!shader->data, "[ shader ] SPIR-V module data ptr must be null; will be overwritten.\n");

    size_t* non_const_data_size = ( size_t* )&(shader->data_size);
    char**  non_const_data      = ( char** )&(shader->data);

    FILE* spir_v_file = NULL;
    fopen_s(&spir_v_file, shader->spv_path, "rb");
    if (!spir_v_file) { uFatal("[ shader ] Unable to open SPIR-V file: %s\n", shader->spv_path); }

    s32 fseek_success = fseek(spir_v_file, 0L, SEEK_END);
    if (fseek_success) // Success == 0
    {
        uFatal("[ shader ] Unable to seek within SPIR-V file: %s\n", shader->spv_path);
    }

    *non_const_data_size = ( size_t )ftell(spir_v_file);
    if (!shader->data_size) { uFatal("[ shader ] SPIR-V file: %s has zero size.\n", shader->spv_path); }

    // [ cfarvin::TODO ] Ensure that this is freed
    // Note: not freed in this scope
    *non_const_data = ( char* )calloc(1, shader->data_size);
    if (!shader->data) { uFatal("[ shader ] Unable to allocate host memory for SPIR-V file: %s.\n", shader->spv_path); }

    rewind(spir_v_file);
    size_t items_read = fread_s(( void* )(*non_const_data), // buffer
                                shader->data_size,          // buffer size
                                shader->data_size,          // element size
                                1,                          // element count
                                spir_v_file);               // FILE*

    if (items_read != 1) { uFatal("[ shader ] Unable to verify reading of SPIR-V file: %s.\n", shader->spv_path); }

    fclose(spir_v_file);
}

static void
uCreateVulkanShaderModule(const uVulkanShader* const shader)
{
    const uVulkanInfo* const v_info = uGetVulkanInfo();

    uAssertMsg_v(v_info->logical_device, "[ shaders ] SPIR-V file data must be non null.\n");
    uAssertMsg_v(shader, "[ shaders ] uVulkanShader ptr must be non null\n.");
    uAssertMsg_v(!(shader->module), "[ shaders ] VkShaderModule must be zero; will be overwritten\n.");
    uAssertMsg_v(shader->data, "[ shaders ] Shader data ptr must be non null\n.");
    uAssertMsg_v(shader->data_size, "[ shaders ] Shader byte count must be non zero\n.");
    uAssertMsg_v(uValidateVulkanShaderType(shader->type), "[ shaders ] Shader type must be valid.\n");

    VkShaderModuleCreateInfo smodule_create_info = {};
    smodule_create_info.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    smodule_create_info.codeSize                 = shader->data_size;
    smodule_create_info.pCode                    = ( const u32* )shader->data;

    VkResult result = vkCreateShaderModule(v_info->logical_device, &smodule_create_info, NULL, ( VkShaderModule* )&(shader->module));

    if (result != VK_SUCCESS)
    {
        free(( void* )shader->data);
        uFatal("[ shaders ] Unable to create shader module.\n");
    }
}

// [ cfarvin::REMOVE ] This will no longer be needed when we autogen-bake shaders into the exe
// Also reset the data in kShadersToLoad which was stored when/if the shader modules were originally loaded
// When the augogen-bake features are complete, this global will not have the shader module info in it, only
// the raw module data and shader stage type.
__UE_inline__ static void
uVulkanDestroyStaticShaderData_TEMP()
{
    for (size_t shader_idx = 0; shader_idx < sizeof(kShadersToLoad) / sizeof(uVulkanShader); shader_idx++)
    {
        VkShaderModule* non_const_module    = ( VkShaderModule* )&(kShadersToLoad[shader_idx].module);
        char**          non_const_data      = ( char** )&(kShadersToLoad[shader_idx].data);
        size_t*         non_const_data_size = ( size_t* )&kShadersToLoad[shader_idx].data_size;

        uAssert(non_const_module);
        uAssert(non_const_data);
        uAssert(non_const_data_size);

        uAssert(*non_const_module);
        uAssert(*non_const_data);
        uAssert(*non_const_data_size);

        *non_const_data_size = ( size_t )0;
        *non_const_module    = ( VkShaderModule )0;
        *non_const_data      = ( char* )0;
    }
}

// [ cfarvin::TODO ] Load glslangvalidator lib and validate spirv's (in debug mode only?)
static void
uLoadSpirvModules(uVulkanShader** const restrict return_shaders, u32* const restrict num_return_shaders)
{
    // [ cfarvin::REMOVE ] This is temporary until shader modules are autogenerated and baked
    const size_t num_shader_infos = sizeof(kShadersToLoad) / sizeof(uVulkanShader);

    uAssertMsg_v(num_shader_infos, "[ shaders ] Number of shaders must be non zero.\n");
    uAssertMsg_v(return_shaders, "[ shaders ] uVulkanShader ptr ptr must be non null.\n");
    uAssertMsg_v(!(*return_shaders), "[ shaders ] uVulkanShader ptr must be null; will be overwritten.\n");
    uAssertMsg_v(num_return_shaders, "[ shaders ] Number of return shaders ptr must be non null.\n");
    uAssertMsg_v(!(*num_return_shaders), "[ shaders ] Number of return shaders must be zero; will be overwritten.\n");

    // Note: not freed in this scope
    *return_shaders                      = ( uVulkanShader* )calloc(num_shader_infos, sizeof(uVulkanShader));
    const char return_shader_alloc_err[] = "[ shaders ] Unable to allocate uVulkanShader array.\n";
    uAssertMsg_v(return_shaders, return_shader_alloc_err);
    if (!return_shaders) { uFatal(return_shader_alloc_err); }

    uDebugPrint("Loading %zd SPIR-V files...\n", num_shader_infos);
    for (u32 shader_idx = 0; shader_idx < num_shader_infos; shader_idx++)
    {
        // Validate incoming shader info
        uAssertMsg_v(( const char* )(kShadersToLoad[shader_idx].spv_path), "[ shaders ] Shader spv file path must not be null.\n");
        uAssertMsg_v(uValidateVulkanShaderType(kShadersToLoad[shader_idx].type), "[ shaders ] Shader type must be valid.\n");

        uVulkanShader* return_shader = &(kShadersToLoad[shader_idx]);
        uReadSpirvFile(return_shader);

        // Create vulkan shader module
        uCreateVulkanShaderModule(return_shader);
        uAssertMsg_v(return_shader->module, "[ shaders ] Shader module failed creation.\n");

        // Copy newly created uVulkanShader to return_shaders array
        return_shaders[shader_idx] = ( uVulkanShader* )return_shader;
        (*num_return_shaders)++;
    }
}

#endif // __UE_SHADER_TOOLS__
