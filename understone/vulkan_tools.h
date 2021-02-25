// [ cfarvin::TODO ] Something like this would be REALLY helpful in debugging in
// the appropriate
//                   function header contexts:
//                   uDebugStatement(static char times_called = 1);
//                   uDebugStatement(const u8 expected_times_called = <X>);
//                   uDebugStatement(times_called++);
//                   uDebugStatement(if(times_called > expected_times_called)
//                   uAssertMsg_v(...) );

#ifndef __UE_VULKAN_TOOLS_H__
#define __UE_VULKAN_TOOLS_H__

#if _WIN32
#define VK_USE_PLATFORM_WIN32_KHR 1
#endif // _WIN32

#include "vulkan/vulkan.h"

#if _WIN32
#include <understone/win_platform.h>
#endif // _WIN32

#include "engine_info.h"
#include "memory_tools.h"
#include "render_tools.h"
#include "shader_tools.h"
#include "vulkan_macros.h"
#include "vulkan_types.h"
#include "window_tools.h"

extern const u8 kNumVulkanCommandBuffers;

// [ cfarvin::REMOVE ] Remove stdio.h
#include <stdio.h>

void
uDestroyVulkan();

// [ cfarvin::TODO ] Remove all singletons from function parameter calls.

void
uQueryVulkanDeviceExtensions(const VkPhysicalDevice* const restrict physical_device,
                             const char** const restrict            user_device_extension_names,
                             const u16                              num_user_device_extension_names,
                             u16* const restrict                    num_verified_extension_names);

__UE_inline__ bool
uSelectVulkanSwapChain(const uGameWindow* const restrict game_window);

__UE_inline__ void
uCreateVulkanSwapChain(const uGameWindow* const restrict);

__UE_inline__ void
uCreateVulkanImageViews();

__UE_inline__ void
uCreateVulkanRenderPass();

void
uCreateVulkanGraphicsPipeline(const size_t                                          num_shader_stage_create_infos,
                              const VkPipelineShaderStageCreateInfo* const restrict shader_stage_create_infos);

void
uCreateVulkanFrameBuffers();

void
uCreateVulkanCommandBuffers();

bool
uValidateVulkanSwapChainAndSurfaceCompatibility(const VkPhysicalDevice             potential_physical_device,
                                                const VkSurfaceKHR                 potential_surface,
                                                u32* const restrict                return_num_surface_formats,
                                                VkSurfaceFormatKHR* const restrict return_surface_format,
                                                u32* const restrict                return_num_present_modes,
                                                VkPresentModeKHR* const restrict   return_present_modes);

__UE_inline__ void
uResetOldSwapChain()
{
    uVkVerbose("Resetting old swap chain components...\n");

    uVulkanImageGroup* image_group = ( uVulkanImageGroup* )uGetVulkanImageGroup();
    uAssertMsg_v(image_group, "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");
    uAssertMsg_v(image_group->swap_chain, "[ vulkan ] Image group swap chain ptr must be non null.\n");

    VkSwapchainKHR old_swap_chain = image_group->swap_chain;

    // [ cfarvin::REVISIT ] [ SWAPCHAIN ] Still plan on doing this, just getting
    // compilation working.
    /* uPrepImageGroupForSwapchainRebuild(); */
    /* uPrepCommandBuffersForSwapchainRebuild(); */
    /* uPrepRenderToolsForSwapchainRebuild(); */

    // [ cfarvin::TODO ] You're always confused about where some of these objects
    // are. Are they
    //                   in render_tools? Maybe vulkan_tools? Perhaps
    //                   vulkan_types? Solve this please.
    // [ cfarvin::TODO ] Remove these when the "prep" functions above are
    // implemented correctly.
    /* uResetVulkanImageGroup(); */
    /* uDestroyVulkanCommandBuffers(); */
    /* uResetVulkanRenderInfo(); */

    // Image group was reset.
    image_group                          = ( uVulkanImageGroup* )uGetVulkanImageGroup();
    VkSwapchainKHR* non_const_swap_chain = ( VkSwapchainKHR* )&(image_group->old_swap_chain);
    *non_const_swap_chain                = old_swap_chain;
}

// Note: Considered undefined behavior to call when game window is minimized.
__UE_inline__ void
uRebuildVulkanSwapChain(const VkPhysicalDevice                                physical_device,
                        const VkDevice                                        logical_device,
                        const VkInstance                                      instance,
                        const VkPipelineShaderStageCreateInfo* const restrict shader_stage_create_infos,
                        const size_t                                          num_shader_stage_create_infos,
                        const uGameWindow* const restrict                     game_window)
{
    // [ cfarvin::REVISIT ] I removed "is_rebuilding_swap_chain" parameter from
    // uValidateVulkanSwapChainAndSurfaceCompatibility and commented out some code
    // which freed the surface_info->present_modes and
    // surface_info->surface_formats (when rebuilding). We may need to handle this
    // here or elsewhere, since it is no longer handled there.

    // [ cfarvin::NOTE ] [ SWAPCHAIN ] Since we're working on using the old
    // swapchain data, building a new
    //                   swapchain before we destroy the old one - maybe - just
    //                   maybe - we dont' need to wait for the device to be idle
    //                   as is done below. Glances at the documentation seem to
    //                   support this (saying that images MAY be used from the old
    //                   swapchain).
    /* uVulkanInfo* v_info = ( uVulkanInfo* )uGetVulkanInfo(); */
    /* vkDeviceWaitIdle(v_info->logical_device); */

    uVkVerbose("[ begin ] swap chain rebuild\n");
    uResetOldSwapChain();

    // Rebuild swap chain
    VkSurfaceKHR        new_surface             = NULL;
    u32                 new_num_surface_formats = 0;
    VkSurfaceFormatKHR* new_surface_formats     = NULL;
    u32                 new_num_present_modes   = 0;
    VkPresentModeKHR*   new_present_modes       = NULL;

    //
    // [ cfarvin::REVISIT ] you were about to continue filling in the vulkan
    // objects above
    //                      starting with vksurfacekhr that are going to be needed
    //                      to create a brand new swapchain. You only stopped to
    //                      go through the functions towards the bottom of this
    //                      function, in order to make versions of them which do
    //                      not rely upon our singleton system.
    //

    bool swap_chain_surface_compatible = uValidateVulkanSwapChainAndSurfaceCompatibility(physical_device,
                                                                                         new_surface,
                                                                                         &new_num_surface_formats,
                                                                                         new_surface_formats,
                                                                                         &new_num_present_modes,
                                                                                         new_present_modes);

    const char swap_chain_surface_error[] = "[ vulkan ] The swap chain and surface were found to be incompatible.\n";
    uAssertMsg_v(swap_chain_surface_compatible, swap_chain_surface_error);
    if (!swap_chain_surface_compatible)
    {
        uFatal(swap_chain_surface_error);
    }

    uCreateVulkanSwapChain(game_window);
    uCreateVulkanImageViews();
    uCreateVulkanRenderPass();
    uCreateVulkanGraphicsPipeline(num_shader_stage_create_infos, shader_stage_create_infos);
    uCreateVulkanFrameBuffers();
    uCreateVulkanCommandBuffers();

    uVkVerbose("[ end ] swap chain rebuild\n");
    uDebugStatement(fflush(stdout));
}

void
uCreateVulkanCommandBuffers()
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanCommandInfo* command_info = ( uVulkanCommandInfo* )uGetVulkanCommandInfo();
    uVulkanImageGroup*  image_group  = ( uVulkanImageGroup* )uGetVulkanImageGroup();
    uVulkanRenderInfo*  render_info  = ( uVulkanRenderInfo* )uGetVulkanRenderInfo();
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkaninfo ptr must be non null.\n");
    uAssertMsg_v(v_info->physical_device, "[ vulkan ] VkPhysicalDevice must be non zero.\n");
    uAssertMsg_v(command_info, "[ vulkan ] uVulkanCommandInfo ptr must be non null.\n");
    uAssertMsg_v(command_info->command_pool, "[ vulkan ] VkCommandPool must be non zero.\n");
    uAssertMsg_v(image_group, "[ vulkan ] uVulkanImageGroup ptr must be non zero.\n");
    uAssertMsg_v(image_group->num_images, "[ Vulkan ] Image count must be non zero.\n");
    uAssertMsg_v(render_info, "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");
    uAssertMsg_v(render_info->render_pass, "[ vulkan ] VkRenderPass must be non zero.\n");
    uAssertMsg_v(render_info->graphics_pipeline, "[ vulkan ] VkPipeline ptr must be non zero.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo must be non null.\n");

    // Create command buffers
    if (image_group->num_images > kNumVulkanCommandBuffers)
    {
        uFatal("[ api ] More swap chain images than command buffers: (%d, %d).\n",
               image_group->num_images,
               kNumVulkanCommandBuffers);
    }

    *( VkCommandBuffer** )&(command_info->command_buffers) =
      ( VkCommandBuffer* )calloc(image_group->num_images, sizeof(VkCommandBuffer));

    if (!command_info->command_buffers)
    {
        uFatal("[ vulkan ] Unable to allocate command buffers.\n");
    }

    // Note: cmd buffer count set in struct for outside use; image number used
    // internally.
    *( u32* )&(command_info->num_command_buffers) = image_group->num_images;

    VkCommandBufferAllocateInfo cb_alloc_create_info = {};
    cb_alloc_create_info.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cb_alloc_create_info.commandPool                 = command_info->command_pool;
    cb_alloc_create_info.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cb_alloc_create_info.commandBufferCount          = image_group->num_images;

    VkResult result = vkAllocateCommandBuffers(v_info->logical_device,
                                               &cb_alloc_create_info,
                                               ( VkCommandBuffer* )command_info->command_buffers);

    if (result != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to create command buffers.\n");
    }

    for (u32 image_idx = 0; image_idx < image_group->num_images; image_idx++)
    {
        // Begin cb recording
        VkCommandBufferBeginInfo cb_begin_info = {};
        cb_begin_info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        cb_begin_info.flags                    = 0;
        cb_begin_info.pInheritanceInfo         = NULL;

        result = vkBeginCommandBuffer(command_info->command_buffers[image_idx], &cb_begin_info);

        if (result != VK_SUCCESS)
        {
            uFatal("[ vulkan ] Unable to begin command buffer at index: %d.\n", image_idx);
        }

        // Begin render passs
        VkClearValue clear_value     = {};
        clear_value.color.float32[3] = 1.0f;

        VkRenderPassBeginInfo rp_begin_info = {};
        rp_begin_info.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        rp_begin_info.renderPass            = render_info->render_pass;
        rp_begin_info.framebuffer           = image_group->frame_buffers[image_idx];
        rp_begin_info.renderArea.offset.x   = 0;
        rp_begin_info.renderArea.offset.y   = 0;
        rp_begin_info.renderArea.extent     = surface_info->surface_capabilities.currentExtent;
        rp_begin_info.clearValueCount       = 1;
        rp_begin_info.pClearValues          = &clear_value;

        vkCmdBeginRenderPass(command_info->command_buffers[image_idx], &rp_begin_info, VK_SUBPASS_CONTENTS_INLINE);

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
            uFatal("[ vulkan ] Unable to end command buffer.\n");
        }
    }
}

