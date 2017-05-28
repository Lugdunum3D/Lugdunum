#include <lug/Graphics/Vulkan/API/Surface.hpp>

#include <lug/Graphics/Vulkan/API/Instance.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

Surface::Surface(VkSurfaceKHR surface, const Instance* instance) : _surface(surface), _instance(instance) {}

Surface::Surface(Surface&& surface) {
    _surface = surface._surface;
    _instance = surface._instance;
    surface._surface = VK_NULL_HANDLE;
    surface._instance = nullptr;
}

Surface& Surface::operator=(Surface&& surface) {
    destroy();

    _surface = surface._surface;
    _instance = surface._instance;
    surface._surface = VK_NULL_HANDLE;
    surface._instance = nullptr;

    return *this;
}

Surface::~Surface() {
    destroy();
}

void Surface::destroy() {
    if (_surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(static_cast<VkInstance>(*_instance), _surface, nullptr);
        _surface = VK_NULL_HANDLE;
    }
}

} // API
} // Vulkan
} // Graphics
} // lug
