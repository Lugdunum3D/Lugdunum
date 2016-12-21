#include <lug/Graphics/Vulkan/Swapchain.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Swapchain::Swapchain(VkSwapchainKHR swapchain, const Device* device) : _swapchain(swapchain), _device(device) {}

Swapchain::Swapchain(Swapchain&& swapchain) {
    _swapchain = swapchain._swapchain;
    _device = swapchain._device;
    swapchain._swapchain = VK_NULL_HANDLE;
    swapchain._device = nullptr;
}

Swapchain& Swapchain::operator=(Swapchain&& swapchain) {
    _swapchain = swapchain._swapchain;
    _device = swapchain._device;
    swapchain._swapchain = VK_NULL_HANDLE;
    swapchain._device = nullptr;

    return *this;
}

Swapchain::~Swapchain() {
    destroy();
}

void Swapchain::destroy() {
    if (_swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(*_device, _swapchain, nullptr);
        _swapchain = VK_NULL_HANDLE;
        _device = nullptr;
    }
}

} // Vulkan
} // Graphics
} // lug