void
uCreateVulkanCommandPool()
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanCommandInfo* command_info = ( uVulkanCommandInfo* )uGetVulkanCommandInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkaninfo ptr must be non null.\n");
    uAssertMsg_v(v_info->physical_device, "[ vulkan ] VkPhysicalDevice must be non zero.\n");
    uAssertMsg_v(command_info, "[ vulkan ] uVulkanCommandInfo ptr must be non zero.\n");

    // Create graphics command pool
    VkCommandPoolCreateInfo command_pool_create_info = {};
    command_pool_create_info.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.queueFamilyIndex        = uVULKAN_GRAPHICS_QUEUE_INDEX;
    command_pool_create_info.flags                   = 0;

    VkResult result = vkCreateCommandPool(v_info->logical_device,
                                          &command_pool_create_info,
                                          NULL,
                                          ( VkCommandPool* )&(command_info->command_pool));
    if (result != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to create command pool.\n");
    }
}

void
uCreateVulkanFrameBuffers()
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();
    uVulkanImageGroup*  image_group  = ( uVulkanImageGroup* )uGetVulkanImageGroup();
    uVulkanRenderInfo*  render_info  = ( uVulkanRenderInfo* )uGetVulkanRenderInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkPhysicalDevice must be non zero.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(image_group, "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");
    uAssertMsg_v(render_info, "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");

    // [ cfarvin::TODO ] We're not doing things this way any more, but the check
    // for null frame buffers is still valid when where rebuilding the
    //                   swap chain. Need to implement this in the right place.
    /* uAssertMsg_v(is_rebuilding_swap_chain || !image_group->frame_buffers, "[
     * vulkan ] VkFrameBuffer ptr must be null; will be overwritten.\n"); */

    // Create a frame buffer for each image view
    *( VkFramebuffer** )&(image_group->frame_buffers) =
      ( VkFramebuffer* )calloc(image_group->num_images, sizeof(VkFramebuffer));
    if (!(image_group->frame_buffers))
    {
        uFatal("[ vulkan ] Unable to allocate frame buffer array.\n");
    }

    for (u32 image_idx = 0; image_idx < image_group->num_images; image_idx++)
    {
        VkFramebufferCreateInfo frame_buffer_create_info = {};
        frame_buffer_create_info.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frame_buffer_create_info.renderPass              = render_info->render_pass;
        frame_buffer_create_info.attachmentCount         = 1;
        frame_buffer_create_info.pAttachments            = &(image_group->image_views[image_idx]);
        frame_buffer_create_info.width                   = surface_info->surface_capabilities.currentExtent.width;
        frame_buffer_create_info.height                  = surface_info->surface_capabilities.currentExtent.height;
        frame_buffer_create_info.layers                  = 1; // Non stereoscopic

        VkResult result = vkCreateFramebuffer(v_info->logical_device,
                                              &frame_buffer_create_info,
                                              NULL,
                                              &(image_group->frame_buffers[image_idx]));

        if (result != VK_SUCCESS)
        {
            uFatal("[ vulkan ] Unable to create frame buffer.\n");
        }
    }
}

// [ cfarvin::TODO ] When we are rendering w/ more than one attachment:
//                   Update this function to take the number and type of
//                   attachments.
__UE_inline__ void
uCreateVulkanRenderPass()
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();
    uVulkanRenderInfo*  render_info  = ( uVulkanRenderInfo* )uGetVulkanRenderInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info->surface_formats, "[ vulkan ] VkSurfaceFormatsKHR ptr must be non null.\n");
    uAssertMsg_v(render_info, "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");

    // [ cfarvin::TODO ] Account for these checks in the new system
    /* uAssertMsg_v(is_rebuilding_swap_chain ||
     * !render_info->attachment_descriptions, */
    /*              "[ vulkan ] VkAttachmentDesctiption ptr must be null; will be
     * " */
    /*              "overwritten.\n"); */
    /* uAssertMsg_v(is_rebuilding_swap_chain ||
     * !render_info->attachment_references, */
    /*              "[ vulkan ] VkAttachmentReference ptr must be null; will be "
     */
    /*              "overwritten.\n"); */

    *( u32* )&render_info->num_attachments = 1;
    *( VkAttachmentDescription** )&(render_info->attachment_descriptions) =
      ( VkAttachmentDescription* )calloc(render_info->num_attachments, sizeof(VkAttachmentDescription));
    if (!render_info->attachment_descriptions)
    {
        uFatal("[ vulkan ] Unable to allocate attachment descriptions.\n");
    }

    *( VkAttachmentReference** )&(render_info->attachment_references) =
      ( VkAttachmentReference* )calloc(render_info->num_attachments, sizeof(VkAttachmentReference));
    if (!render_info->attachment_references)
    {
        uFatal("[ vulkan ] Unable to allocate attachment references.\n");
    }

    VkSurfaceFormatKHR surface_format = (surface_info->surface_formats)[surface_info->designated_format_index];

    for (u32 attachment_idx = 0; attachment_idx < render_info->num_attachments; attachment_idx++)
    {
        // Attachement description
        VkAttachmentDescription* attachment_descriptions =
          *( VkAttachmentDescription** )&(render_info->attachment_descriptions);

        attachment_descriptions[attachment_idx].format  = surface_format.format;
        attachment_descriptions[attachment_idx].samples = VK_SAMPLE_COUNT_1_BIT; // Note: 1 == No MSAA
        attachment_descriptions[attachment_idx].loadOp =
          VK_ATTACHMENT_LOAD_OP_CLEAR; // [ cfarvin::PERF ] expensive or nominal?
        attachment_descriptions[attachment_idx].storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        attachment_descriptions[attachment_idx].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment_descriptions[attachment_idx].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment_descriptions[attachment_idx].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment_descriptions[attachment_idx].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        // Attachment reference
        VkAttachmentReference* color_attachment_reference =
          *( VkAttachmentReference** )&(render_info->attachment_references);

        color_attachment_reference[attachment_idx].attachment = 0; // Read: "Attachment index"
        color_attachment_reference[attachment_idx].layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }

    // Subpasses
    // Note: References layout(location = 0) in frag shader.
    VkSubpassDependency subpass_dependency = {};
    subpass_dependency.srcSubpass          = VK_SUBPASS_EXTERNAL; // Indicates this is the first subpass to be run
    subpass_dependency.dstSubpass          = 0;                   // Index of this subpass in the pSubpasses array
    subpass_dependency.srcStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // Stage to wait on.
    subpass_dependency.srcAccessMask       = 0; // Types of operations to wait on in that ^ stage.
    subpass_dependency.dstStageMask        = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // Stage to wait on.
    subpass_dependency.dstAccessMask       = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; // Types of operations to wait on in
                                                                                   // that ^ stage.

    VkSubpassDescription subpass_description = {};
    subpass_description.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass_description.colorAttachmentCount = render_info->num_attachments;
    subpass_description.pColorAttachments    = render_info->attachment_references;

    // Render pass
    VkRenderPassCreateInfo render_pass_create_info = {};
    render_pass_create_info.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.attachmentCount        = render_info->num_attachments;
    render_pass_create_info.pAttachments           = render_info->attachment_descriptions;
    render_pass_create_info.subpassCount           = 1;
    render_pass_create_info.pSubpasses             = &subpass_description;
    render_pass_create_info.dependencyCount        = 1;
    render_pass_create_info.pDependencies          = &subpass_dependency;

    VkResult result = vkCreateRenderPass(v_info->logical_device,
                                         &render_pass_create_info,
                                         NULL,
                                         ( VkRenderPass* )&(render_info->render_pass));

    if (result != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to create render pass.\n");
    }
}

