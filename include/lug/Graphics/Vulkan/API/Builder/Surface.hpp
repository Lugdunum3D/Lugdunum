#pragma once

#include <memory>

#include <lug/Graphics/Vulkan/API/Surface.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

class Instance;

class Surface {
public:
    Surface(const API::Instance& instance);

    Surface(const Surface&&) = delete;
    Surface(Surface&&) = delete;

    Surface& operator=(const Surface&&) = delete;
    Surface& operator=(Surface&&) = delete;

    ~Surface() = default;

    // Setters
#if defined(LUG_SYSTEM_WINDOWS) // Win32 surface
    void setWindowInformations(HINSTANCE hinstance, HWND hwnd);
#elif defined(LUG_SYSTEM_LINUX) // Linux surface
    void setWindowInformations(Display* dpy, ::Window window);
#elif defined(LUG_SYSTEM_ANDROID) // Android Surface
    void setWindowInformations(ANativeWindow* window);
#endif

    // Build methods
    bool build(API::Surface& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::Surface> build(VkResult* returnResult = nullptr);

private:
    const API::Instance& _instance;

#if defined(LUG_SYSTEM_WINDOWS) // Win32 surface
    HINSTANCE _hinstance;
    HWND _hwnd;
#elif defined(LUG_SYSTEM_LINUX) // Linux surface
    Display* _dpy;
    ::Window _window;
#elif defined(LUG_SYSTEM_ANDROID) // Android Surface
    ANativeWindow* _window;
#endif
};

#include <lug/Graphics/Vulkan/API/Builder/Surface.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
