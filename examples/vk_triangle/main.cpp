#include "vk_triangle.h"

#if __linux__
#include <understone/nix_platform.h>
#elif _WIN32
#include <understone/win_platform.h>
#endif // _WIN32

#include "shaders/baked_shaders/vkTriangleShader.h"

#include <understone/render_tools.h>
#include <understone/shader_tools.h>
#include <understone/tests/tests.h> // __uTESTS_ENABLED__ = 0 to disable tests on startup
#include <understone/vulkan_tools.h>
#include <understone/vulkan_types.h>

static constexpr size_t                kVkTriangleNumShaderStages                               = 2;
static VkPipelineShaderStageCreateInfo vk_triangle_shader_stage_cis[kVkTriangleNumShaderStages] = {};
static uVulkanShader      vk_triangle_shaders[kVkTriangleNumShaderStages] = { vkTriangle_vert, vkTriangle_frag };
static uVulkanRenderTools vk_triangle_render_tools                        = {};
static bool               vk_triangle_running                             = true;
static uGameWindow        vk_triangle_window                              = {};

LRESULT CALLBACK
uEngineWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
        {
            kWin32SystemEvent = uEventClose;
            break;
        }

        case WM_DESTROY:
        {
            kWin32SystemEvent = uEventClose;
            PostQuitMessage(0);
            break;
        }

        case WM_LBUTTONDOWN:
        {
            uSetInputPressed(uMouse_left);
            break;
        }

        case WM_RBUTTONDOWN:
        {
            uSetInputPressed(uMouse_right);
            break;
        }

        case WM_LBUTTONUP:
        {
            uSetInputReleased(uMouse_left);
            break;
        }

        case WM_RBUTTONUP:
        {
            uSetInputReleased(uMouse_right);
            break;
        }

        case WM_SIZE:
        {
            // [ cfarvin::TODO ] scaling/ortho
            vk_triangle_window.width  = ( u16 )LOWORD(lParam);
            vk_triangle_window.height = ( u16 )HIWORD(lParam);
            kWin32SystemEvent         = uEventResize;

            if (wParam == SIZE_MINIMIZED)
            {
                vk_triangle_window.is_minimized = true;
            }
            else if (wParam == SIZE_RESTORED)
            {
                vk_triangle_window.is_minimized = false;
            }

            break;
        }

        case WM_MOUSEMOVE:
        {
            GetCursorPos(&kWin32MouseCoordinates);
            ScreenToClient(hwnd, &kWin32MouseCoordinates);

            // Note: uMousePos has origin @ lower left == (0, 0, 0)
            mouse_pos.x = ( u16 )kWin32MouseCoordinates.x;
            mouse_pos.y = (u16)(vk_triangle_window.height - kWin32MouseCoordinates.y);
            break;
        }
    }

    // Default Window Procedure
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void
VkTriangleDestroy()
{
    uDestroyRenderToolSyncMembers(( uVulkanRenderTools* )&vk_triangle_render_tools);
    uDestroyShaderModules(&vk_triangle_shaders[0], kVkTriangleNumShaderStages);
    uAssertMsg_v(!vk_triangle_running, "[ engine ] Tear down called while ` kRunning == true`.\n");

    printf("[ VkTriangleExample ] Graceful exit.\n");
}

void
VkTriangleInit()
{
    const char* const kApplicationName                    = "Vk Triangle Example";
    const char*       kRequiredInstanceValidationLayers[] = { "VK_LAYER_KHRONOS_validation" };
    const char*       kRequiredDeviceExtensions[]         = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    const char* kRequiredInstanceExtensions[] = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
                                                  "VK_KHR_surface",
                                                  "VK_KHR_win32_surface" };
#if _WIN32
    uWin32Info os_info = uCreateWin32Info(&vk_triangle_window, uEngineWindowProc);
#else

#endif // _WIN32

    // [ cfarvin::TODO ] This is a super sloppy way to start off
    uInitializeVulkan(kApplicationName,
                      ( void* )&os_info,
                      kRequiredInstanceValidationLayers,
                      sizeof(kRequiredInstanceValidationLayers) / sizeof(char*),
                      kRequiredInstanceExtensions,
                      sizeof(kRequiredInstanceExtensions) / sizeof(char*),
                      kRequiredDeviceExtensions,
                      sizeof(kRequiredDeviceExtensions) / sizeof(char*));

    // Load shaders
    if (!uCreateVulkanShaderModules(&vk_triangle_shaders[0],
                                    kVkTriangleNumShaderStages,
                                    &vk_triangle_shader_stage_cis[0]))
    {
        uError_v("Unable to create shader modules!\n");
        vk_triangle_running = false;
        VkTriangleDestroy();
    }

    uCreateVulkanSwapChain(&vk_triangle_window);
    uCreateVulkanImageViews();
    uCreateVulkanRenderPass();
    uCreateVulkanGraphicsPipeline(kVkTriangleNumShaderStages, vk_triangle_shader_stage_cis);
    uCreateVulkanFrameBuffers();
    uCreateVulkanCommandPool();
    uCreateVulkanCommandBuffers();
    uCreateRenderTools(&vk_triangle_render_tools, vk_triangle_shader_stage_cis, kVkTriangleNumShaderStages);
}

void
VkTriangleHandleWindowResize()
{
    uDebugPrint("[ resize ] width: %d, height: %d\n", vk_triangle_window.width, vk_triangle_window.height);

    if (!vk_triangle_window.is_minimized)
    {
        uDestroyShaderModules(&vk_triangle_shaders[0], kVkTriangleNumShaderStages);
        uCreateVulkanShaderModules(&vk_triangle_shaders[0],
                                   sizeof(vk_triangle_shaders) / sizeof(uVulkanShader),
                                   &vk_triangle_shader_stage_cis[0]);
        uRebuildVulkanSwapChain(vk_triangle_shader_stage_cis, kVkTriangleNumShaderStages, &vk_triangle_window);
        is_render_tool_outdated = true;
    }
}

void
VkTriangleRefreshInputState()
{
    uSystemEvent sys_event = uEventNone;

#if __linux__
    // [ cfarvin::RESTORE ] Re-implement
    // sys_event = uNixHandleEvents();
#elif _WIN32
    sys_event = uWin32HandleEvents();
#else
    assert(0);
#endif // __linux__ _WIN32

    switch (sys_event)
    {
        case uEventNone:
        {
            break;
        }
        case uEventResize:
        {
            VkTriangleHandleWindowResize();
            break;
        }
        case uEventClose:
        {
            vk_triangle_running = false;
            break;
        }
    }
}

void
VkTriangleGameLoop()
{
    while (vk_triangle_running)
    {
        // [ cfarvin::TODO ] Is this only called on minimize or resize? If so, can
        //                   This be done on the function that handles these rather
        //                   than the draw loop?
        if (is_render_tool_outdated && !vk_triangle_window.is_minimized)
        {
            uRebuidlRenderTools(&vk_triangle_render_tools, vk_triangle_shader_stage_cis, kVkTriangleNumShaderStages);
        }

        if (!vk_triangle_window.is_minimized)
        {
            uRenderFrame(&vk_triangle_window, &vk_triangle_render_tools);
        }

        VkTriangleRefreshInputState();
    }
}

int
main(int argc, char** argv)
{
    if (argc && argv)
    {
    }

    // [ cfarvin::TODO ] Tests should be their own project
    runAllTests();

    // [ cfarvin::TODO ] Move these to engine side; maybe a function
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

    VkTriangleInit();
    VkTriangleGameLoop();
    VkTriangleDestroy();

    return 0;
}