// [ cfarvin::TODO ] It may be that each application needs to implement this
// function on their own,
//                   and sometimes with different attributes for the vk
//                   structures within. In that case, the engine could at least
//                   provide default versions of these structures.
void
uCreateVulkanGraphicsPipeline(const size_t                                          num_shader_stage_create_infos,
                              const VkPipelineShaderStageCreateInfo* const restrict shader_stage_create_infos)
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();
    uVulkanRenderInfo*  render_info  = ( uVulkanRenderInfo* )uGetVulkanRenderInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkPhysical device must be non zero.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(render_info, "[ vulkan ] uVulkanRenderInfo ptr must be non null.\n");

    // [ cfarvin::TODO ] Added fn params, but have not yet added uassertmsg checks

    // Vertex input
    VkPipelineVertexInputStateCreateInfo vertex_input_create_info = {};
    vertex_input_create_info.sType                         = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_create_info.vertexBindingDescriptionCount = 0;
    vertex_input_create_info.pVertexBindingDescriptions    = NULL;
    vertex_input_create_info.vertexAttributeDescriptionCount = 0;
    vertex_input_create_info.pVertexAttributeDescriptions    = NULL;

    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info = {};
    input_assembly_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_create_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

    // Viewport
    VkViewport frame_buffer_viewport = {};
    frame_buffer_viewport.x          = 0.0f;
    frame_buffer_viewport.y          = 0.0f;
    frame_buffer_viewport.width      = ( r32 )surface_info->surface_capabilities.currentExtent.width;
    frame_buffer_viewport.height     = ( r32 )surface_info->surface_capabilities.currentExtent.height;
    frame_buffer_viewport.minDepth   = 0.0f;
    frame_buffer_viewport.maxDepth   = 1.0f;

    // Scissor
    VkRect2D frame_buffer_scissor = {};
    frame_buffer_scissor.offset.x = 0;
    frame_buffer_scissor.offset.y = 0;
    frame_buffer_scissor.extent   = surface_info->surface_capabilities.currentExtent;

    // Viewport state
    VkPipelineViewportStateCreateInfo frame_buffer_viewport_create_info = {};
    frame_buffer_viewport_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    frame_buffer_viewport_create_info.viewportCount = 1;
    frame_buffer_viewport_create_info.pViewports    = &frame_buffer_viewport;
    frame_buffer_viewport_create_info.scissorCount  = 1;
    frame_buffer_viewport_create_info.pScissors     = &frame_buffer_scissor;

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo raster_create_info = {};
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
    VkPipelineMultisampleStateCreateInfo multi_sample_create_info = {};
    multi_sample_create_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multi_sample_create_info.sampleShadingEnable   = VK_FALSE;
    multi_sample_create_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
    multi_sample_create_info.minSampleShading      = 1.0f;
    multi_sample_create_info.pSampleMask           = NULL;
    multi_sample_create_info.alphaToCoverageEnable = VK_FALSE;
    multi_sample_create_info.alphaToOneEnable      = VK_FALSE;

    // Depth stencil [ currently off ]
    VkPipelineDepthStencilStateCreateInfo* depth_stencil_create_info = NULL;

    // Color blending
    // Note: VkPipelineColorBlendStateCreateInfo builds global state.
    //       VkPipelineColorBlendAttachmentState sets local state per attached
    //       frame buffer.
    VkPipelineColorBlendAttachmentState color_blend_attachment = {};
    color_blend_attachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable         = VK_FALSE;
    color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_blend_attachment.colorBlendOp        = VK_BLEND_OP_ADD;
    color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment.alphaBlendOp        = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo color_blend_create_info = {};
    color_blend_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_create_info.logicOpEnable     = VK_FALSE;
    color_blend_create_info.logicOp           = VK_LOGIC_OP_COPY;
    color_blend_create_info.attachmentCount   = 1;
    color_blend_create_info.pAttachments      = &color_blend_attachment;
    color_blend_create_info.blendConstants[0] = 0.0f;
    color_blend_create_info.blendConstants[1] = 0.0f;
    color_blend_create_info.blendConstants[2] = 0.0f;
    color_blend_create_info.blendConstants[3] = 0.0f;

    // Dynamic state
    // Note: Causes previous configuration to be ignored, and information to be
    // supplied at drawing time.
    VkDynamicState dynamic_states[2] = {};
    dynamic_states[0]                = VK_DYNAMIC_STATE_VIEWPORT;
    dynamic_states[1]                = VK_DYNAMIC_STATE_LINE_WIDTH;

    VkPipelineDynamicStateCreateInfo dynamic_state_create_info = {};
    dynamic_state_create_info.sType                            = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_create_info.dynamicStateCount                = 2;
    dynamic_state_create_info.pDynamicStates                   = dynamic_states;

    // Pipeline layout
    VkPipelineLayoutCreateInfo pipeline_layout_create_info = {};
    pipeline_layout_create_info.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.setLayoutCount             = 0;
    pipeline_layout_create_info.pSetLayouts                = NULL;
    pipeline_layout_create_info.pushConstantRangeCount     = 0;
    pipeline_layout_create_info.pPushConstantRanges        = NULL;

    VkResult result = vkCreatePipelineLayout(v_info->logical_device,
                                             &pipeline_layout_create_info,
                                             NULL,
                                             ( VkPipelineLayout* )&(render_info->pipeline_layout));
    if (result != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to create pipeline layout.\n");
    }

    // Create graphics pipeline
    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {};
    graphics_pipeline_create_info.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

    graphics_pipeline_create_info.stageCount          = num_shader_stage_create_infos;
    graphics_pipeline_create_info.pStages             = shader_stage_create_infos;
    graphics_pipeline_create_info.pVertexInputState   = &vertex_input_create_info;
    graphics_pipeline_create_info.pInputAssemblyState = &input_assembly_create_info;
    graphics_pipeline_create_info.pViewportState      = &frame_buffer_viewport_create_info;
    graphics_pipeline_create_info.pRasterizationState = &raster_create_info;
    graphics_pipeline_create_info.pMultisampleState   = &multi_sample_create_info;
    graphics_pipeline_create_info.pDepthStencilState  = depth_stencil_create_info; // Currently off
    graphics_pipeline_create_info.pColorBlendState    = &color_blend_create_info;
    graphics_pipeline_create_info.pDynamicState       = NULL; // Filled out, not using.
    graphics_pipeline_create_info.layout              = render_info->pipeline_layout;
    graphics_pipeline_create_info.renderPass          = render_info->render_pass;
    graphics_pipeline_create_info.subpass             = 0; // Read: "Subpass index"
    graphics_pipeline_create_info.basePipelineHandle  = NULL;
    graphics_pipeline_create_info.basePipelineIndex   = -1;

    result = vkCreateGraphicsPipelines(v_info->logical_device,
                                       VK_NULL_HANDLE,
                                       1,
                                       &graphics_pipeline_create_info,
                                       NULL,
                                       ( VkPipeline* )&(render_info->graphics_pipeline));

    if (result != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to create graphics pipeline.\n");
    }
}

__UE_inline__ void
uCreateVulkanImageViews()
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();
    uVulkanImageGroup*  image_group  = ( uVulkanImageGroup* )uGetVulkanImageGroup();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkDevice ptr must be non null.\n");
    uAssertMsg_v(image_group, "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");
    uAssertMsg_v(image_group->images, "[ vulkan ] VkImages ptr must be non null.\n");
    uAssertMsg_v(image_group->num_images, "[ vulkan ] VkImage quantity must be non zero.\n");
    uAssertMsg_v(image_group->image_views, "[ vulkan ] VkImages ptr must be non null.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo must be non null.\n");
    uAssertMsg_v(surface_info->surface_formats, "[ vulkan ] VkSurfaceFormatKHR ptr must be non null.\n");

    VkSurfaceFormatKHR* surface_formats = ( VkSurfaceFormatKHR* )surface_info->surface_formats;
    for (u32 image_idx = 0; image_idx < image_group->num_images; image_idx++)
    {
        // Fill create info
        VkImageViewCreateInfo image_view_create_info = {};

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
            uFatal("[ vulkan ] Unable to create image view.\n");
        }
    }
}

