#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/System/Debug.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Device::Device(VkDevice device, PhysicalDeviceInfo* physicalDeviceInfo) : _device(device), _physicalDeviceInfo(physicalDeviceInfo) {}

Device::Device(Device&& device) {
    _device = device._device;
    device._device = VK_NULL_HANDLE;
}

Device& Device::operator=(Device&& device) {
    destroy();

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

const PhysicalDeviceInfo* Device::getPhysicalDeviceInfo() const {
    LUG_ASSERT(_physicalDeviceInfo != nullptr, "Device::_physicalDeviceInfo should not be null");
    return _physicalDeviceInfo;
}

} // Vulkan
} // Graphics
} // lug
