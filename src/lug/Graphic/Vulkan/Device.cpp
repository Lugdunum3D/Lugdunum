#include <lug/Graphic/Vulkan/Device.hpp>

namespace lug {
namespace Graphic {
namespace Vulkan {

Device::Device(VkDevice device) : _device(device) {}

Device::Device(Device&& device) {
    _device = device._device;
    device._device = VK_NULL_HANDLE;
}

Device& Device::operator=(Device&& device) {
    _device = device._device;
    device._device = VK_NULL_HANDLE;

    return *this;
}

Device::~Device() {
    destroy();
}

void Device::destroy() {
    if (_device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(_device);
        vkDestroyDevice(_device, nullptr);
        _device = VK_NULL_HANDLE;
    }
}

} // Vulkan
} // Graphic
} // lug