void
uVulkanExtractUniqueQueueFamilies(const uVulkanQueueInfo* const restrict queue_info,
                                  u32* const restrict                    unique_queue_array,
                                  const u8                               num_possible_queues,
                                  u8* const restrict                     unique_queues_found)
{
    uAssertMsg_v(queue_info, "[ vulkan ] The uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(unique_queue_array, "[ vulkan ] The 'unique_queue_array' ptr must be non null.\n");
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
                for (u8 unique_queue_idx = 0; unique_queue_idx < (*unique_queues_found); unique_queue_idx++)
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

__UE_inline__ void
uCreateVulkanSwapChainCore(const VkDevice                           logical_device,
                           const VkSurfaceKHR                       surface,
                           const VkSurfaceFormatKHR* const restrict surface_formats,
                           const VkPresentModeKHR* const restrict   present_modes,
                           const VkSurfaceCapabilitiesKHR           surface_capabilities,
                           const u32                                graphics_queue_index,
                           const u32                                present_queue_index,
                           const uGameWindow* const restrict        game_window)
{
    // Select a suitable swapchain
    bool swap_chain_selected = uSelectVulkanSwapChain(game_window);
    if (!swap_chain_selected)
    {
        uFatal("[ vulkan ] Unable to select a suitable swap chain.\n");
    }

    //
    // [ cfarvin::RETURN ] You were just about to fix some compilation errors
    // while pulling out a
    //                     version of each function involved in swap chain
    //                     recreation dedicated to both the first creation (using
    //                     singleton patterns) and the re-creation (using raw
    //                     vulkan objects). May be a good idea to review your [
    //                     cfarvin::<> ] notes through this document upon return,
    //                     as you've commented and altered a lot of things out but
    //                     have strongly documented those changes each time. Good
    //                     luck! Talking to yourself seems to really, really,
    //                     help. You made awesome progress this morning and should
    //                     feel wonderful about it. <3
    //
    // Determine swap chain image capacity
    u32 min_image_count = surface_info->surface_capabilities.minImageCount;
    u32 max_image_count = surface_info->surface_capabilities.maxImageCount;

    // [ cfarvin::TODO ] Should the designated image count be the number of frames
    // in flight?
    //                   If so, we need to link those two varaibles; they could be
    //                   completely different values presently.

    // Request an additional image
    u32 designated_image_count = min_image_count + 1;

    // Ensure designated count is within bounds
    bool image_count_unlimited = (max_image_count == 0);
    if (!image_count_unlimited && (designated_image_count > max_image_count))
    {
        designated_image_count = max_image_count;
    }

    // Determine if queues are unique
    u32 unique_queue_array[uVULKAN_NUM_QUEUES] = {};
    u8  unique_queues_found                    = 0;
    uVulkanExtractUniqueQueueFamilies(queue_info, &(unique_queue_array[0]), uVULKAN_NUM_QUEUES, &unique_queues_found);

    if (!unique_queues_found)
    {
        uFatal("[ vulkan ] Was unable to find any queues.\n");
    }

    const VkSurfaceFormatKHR* surface_formats = surface_info->surface_formats;
    const VkPresentModeKHR*   present_modes   = surface_info->present_modes;

    // File create info
    VkSwapchainCreateInfoKHR swap_chain_create_info = {};
    swap_chain_create_info.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swap_chain_create_info.surface                  = surface_info->surface;
    swap_chain_create_info.minImageCount            = designated_image_count;
    swap_chain_create_info.imageFormat              = surface_formats[surface_info->designated_format_index].format;
    swap_chain_create_info.imageColorSpace          = surface_formats[surface_info->designated_format_index].colorSpace;
    swap_chain_create_info.imageExtent              = surface_info->surface_extent;
    swap_chain_create_info.imageArrayLayers         = 1;
    swap_chain_create_info.imageUsage               = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // Render directly to images
    swap_chain_create_info.preTransform             = surface_info->surface_capabilities.currentTransform;
    swap_chain_create_info.compositeAlpha           = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Ignore alpha channel
    swap_chain_create_info.presentMode              = present_modes[surface_info->designated_present_index];
    swap_chain_create_info.clipped                  = VK_TRUE;
    swap_chain_create_info.oldSwapchain             = image_group->old_swap_chain;

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
                                           ( VkSwapchainKHR* )&(image_group->swap_chain));

    if (result != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to create swap chain.\n");
    }

    // Get handles to swap chain images
    u32 reported_image_count = designated_image_count;
    result = vkGetSwapchainImagesKHR(v_info->logical_device, image_group->swap_chain, &designated_image_count, NULL);

    if (result != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to obtain swap chain image count.\n");
    }

    if (!reported_image_count)
    {
        uFatal("[ vulkan ] Swap chain reports no images.\n");
    }

    if (designated_image_count != reported_image_count)
    {
        uWarning("[ vulkan ] Swap chain images count != designated count.\n");
        designated_image_count = reported_image_count;
    }

    image_group->images      = ( VkImage* )calloc(designated_image_count, sizeof(VkImage));
    image_group->image_views = ( VkImageView* )calloc(designated_image_count, sizeof(VkImageView));
    result                   = vkGetSwapchainImagesKHR(v_info->logical_device,
                                     image_group->swap_chain,
                                     &designated_image_count,
                                     image_group->images);

    if (result != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to set swap chain image count handle(s).\n");
    }

    // Set image count
    *( u32* )&(image_group->num_images) = designated_image_count;
}

__UE_inline__ void
uCreateVulkanSwapChain(const uGameWindow* const restrict game_window)
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanQueueInfo*   queue_info   = ( uVulkanQueueInfo* )uGetVulkanQueueInfo();
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();
    uVulkanImageGroup*  image_group  = ( uVulkanImageGroup* )uGetVulkanImageGroup();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->logical_device, "[ vulkan ] VkLogicalDevice must be non zero.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info->surface, "[ vulkan ] VkSurfaceKHR ptr must be non null.\n");
    uAssertMsg_v(queue_info, "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info->surface_formats, "[ vulkan ] VkSurfaceFormatKHR ptr must be non null.\n");
    uAssertMsg_v(surface_info->present_modes, "[ vulkan ] VkPresentModeKHR ptr must be non null.\n");
    uAssertMsg_v(image_group, "[ vulkan ] uVulkanImageGroup ptr must be non null.\n");
    uAssertMsg_v(image_group->swap_chain, "[ vulkan ] Swap chain ptr must be non null.\n");
    uAssertMsg_v(!(*image_group->swap_chain), "[ vulkan ] Swap chain must be null; will be overwritten.\n");
    uAssertMsg_v(image_group->images, "[ vulkan ] Images ptr must be non null.\n");
    uAssertMsg_v(!(*image_group->images), "[ vulkan ] Images must be null; will be overwritten.\n");
    uAssertMsg_v(image_group->image_views, "[ vulkan ] Image views ptr must be non null.\n");
    uAssertMsg_v(!(*image_group->image_views), "[ vulkan ] Image views must be null; will be overwritten.\n");

    uCreateVulkanSwapChainCore(v_info->logical_device,
                               surface_info->surface,
                               surface_info->surface_formats,
                               surface_info->present_modes,
                               surface_info->surface_capabilities,
                               surface_info->surface_extent,
                               queue_info->designated_graphics_index,
                               queue_info->designated_present_index,
                               game_window);
}

void
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
uCreateVulkanLogicalDevice(const char** const restrict instance_validation_layer_names,
                           const u16                   num_instance_validation_layer_names,
#else
uCreateVulkanLogicalDevice(
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                           const char** const restrict user_device_extension_names,
                           const u16                   num_user_device_extension_names)
{
    uVulkanInfo*      v_info     = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanQueueInfo* queue_info = ( uVulkanQueueInfo* )uGetVulkanQueueInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->physical_device, "[ vulkan ] VkPhysicalDevice must be non zero.\n");
    uAssertMsg_v(!v_info->logical_device, "[ vulkan ] VkDevice must be zero; will be overwritten.\n");
    uAssertMsg_v(queue_info, "[ vulkan ] Queue indices ptr must be non null\n");
    uAssertMsg_v(queue_info->queues, "[ vulkan ] VkQueue ptr must be non null.\n");

#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    if (num_instance_validation_layer_names)
    {
        uAssertMsg_v(instance_validation_layer_names && *instance_validation_layer_names,
                     "[ vulkan ] If the instance validation layer quanitity is non "
                     "zero, the names ptr must be non null\n");
    }
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

    if (num_user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, "
                     "the names ptr must be non null\n");
    }

    // Extract unique queue family index values from uVulkanQueueInfo
    u8  num_unique_queues                 = 0;
    u32 unique_queues[uVULKAN_NUM_QUEUES] = {};
    uVulkanExtractUniqueQueueFamilies(queue_info, &(unique_queues[0]), uVULKAN_NUM_QUEUES, &num_unique_queues);

    // Create logical device create info structure(s)
    VkDeviceQueueCreateInfo* device_queue_create_infos =
      ( VkDeviceQueueCreateInfo* )calloc(num_unique_queues, sizeof(VkDeviceQueueCreateInfo));
    if (!device_queue_create_infos)
    {
        uFatal("[ vulkan ] Unable to create logical device.\n");
    }

    for (u32 queue_family_idx = 0; queue_family_idx < num_unique_queues; queue_family_idx++)
    {
        r32 device_queue_priorities                                  = 1.0f;
        device_queue_create_infos[queue_family_idx].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        device_queue_create_infos[queue_family_idx].queueFamilyIndex = queue_family_idx;
        device_queue_create_infos[queue_family_idx].queueCount       = 1;

        // All queues have equal priority.
        device_queue_create_infos[queue_family_idx].pQueuePriorities = &device_queue_priorities;
    }

    // [ cfarvin::TODO ] Added the vkGetPhysicalDeviceFeatures call a while after
    // the original
    //                   implementation. I think this was an oversight originally;
    //                   let's be sure. If there are sudden errors in this part of
    //                   the code; this could be why.
    VkPhysicalDeviceFeatures physical_device_features = {};
    vkGetPhysicalDeviceFeatures(v_info->physical_device, *physical_device_features);

    VkDeviceCreateInfo logical_device_create_info   = {};
    logical_device_create_info.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    logical_device_create_info.pQueueCreateInfos    = device_queue_create_infos;
    logical_device_create_info.queueCreateInfoCount = num_unique_queues;
    logical_device_create_info.pEnabledFeatures     = &physical_device_features;

#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    logical_device_create_info.enabledLayerCount   = num_instance_validation_layer_names;
    logical_device_create_info.ppEnabledLayerNames = ( const char** const )instance_validation_layer_names;
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

    logical_device_create_info.enabledExtensionCount   = num_user_device_extension_names;
    logical_device_create_info.ppEnabledExtensionNames = ( const char** const )user_device_extension_names;

    VkResult device_creation_success =
      vkCreateDevice(v_info->physical_device, &logical_device_create_info, NULL, ( VkDevice* )&v_info->logical_device);

    if (device_creation_success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to create logical device.\n");
    }

    vkGetDeviceQueue(v_info->logical_device,
                     queue_info->designated_graphics_index,
                     0,
                     ( VkQueue* )(&(queue_info->queues)[uVULKAN_GRAPHICS_QUEUE_INDEX]));

    vkGetDeviceQueue(v_info->logical_device,
                     queue_info->designated_present_index,
                     0,
                     ( VkQueue* )(&(queue_info->queues)[uVULKAN_PRESENT_QUEUE_INDEX]));

    if (device_queue_create_infos)
    {
        free(device_queue_create_infos);
    }
}

