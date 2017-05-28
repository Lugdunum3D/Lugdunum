#include <lug/Graphics/Vulkan/API/Builder/Surface.hpp>

#include <lug/Graphics/Vulkan/API/Instance.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

Surface::Surface(const API::Instance& instance) : _instance{instance} {}

bool Surface::build(API::Surface& surface, VkResult* returnResult) {
    // Create the surface
    VkSurfaceKHR vkSurface{VK_NULL_HANDLE};

#if defined(LUG_SYSTEM_WINDOWS) // Win32 surface
    const VkWin32SurfaceCreateInfoKHR createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.hinstance */ _hinstance,
        /* createInfo.hwnd */ _hwnd
    };

    VkResult result = vkCreateWin32SurfaceKHR(static_cast<VkInstance>(_instance), &createInfo, nullptr, &vkSurface);
#elif defined(LUG_SYSTEM_LINUX) // Linux surface
    const VkXlibSurfaceCreateInfoKHR createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.dpy */ _dpy,
        /* createInfo.window */ _window
    };

    VkResult result = vkCreateXlibSurfaceKHR(static_cast<VkInstance>(_instance), &createInfo, nullptr, &vkSurface);
#elif defined(LUG_SYSTEM_ANDROID) // Android Surface
    const VkAndroidSurfaceCreateInfoKHR createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.window */ _window
    };

    VkResult result = vkCreateAndroidSurfaceKHR(static_cast<VkInstance>(_instance), &createInfo, nullptr, &vkSurface);
#endif

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    surface = API::Surface(vkSurface, &_instance);

    return true;
}

std::unique_ptr<API::Surface> Surface::build(VkResult* returnResult) {
    std::unique_ptr<API::Surface> surface = std::make_unique<API::Surface>();
    return build(*surface, returnResult) ? std::move(surface) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
