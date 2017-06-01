#include <lug/Graphics/Vulkan/API/Framebuffer.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

Framebuffer::Framebuffer(VkFramebuffer framebuffer, const Device* device, const Extent& extent) : _framebuffer(framebuffer), _device(device), _extent(extent) {}

Framebuffer::Framebuffer(Framebuffer&& framebuffer) {
    _framebuffer = framebuffer._framebuffer;
    _device = framebuffer._device;
    _extent = framebuffer._extent;
    framebuffer._framebuffer = VK_NULL_HANDLE;
    framebuffer._device = nullptr;
    framebuffer._extent = {0, 0};
}

Framebuffer& Framebuffer::operator=(Framebuffer&& framebuffer) {
    destroy();

    _framebuffer = framebuffer._framebuffer;
    _device = framebuffer._device;
    _extent = framebuffer._extent;
    framebuffer._framebuffer = VK_NULL_HANDLE;
    framebuffer._device = nullptr;
    framebuffer._extent = {0, 0};

    return *this;
}

Framebuffer::~Framebuffer() {
    destroy();
}

void Framebuffer::destroy() {
    if (_framebuffer != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(static_cast<VkDevice>(*_device), _framebuffer, nullptr);
        _framebuffer = VK_NULL_HANDLE;
    }
}

} // API
} // Vulkan
} // Graphics
} // lug