// [ cfarvin::TODO ] Define device feature requirements
bool
uValidateVulkanDeviceFeaturesReqruirement(const VkPhysicalDevice physical_device)
{
    if (physical_device)
    {
    }

    // Aquire physical device features
    /* VkPhysicalDeviceFeatures device_features; */
    /* vkGetPhysicalDeviceFeatures(physical_device, */
    /*                             &device_features); */

    return true;
}

// [ cfarvin::TODO ] Define device property requirements
bool
uValidateVulkanDevicePropertiesReqruirement(const VkPhysicalDevice physical_device)
{
    if (physical_device)
    {
    }

    // Aquire physical device properties
    /* VkPhysicalDeviceProperties device_properties; */
    /* vkGetPhysicalDeviceProperties(physical_device, */
    /*                               &device_properties); */

    return true;
}

bool
uValidateVulkanDeviceExtensionsReqruirement(const VkPhysicalDevice physical_device,
                                            const char** restrict  user_device_extension_names,
                                            const u16              num_user_device_extension_names)
{
    if (user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, "
                     "the names ptr must be non null\n");
    }

    u16 num_validated_extension_names = 0;
    uQueryVulkanDeviceExtensions(&physical_device,
                                 user_device_extension_names,
                                 num_user_device_extension_names,
                                 &num_validated_extension_names);

    const char extension_name_err_msg[] = "[ vulkan ] Unable to verify user extension names.\n";
    uAssertMsg_v(num_validated_extension_names == num_user_device_extension_names, extension_name_err_msg);
    if (num_validated_extension_names != num_user_device_extension_names)
    {
        uVkVerbose(extension_name_err_msg);
        return false;
    }

    return true;
}

bool
uValidateVulkanDeviceQueueRequirement(const VkPhysicalDevice                   physical_device,
                                      const uVulkanSurfaceInfo* const restrict surface_info,
                                      uVulkanQueueInfo* const restrict         queue_info)
{
    uAssertMsg_v(surface_info, "[ vulkan ] The uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info->surface, "[ vulkan ] The VKSurfaceKHR must be non zero.\n");
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
    VkQueueFamilyProperties* queue_family_props =
      ( VkQueueFamilyProperties* )calloc(queue_family_count, sizeof(VkQueueFamilyProperties));
    if (!queue_family_props)
    {
        return false;
    }

    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_family_props);

    // Require graphics family
    uQueueFamilyPair graphics_pair = {};
    num_queues++;

    // Require present family
    uQueueFamilyPair present_pair = {};
    num_queues++;

    bool queues_satisfied = false;
    for (u32 queue_idx = 0; queue_idx < queue_family_count; queue_idx++)
    {
        // Check grahpics capability for this family
        if (queue_family_props[queue_idx].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphics_pair.index     = queue_idx;
            graphics_pair.validated = true;
        }

        // Check present capability for this family
        VkBool32 present_capability = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, queue_idx, surface_info->surface, &present_capability);

        if (present_capability)
        {
            present_pair.index     = queue_idx;
            present_pair.validated = true;
        }

        // Break when all conditions are met
        if (graphics_pair.validated && present_pair.validated)
        {
            // Store family indices for each required queue.
            *( u32* )&queue_info->designated_graphics_index = graphics_pair.index;
            *( u32* )&queue_info->designated_present_index  = present_pair.index;

            // Issue perf warning for different graphics/presentation queue
            if (graphics_pair.index != present_pair.index)
            {
                uWarning("[ vulkan ] [ perf ] Different graphics and prsent queue "
                         "families chosen.\n");
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
        uFatal("[ engine ][ vulkan ] uVulkanInfo.queues length: %d. %d queues were "
               "checked during physical device "
               "creation.\n",
               uVULKAN_NUM_QUEUES,
               num_queues);
    }

    return queues_satisfied;
}

__UE_inline__ void
uSelectVulkanSwapChainExtent(const uGameWindow* restrict const game_window)
{
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();

    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");

    VkSurfaceCapabilitiesKHR surface_capabilities = surface_info->surface_capabilities;
    u32*                     extent_width         = ( u32* )(( VkExtent2D* )&(surface_info->surface_extent).width);
    u32*                     extent_height        = ( u32* )(( VkExtent2D* )&(surface_info->surface_extent).height);

    // Note: Some window managers use u32_MAX as a magic value which indicates
    // that the extent must be manually set up.
    u32 u32_MAX = ~( u32 )0;
    if (surface_capabilities.currentExtent.width != u32_MAX)
    {
        *extent_width  = surface_capabilities.currentExtent.width;
        *extent_height = surface_capabilities.currentExtent.height;
    }
    else
    {
        *extent_width  = game_window->width;
        *extent_height = game_window->height;
    }

    // Ensure we do not exceed maximums
    if (*extent_width > surface_capabilities.maxImageExtent.width)
    {
        uVkVerbose("[ vulkan ] Calculated extent width exceeded surface "
                   "capabiility; capped.\n");
        *extent_width = surface_capabilities.maxImageExtent.width;
    }

    if (*extent_height > surface_capabilities.maxImageExtent.height)
    {
        uVkVerbose("[ vulkan ] Calculated extent height exceeded surface "
                   "capabiility; capped.\n");
        *extent_height = surface_capabilities.maxImageExtent.height;
    }

    // Ensure we do not fall below minimums
    if (*extent_width < surface_capabilities.minImageExtent.width)
    {
        uVkVerbose("[ vulkan ] Calculated extent width fell below surface "
                   "capabiility; capped.\n");
        *extent_width = surface_capabilities.minImageExtent.width;
    }

    if (*extent_height < surface_capabilities.minImageExtent.height)
    {
        uVkVerbose("[ vulkan ] Calculated extent height fell below surface "
                   "capabiility; capped.\n");
        *extent_height = surface_capabilities.minImageExtent.height;
    }
}

__UE_inline__ bool
uSelectVulkanSwapChain(const uGameWindow* const restrict game_window)
{
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();

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
    u32* non_const_designated_format_index = ( u32* )&(surface_info->designated_format_index);
    bool optimal_format_found              = false;
    for (u16 format_idx = 0; format_idx < surface_info->num_surface_formats; format_idx++)
    {
        if (surface_info->surface_formats[format_idx].format == VK_FORMAT_B8G8R8A8_SRGB &&
            surface_info->surface_formats[format_idx].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            *non_const_designated_format_index = format_idx;
            optimal_format_found               = true;
            break;
        }
    }

    if (!optimal_format_found)
    {
        // Issue perf warning
        uWarning("[ vulkan ] [ perf ] 8bpp sRGB swap chain format unavailble. "
                 "Defaulting to first format index.\n");
    }

    // Determine best available presentation mode
    u32* non_const_designated_present_index = ( u32* )&(surface_info->designated_present_index);
    bool suitable_present_found             = false;
    bool optimal_present_found              = false;
    u16  fifo_present_index                 = 0;
    for (u16 present_idx = 0; present_idx < surface_info->num_present_modes; present_idx++)
    {
        VkPresentModeKHR present_mode = surface_info->present_modes[present_idx];
        if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            *non_const_designated_present_index = present_idx;
            optimal_present_found               = true;
            suitable_present_found              = true;
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
        uWarning("[ vulkan ] [ perf ] Tripple buffering present mode unavailble. "
                 "Defaulting to FIFO.\n");
        *non_const_designated_present_index = fifo_present_index;
    }

    // Determine best avialable swap extent
    uSelectVulkanSwapChainExtent(game_window);

    // Note: require FIFO present mode; accept any format
    return suitable_present_found;
}

__UE_inline__ void
uValidateVulkanSwapChainAndSurfaceCompatibility(const VkPhysicalDevice             potential_physical_device,
                                                const VkSurface                    potential_surface,
                                                u32* const restrict                return_num_surface_formats,
                                                VkSurfaceFormatKHR* const restrict return_surface_formats,
                                                u32* const restrict                return_num_present_modes,
                                                VkPresentModeKHR* const restrict   return_present_modes)
{
    uAssertMsg_v(!return_num_surface_formats, "[ vulkan ] Num surface formats ptr must be null; may be overwritten.\n");
    uAssertMsg_v(!return_surface_formats, "[ vulkan ] Surface formats ptr must be null; may be overwritten.\n");
    uAssertMsg_v(!return_num_present_mode, "[ vulkan ] Num present modes ptr must be null; may be overwritten.\n");
    uAssertMsg_v(!return_present_modes, "[ vulkan ] Present modes ptr must be null; may be overwritten.\n");

    // Get surface capabilities
    VkSurfaceCapabilitiesKHR* potential_surface_capabilities = NULL;
    VkResult                  success = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(potential_physical_device,
                                                                 potential_surface,
                                                                 potential_surface_capabilities);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Could not attain surface capabilities.\n");
    }

    // Get surface formats
    u32* potential_num_surface_formats = NULL
      : success                        = vkGetPhysicalDeviceSurfaceFormatsKHR(potential_physical_device,
                                                       potential_surface,
                                                       potential_num_surface_formats,
                                                       NULL);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Could not attain surface format quantity.\n");
    }

    VkSurfaceFormatKHR* potential_surface_formats = NULL;
    if (!(*potential_num_surface_formats))
    {
        return false;
    }

    // [ cfarvin::REVISIT ] Remove this?
    /* if (is_rebuilding_swap_chain && surface_info->surface_formats) */
    /* { */
    /*     free(( VkSurfaceFormatKHR* )surface_info->surface_formats); */
    /* } */

    // [ cfarvin::TODO ] [ MEMFREE ]
    potential_surface_formats =
      ( VkSurfaceFormatKHR* )malloc(potential_num_surface_formats * sizeof(VkSurfaceFormatKHR));

    if (!potential_surface_formats)
    {
        uFatal("[ vulkan ] Unable to allocate memory for surface formats.\n");
    }

    success = vkGetPhysicalDeviceSurfaceFormatsKHR(potential_physical_device,
                                                   potential_surface,
                                                   potential_num_surface_formats,
                                                   potential_surface_formats);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Could not attain surface formats.\n");
    }

    // Get surface present modes
    u32* potential_num_present_modes = NULL;
    success                          = vkGetPhysicalDeviceSurfacePresentModesKHR(potential_physical_device,
                                                        potential_surface,
                                                        potential_num_present_modes,
                                                        NULL);
    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Could not attain present mode quantity.\n");
    }

    if (!(*num_present_modes))
    {
        return false;
    }

    // [ cfarvin::REVISIT ] Remove this?
    /* if (is_rebuilding_swap_chain && surface_info->present_modes) */
    /* { */
    /*     free(( VkPresentModeKHR* )surface_info->present_modes); */
    /* } */

    // [ cfarvin::TODO ] [ MEMFREE ]
    VkPresentModeKHR* potential_present_modes =
      ( VkPresentModeKHR* )malloc(potential_num_present_modes * sizeof(VkPresentModeKHR));

    if (!potential_present_modes)
    {
        uFatal("[ vulkan ] Unable to allocate memory for present modes.\n");
    }

    success = vkGetPhysicalDeviceSurfacePresentModesKHR(potential_physical_device,
                                                        potential_surface,
                                                        potential_num_present_modes);
    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Could not attain present modes.\n");
    }

    // Consider successful if there are present modes that are compatible with
    // surface formats
    if (potential_num_surface_formats && potential_num_present_modes)
    {
        *return_num_surface_formats = *potential_num_surface_formats;
        *return_num_present_modes   = *potential_num_present_modes;

        // [ cfarvin::TODO ] [ MEMFREE ] Make sure to free these in calling scope.
        // Note: must be freed in calling scope.
        size_t surface_format_bytes = potential_num_surface_formats * sizeof(VkSurfaceFormatKHR);
        size_t present_mode_bytes   = potential_num_present_modes * sizeof(VkPresentModeKHR) return_present_modes =
                                      ( VkSurfaceFormatKHR* )malloc(surface_format_bytes);
        return_present_modes = ( VkPresentModeKHR* )malloc(present_modes_byts);

        memcpy(return_surface_formats, potential_surface_formats, surface_format_bytes);
        memcpy(return_present_modes, potential_present_modes, present_mode_bytes);
    }
}

