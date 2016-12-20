#include <lug/Graphics/Vulkan/RenderWindow.hpp>

#if defined(LUG_SYSTEM_WINDOWS)

    #include <lug/Window/Win32/WindowImplWin32.hpp>

#elif defined(LUG_SYSTEM_LINUX)

    #include <lug/Window/Unix/WindowImplX11.hpp>

#elif defined(LUG_SYSTEM_ANDROID)

    #include <lug/Window/Android/WindowImplAndroid.hpp>

#endif

namespace lug {
namespace Graphics {
namespace Vulkan {

RenderWindow::RenderWindow(Renderer& renderer): _renderer(renderer) {}

std::unique_ptr<RenderWindow> RenderWindow::create(Renderer& renderer, uint16_t width, uint16_t height, const std::string& title, lug::Window::Style style) {
    std::unique_ptr<RenderWindow> win(new RenderWindow(renderer));

    if (!win->createWindow(width, height, title, style)) {
        return nullptr;
    }

    return win;
}

bool RenderWindow::createSurface() {
    // Create window surface

    #if defined(LUG_SYSTEM_WINDOWS) // Win32 surface
        VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
        surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceInfo.hinstance = _impl->getHinstance();
        surfaceInfo.hwnd = _impl->getHandle();
        return vkCreateWin32SurfaceKHR(_renderer.getInstance(), &surfaceInfo, nullptr, &_surface) == VK_SUCCESS; // TODO: Check error
    #elif defined(LUG_SYSTEM_LINUX) // Linux surface
        VkXlibSurfaceCreateInfoKHR surfaceInfo = {};
        surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        surfaceInfo.dpy = _impl->getDisplay();
        surfaceInfo.window = _impl->getWindow();
        return vkCreateXlibSurfaceKHR(_renderer.getInstance(), &surfaceInfo, nullptr, &_surface) == VK_SUCCESS; // TODO: Check error
    #else
        #error "RenderWindow::createSurface Unknow platform"
    #endif
}

bool RenderWindow::init() {
    return createSurface();
}

} // Vulkan
} // Graphics
} // lug
