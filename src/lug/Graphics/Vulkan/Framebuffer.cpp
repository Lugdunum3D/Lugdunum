#include <lug/Graphics/Vulkan/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Framebuffer::Framebuffer(VkFramebuffer framebuffer, const Device* device) : _framebuffer(framebuffer), _device(device) {}

Framebuffer::Framebuffer(Framebuffer&& framebuffer) {
    _framebuffer = framebuffer._framebuffer;
    _device = framebuffer._device;
    framebuffer._framebuffer = VK_NULL_HANDLE;
    framebuffer._device = nullptr;
}

Framebuffer& Framebuffer::operator=(Framebuffer&& framebuffer) {
    destroy();

    _framebuffer = framebuffer._framebuffer;
    _device = framebuffer._device;
    framebuffer._framebuffer = VK_NULL_HANDLE;
    framebuffer._device = nullptr;

    return *this;
}

Framebuffer::~Framebuffer() {
    destroy();
}

void Framebuffer::destroy() {
    if (_framebuffer != VK_NULL_HANDLE) {
        vkDestroyFramebuffer(*_device, _framebuffer, nullptr);
        _framebuffer = VK_NULL_HANDLE;
    }
}

} // Vulkan
} // Graphics
} // lug