void
uSelectVulkanPhysicalDevice(const VkPhysicalDevice** const restrict physical_device_list,
                            VkPhysicalDevice* const restrict        return_device,
                            uVulkanQueueInfo* const restrict        queue_info,
                            const u32                               num_physical_devices,
                            const char** const restrict             user_device_extension_names,
                            const u16                               num_user_device_extension_names)
{
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();

    uAssertMsg_v(physical_device_list, "[ vulkan ] VkPhysicalDevice ptr ptr must be non null.\n");
    uAssertMsg_v(!(*return_device), "[ vulkan ] VkPhysicalDevice ptr must be null; will be overwritten\n");
    uAssertMsg_v(queue_info, "[ vulkan ] uVulkanQueueInfo ptr must be non null\n");
    uAssertMsg_v(num_physical_devices, "[ vulkan ] Physical devices count must be non zero.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info->surface, "[ vulkan ] VkSurfaceKHR must be non zero.\n");
    for (u32 device_idx = 0; device_idx < num_physical_devices; device_idx++)
    {
        uAssertMsg_v(physical_device_list[device_idx],
                     "[ vulkan ] Indices of physical_device_list must be non-null.\n");
    }
    if (user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, "
                     "the names ptr must be non null\n");
    }

    bool selection_complete = false;
    for (u32 device_idx = 0; device_idx < num_physical_devices; device_idx++)
    {
        const VkPhysicalDevice potential_physical_device = *physical_device_list[device_idx];
        if (!physical_device)
        {
            continue;
        }

        selection_complete = uValidateVulkanDeviceFeaturesReqruirement(physical_device) &&
                             uValidateVulkanDevicePropertiesReqruirement(physical_device) &&
                             uValidateVulkanDeviceQueueRequirement(physical_device, surface_info, queue_info) &&
                             uValidateVulkanDeviceExtensionsReqruirement(physical_device,
                                                                         user_device_extension_names,
                                                                         num_user_device_extension_names) &&
                             // Must query for surface and extension support before swap chain
                             // support.
                             uValidateVulkanSwapChainAndSurfaceCompatibility(potential_physical_device);

        // Set the physical device
        if (selection_complete)
        {
            *return_device = physical_device;

            // If there's more than one physical device detected, report which one was
            // chosen
            if (num_physical_devices > 1)
            {
                VkPhysicalDeviceProperties physical_device_properties = {};
                vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);
                printf("[ vulkan ] Chosen device: %s\n", physical_device_properties.deviceName);
            }

            break;
        }
    }

    if (!selection_complete)
    {
        uFatal("[ vulkan ] Unable to select a suitable physical device.\n");
    }
}

void
uCreateVulkanPhysicalDevice(uVulkanInfo* const restrict        v_info,
                            uVulkanQueueInfo* const restrict   queue_info,
                            const char** const restrict        user_device_extension_names,
                            const u16                          num_user_device_extension_names,
                            uVulkanSurfaceInfo* const restrict return_surface_info)
{
    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(!v_info->physical_device, "[ vulkan ] VkPhysicalDevice must be zero; will be overwritten.\n");
    uAssertMsg_v(!v_info->logical_device, "[ vulkan ] VkDevice must be zero; will be overwritten.\n");
    uAssertMsg_v(queue_info, "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(return_surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");
    if (num_user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, "
                     "the names ptr must be non null\n");
    }

    u32 num_physical_devices = 0;
    vkEnumeratePhysicalDevices(v_info->instance, &num_physical_devices, NULL);

    uAssertMsg_v(num_physical_devices, "[ vulkan ] No physical devices found.\n");
    if (!num_physical_devices)
    {
        return;
    }

    VkPhysicalDevice* physical_device_list =
      ( VkPhysicalDevice* )calloc(num_physical_devices, sizeof(VkPhysicalDevice));
    uAssertMsg_v(physical_device_list, "[ vulkan ] Unable to allocate physical device list.\n");

    vkEnumeratePhysicalDevices(v_info->instance, &num_physical_devices, physical_device_list);
    uVkVerbose("Found %d physical devices.\n", num_physical_devices);

    VkPhysicalDevice candidate_device = NULL;
    uSelectVulkanPhysicalDevice(( const VkPhysicalDevice** )&physical_device_list,
                                &candidate_device,
                                queue_info,
                                num_physical_devices,
                                user_device_extension_names,
                                num_user_device_extension_names,
                                return_surface_info);

    if (!candidate_device)
    {
        uFatal("[ vulkan ] Unable to find suitable device.\n");
    }

    VkPhysicalDevice* non_const_physical_device = ( VkPhysicalDevice* )&(v_info->physical_device);
    *non_const_physical_device                  = candidate_device;

    if (physical_device_list)
    {
        free(physical_device_list);
    }
}

