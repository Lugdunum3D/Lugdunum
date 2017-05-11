#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/System/Debug.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

Device::Device(VkDevice device, const PhysicalDeviceInfo* physicalDeviceInfo) : _device(device), _physicalDeviceInfo(physicalDeviceInfo) {}

Device::Device(Device&& device) {
    _device = device._device;
    _physicalDeviceInfo = device._physicalDeviceInfo;
    device._device = VK_NULL_HANDLE;
    device._physicalDeviceInfo = nullptr;
}

Device& Device::operator=(Device&& device) {
    destroy();

    _device = device._device;
    _physicalDeviceInfo = device._physicalDeviceInfo;
    device._device = VK_NULL_HANDLE;
    device._physicalDeviceInfo = nullptr;

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

} // API
} // Vulkan
} // Graphics
} // lug