#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
// Note: no function/argument decorations to conform w/ Vulkan spec.
VKAPI_ATTR VkBool32 VKAPI_CALL
uVkDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity_bits,
                 VkDebugUtilsMessageTypeFlagsEXT             message_type_bits,
                 const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                 void*                                       user_data)
{
    VkBool32 should_abort_calling_process = VK_FALSE;
    if (user_data)
    {
    } // Silence warnings

    if (message_severity_bits >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT ||
        message_type_bits >= VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
    {
        printf("[ vulkan validation begin ]\n%s\n[ vulkan validation end ]\n", callback_data->pMessage);
        fflush(stdout);

        should_abort_calling_process = VK_TRUE;
    }

    return should_abort_calling_process;
}

void
uCreateVulkanDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT* const restrict debug_message_create_info)
{
    uAssertMsg_v(debug_message_create_info, "[ vulkan ] VkDebugUtilsMessengerCreateInfoEXT ptr must be non null.\n");

    debug_message_create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    debug_message_create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    debug_message_create_info->sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_message_create_info->pfnUserCallback = uVkDebugCallback;
    debug_message_create_info->pUserData       = NULL;
}

// [ cfarvin::TODO ] Remove params and use singletons
void
uCreateVulkanDebugMessenger(const uVulkanInfo* const restrict                  v_info,
                            VkDebugUtilsMessengerCreateInfoEXT* const restrict debug_message_create_info,
                            VkDebugUtilsMessengerEXT* const restrict           debug_messenger)
{
    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] VkInstance ptr must be non zero.\n");
    uAssertMsg_v(debug_message_create_info, "[ vulkan ] VkDebugUtilsMessengerCreateInfoEXT ptr must be non null.\n");

    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT =
      ( PFN_vkCreateDebugUtilsMessengerEXT )vkGetInstanceProcAddr(v_info->instance, "vkCreateDebugUtilsMessengerEXT");

    uAssertMsg_v(vkCreateDebugUtilsMessengerEXT, "[ vulkan ] Failed to acquire pfn: vkCreateDebugUtilsMessengerEXT\n");

    if (vkCreateDebugUtilsMessengerEXT)
    {
        uCreateVulkanDebugMessengerInfo(( VkDebugUtilsMessengerCreateInfoEXT* )debug_message_create_info);
        VkResult success =
          vkCreateDebugUtilsMessengerEXT(v_info->instance, debug_message_create_info, NULL, debug_messenger);

        if (success != VK_SUCCESS)
        {
            uFatal("[ vulkan ] Failed to create debug messenger callback.\n");
        }
    }
}
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

#if _WIN32
// [ cfarvin::TODO ] Better than basic validation for game window param may be
// needed.
void
uCreateWin32Surface(const uWin32Info* const restrict win32_info)
{
    uVulkanInfo*        v_info       = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanSurfaceInfo* surface_info = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(v_info->instance, "[ vulkan ] VkInstance must be non zero.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null");
    uAssertMsg_v(!surface_info->surface, "[ vulkan ] uVulkanSurfaceInfo ptr must be null; will be overwritten.\n");
    uAssertMsg_v(win32_info, "[ vulkan ] [ win32 ] uWin32Info ptr must be non null.\n");
    uAssertMsg_v(win32_info->window, "[ vulkan ] [ win32 ] uWin32Info hwnd ptr member must be non null.\n");

    VkWin32SurfaceCreateInfoKHR win32_surface_info = {};
    win32_surface_info.sType                       = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    win32_surface_info.hwnd                        = win32_info->window;
    win32_surface_info.hinstance                   = GetModuleHandle(NULL);

    VkResult win32_surface_result =
      vkCreateWin32SurfaceKHR(v_info->instance, &win32_surface_info, NULL, ( VkSurfaceKHR* )&surface_info->surface);

    if (win32_surface_result != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to create win32 surface.\n");
    }
}
#endif // _WIN32

void
uQueryVulkanDeviceExtensions(const VkPhysicalDevice* const restrict physical_device,
                             const char** const restrict            user_device_extension_names,
                             const u16                              num_user_device_extension_names,
                             u16* const restrict                    num_verified_extension_names)
{
    uAssertMsg_v(physical_device, "[ vulkan ] VkPhysicalDevice ptr must be non null.\n");
    uAssertMsg_v(num_verified_extension_names, "[ vulkan ] Verified extension count ptr must be non null.\n");
    if (num_user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, "
                     "the names ptr must be non null.\n");
    }

    if (!num_user_device_extension_names)
    {
        return;
    }

    // Query Extension Count
    u32      num_available_device_extensions = 0;
    VkResult success =
      vkEnumerateDeviceExtensionProperties(*physical_device, NULL, &num_available_device_extensions, NULL);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to enumerate extension properties.\n");
    }

    VkExtensionProperties* device_extension_properties =
      ( VkExtensionProperties* )malloc(num_available_device_extensions * sizeof(VkExtensionProperties));

    // Query Extension Names
    success = vkEnumerateDeviceExtensionProperties(*physical_device,
                                                   NULL,
                                                   &num_available_device_extensions,
                                                   device_extension_properties);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to enumerate extension properties.\n");
    }

    // Verify user/device extensions match
    *num_verified_extension_names = 0;
    for (u32 device_extension_name_idx = 0; device_extension_name_idx < num_available_device_extensions;
         device_extension_name_idx++)
    {
        for (u32 user_device_extension_name_idx = 0; user_device_extension_name_idx < num_user_device_extension_names;
             user_device_extension_name_idx++)
        {
            if (strcmp((( const char* )(device_extension_properties[device_extension_name_idx]).extensionName),
                       ( const char* )user_device_extension_names[user_device_extension_name_idx]) == 0)
            {
                (*num_verified_extension_names)++;
            }
        }
    }

    if (device_extension_properties)
    {
        free(device_extension_properties);
    }
}

#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
void
uQueryVulkanInstanceValidationLayers(char*** restrict                     instance_validation_layer_names,
                                     VkLayerProperties** const restrict   instance_validation_layer_properties,
                                     VkInstanceCreateInfo* const restrict instance_create_info,
                                     const char** const restrict          user_instance_validation_layer_names,
                                     const u32                            num_user_instance_validation_layer_names)
{
    uAssertMsg_v(instance_create_info, "[ vulkan ] VkInstanceCreateInfo ptr must be non null.\n");
    uAssertMsg_v(!(*instance_validation_layer_names),
                 "[ vulkan ] Layer names ptr must be null; will be overwritten.\n");
    uAssertMsg_v(!(*instance_validation_layer_properties),
                 "[ vulkan ] VkLayerProperties ptr must be null; will be overwritten.\n");
    uAssertMsg_v(user_instance_validation_layer_names, "[ vulkan ] Validation layer names ptr must be non null.\n");
    if (num_user_instance_validation_layer_names)
    {
        uAssertMsg_v(user_instance_validation_layer_names && *user_instance_validation_layer_names,
                     "[ vulkan ] If the instance layer quanitity is non zero, the "
                     "names ptr must be non null.\n");
    }

    if (!num_user_instance_validation_layer_names)
    {
        return;
    }

    // Query Layer Count
    VkResult success              = VK_SUCCESS;
    u32      num_available_layers = 0;
    success                       = vkEnumerateInstanceLayerProperties(&num_available_layers, NULL);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to enumerate layers.\n");
    }

    uAssertMsg_v((num_available_layers >= num_user_instance_validation_layer_names),
                 "[ vulkan ] Number of requested validation layers [ %d ] exceeds total "
                 "avaialbe count [ %d ].\n",
                 num_user_instance_validation_layer_names,
                 num_available_layers);

    *instance_validation_layer_properties =
      ( VkLayerProperties* )malloc(num_available_layers * sizeof(VkLayerProperties));

    // Query Layer Names
    success = vkEnumerateInstanceLayerProperties(&num_available_layers, *instance_validation_layer_properties);
    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to enumerate layers.\n");
    }

    // Set Layer Names
    uVkVerbose("Searching for validation layers...\n");
    u32 num_added_layers             = 0;
    *instance_validation_layer_names = ( char** )malloc(num_available_layers * sizeof(s8**));
    for (u32 available_layer_idx = 0; available_layer_idx < num_available_layers; available_layer_idx++)
    {
        for (u32 user_layer_idx = 0; user_layer_idx < num_user_instance_validation_layer_names; user_layer_idx++)
        {
            uVkVerbose("\tLayer found: %s\n", (*instance_validation_layer_properties)[available_layer_idx].layerName);
            if (strcmp(( const char* )user_instance_validation_layer_names[user_layer_idx],
                       ( const char* )(*instance_validation_layer_properties)[available_layer_idx].layerName) == 0)
            {
                (*instance_validation_layer_names)[num_added_layers] =
                  ( char* )(*instance_validation_layer_properties)[available_layer_idx].layerName;
                num_added_layers++;
            }
        }
    }

    if (num_added_layers != num_user_instance_validation_layer_names)
    {
        uFatal("[ vulkan ] Unable to load all requested layers.\n");
    }

    instance_create_info->enabledLayerCount   = num_added_layers;
    instance_create_info->ppEnabledLayerNames = ( const char** )*instance_validation_layer_names;
}
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

void
uQueryVulkanInstanceExtensions(const char*** restrict                 instance_extension_names,
                               VkExtensionProperties** const restrict instance_extension_properties,
                               VkInstanceCreateInfo* const restrict   instance_create_info,
                               const char** const restrict            user_instance_extension_names,
                               const u16                              num_user_instance_extension_names)
{
    uAssertMsg_v(instance_create_info, "[ vulkan ] VkInstanceCreateInfo ptr must be non null.\n");
    uAssertMsg_v(!(*instance_extension_names), "[ vulkan ] Extension names ptr must be null; will be overwritten.\n");
    uAssertMsg_v(!(*instance_extension_properties),
                 "[ vulkan ] VkExtensionProperties ptr must be null; will be "
                 "overwritten.\n");
    if (num_user_instance_extension_names)
    {
        uAssertMsg_v(user_instance_extension_names && *user_instance_extension_names,
                     "[ vulkan ] If the instance extension quanitity is non zero, "
                     "the names ptr must be non null\n");
    }

    if (!user_instance_extension_names)
    {
        return;
    }

    // Query Extension Count
    VkResult success = VK_SUCCESS;
    success          = vkEnumerateInstanceExtensionProperties(NULL, &instance_create_info->enabledExtensionCount, NULL);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to enumerate layer properties.\n");
    }

    *instance_extension_properties =
      ( VkExtensionProperties* )malloc(instance_create_info->enabledExtensionCount * sizeof(VkExtensionProperties));

    // Query Extension Names
    success = vkEnumerateInstanceExtensionProperties(NULL,
                                                     &instance_create_info->enabledExtensionCount,
                                                     *instance_extension_properties);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to enumerate layer properties.\n");
    }

    // Set Extension Names
    uVkVerbose("Searching for extensions...\n");
    u32 num_added_extensions  = 0;
    *instance_extension_names = ( const char** )malloc(instance_create_info->enabledExtensionCount * sizeof(s8**));
    for (u32 ext_idx = 0; ext_idx < instance_create_info->enabledExtensionCount; ext_idx++)
    {
        uVkVerbose("\tExtension found: %s\n", (*instance_extension_properties)[ext_idx].extensionName);
        for (u32 user_ext_idx = 0; user_ext_idx < num_user_instance_extension_names; user_ext_idx++)
        {
            if (strcmp(( const char* )user_instance_extension_names[user_ext_idx],
                       ( const char* )(*instance_extension_properties)[ext_idx].extensionName) == 0)
            {
                (*instance_extension_names)[num_added_extensions] =
                  ( const char* )(*instance_extension_properties)[ext_idx].extensionName;
                num_added_extensions++;
            }
        }
    }

    if (num_added_extensions != num_user_instance_extension_names)
    {
        uFatal("[ vulkan ] Unable to load all requested extensions.\n");
    }

    instance_create_info->enabledExtensionCount   = num_added_extensions;
    instance_create_info->ppEnabledExtensionNames = ( const char** )*instance_extension_names;
}

void
uCreateVulkanInstance(const uVulkanInfo* const restrict       v_info,
                      const VkApplicationInfo* const restrict application_info,
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                      const char** const restrict user_instance_validation_layer_names,
                      const u16                   num_user_instance_validation_layer_names,
#endif //__UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                      const char** const restrict user_instance_extension_names,
                      const u16                   num_user_instance_extension_names)

{
    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(application_info, "[ vulkan ] VkApplicationInfo ptr must be non null.\n");
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    if (num_user_instance_validation_layer_names)
    {
        uAssertMsg_v(user_instance_validation_layer_names && *user_instance_validation_layer_names,
                     "[ vulkan ] If the instance layer quanitity is non zero, the "
                     "names ptr must be non null.\n");
    }
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    if (num_user_instance_extension_names)
    {
        uAssertMsg_v(user_instance_extension_names && *user_instance_extension_names,
                     "[ vulkan ] If the instance extension quanitity is non zero, "
                     "the names ptr must be non null.\n");
    }

    VkExtensionProperties* instance_extension_properties        = NULL;
    VkLayerProperties*     instance_validation_layer_properties = NULL;
    char**                 instance_extension_names             = NULL;
    char**                 instance_validation_layer_names      = NULL;

#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    uCreateVulkanDebugMessengerInfo(&vulkan_setup_debug_messenger_info);
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

    VkResult             success              = VK_SUCCESS;
    VkInstanceCreateInfo instance_create_info = {};

    // Query extensions; store in VkInstanceCreateInfo
    uQueryVulkanInstanceExtensions(( const char*** )&instance_extension_names,
                                   &instance_extension_properties,
                                   &instance_create_info,
                                   user_instance_extension_names,
                                   num_user_instance_extension_names);

#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    // Query layers; store in VkInstanceCreateInfo
    uQueryVulkanInstanceValidationLayers(&instance_validation_layer_names,
                                         &instance_validation_layer_properties,
                                         &instance_create_info,
                                         user_instance_validation_layer_names,
                                         num_user_instance_validation_layer_names);
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

    instance_create_info.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = application_info;
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    instance_create_info.pNext = &vulkan_setup_debug_messenger_info;
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    success = vkCreateInstance(&instance_create_info, NULL, ( VkInstance* )&v_info->instance);

    if (success != VK_SUCCESS)
    {
        uFatal("[ vulkan ] Unable to create vulkan instance.\n");
    }

#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    uCreateVulkanDebugMessenger(v_info,
                                ( VkDebugUtilsMessengerCreateInfoEXT* )&vulkan_main_debug_messenger_info,
                                ( VkDebugUtilsMessengerEXT* )&vulkan_main_debug_messenger);
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

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

void
uCreateVulkanApplicationInfo(const char* const restrict        application_name,
                             VkApplicationInfo* const restrict application_info)
{
    uAssertMsg_v(application_name, "[ vulkan ] Application names ptr must be non null.\n");
    uAssertMsg_v(application_info, "[ vulkan ] Application info ptr must be non null.\n");

    application_info->sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    application_info->pApplicationName   = ( const char* )application_name;
    application_info->applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    application_info->pEngineName        = ( const char* )uGetEngineName();
    application_info->apiVersion         = VK_API_VERSION_1_0;
}

void
uInitializeVulkan(const char* const restrict application_name,
                  void* const restrict       os_info,
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                  const char** const restrict user_instance_validation_layer_names,
                  const u8                    num_user_instance_validation_layer_names,
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                  const char** const restrict user_instance_extension_names,
                  const u8                    num_user_instance_extension_names,
                  const char** const restrict user_device_extension_names,
                  const u8                    num_user_device_extension_names)
{
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    if (num_user_instance_validation_layer_names)
    {
        uAssertMsg_v(user_instance_validation_layer_names && *user_instance_validation_layer_names,
                     "[ vulkan ] If the instance layer quanitity is non zero, the "
                     "names ptr must be non null.\n");
    }
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    if (num_user_instance_extension_names)
    {
        uAssertMsg_v(user_instance_extension_names && *user_instance_extension_names,
                     "[ vulkan ] If the instance extension quanitity is non zero, "
                     "the names ptr must be non null.\n");
    }
    if (num_user_device_extension_names)
    {
        uAssertMsg_v(user_device_extension_names && *user_device_extension_names,
                     "[ vulkan ] If the device extension quanitity is non zero, "
                     "the names ptr must be non null.\n");
    }

    VkApplicationInfo   application_info = {};
    uVulkanInfo*        v_info           = ( uVulkanInfo* )uGetVulkanInfo();
    uVulkanQueueInfo*   queue_info       = ( uVulkanQueueInfo* )uGetVulkanQueueInfo();
    uVulkanSurfaceInfo* surface_info     = ( uVulkanSurfaceInfo* )uGetVulkanSurfaceInfo();

    uAssertMsg_v(v_info, "[ vulkan ] uVulkanInfo ptr must be non null.\n");
    uAssertMsg_v(queue_info, "[ vulkan ] uVulkanQueueInfo ptr must be non null.\n");
    uAssertMsg_v(surface_info, "[ vulkan ] uVulkanSurfaceInfo ptr must be non null.\n");

#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
    printf("[ vulkan ] Validation layers: ON.\n");
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1

    uCreateVulkanApplicationInfo(application_name, &application_info);
    uCreateVulkanInstance(v_info,
                          &application_info,
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                          user_instance_validation_layer_names,
                          num_user_instance_validation_layer_names,
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                          user_instance_extension_names,
                          num_user_instance_extension_names);

#if _WIN32
    uCreateWin32Surface(( const uWin32Info* const )os_info);
#else
    // [ cfarvin::TODO ] Implement linux surface cration
    uFatal("Linux surface creation not implemented.");
#endif // _WIN32

    // queue_info built
    // surface_info_built
    uCreateVulkanPhysicalDevice(v_info,
                                queue_info,
                                user_device_extension_names,
                                num_user_device_extension_names,
                                surface_info);

    uCreateVulkanLogicalDevice(v_info,
                               queue_info,
#if __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                               user_instance_validation_layer_names,
                               num_user_instance_validation_layer_names,
#endif // __UE_debug__ == 1 || __UE_vkForceValidation__ == 1
                               user_device_extension_names,
                               num_user_device_extension_names);
}

void
uDestroyVulkan()
{
    uVulkanInfo* v_info = ( uVulkanInfo* )uGetVulkanInfo();
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

// [ cfarvin::STUDY ]  VK_DEPENDENCY_BY_REGION_BIT,
// VkPipelineVertexInputStateCreateInfo,
// input_assembly_create_info.primitiveRestartEnable,
// multi_sample_create_info.sampleShadingEnable,
// VkPipelineColorBlendAttachmentState, color_blend_create_info.logicOpEnable
// NOTES:
// - Skip window manager (or build a cusom one) with VK_KHR_display &&
// VK_KHR_display_swapchain; this will render fullscreen.
