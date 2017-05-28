#include <lug/Graphics/Vulkan/API/Device.hpp>

#include <lug/System/Logger/Logger.hpp>

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

const API::QueueFamily* Device::getQueueFamily(VkQueueFlags flags, bool supportPresentation) const {
    const API::QueueFamily* returnQueue = nullptr;

    for (auto& queueFamily : _queueFamilies) {
        if ((queueFamily.getFlags() & flags) == flags && (!supportPresentation || queueFamily.supportsPresentation())) {
            if (returnQueue == nullptr || queueFamily.getFlags() == flags || (supportPresentation && flags == 0 && queueFamily.getFlags() & VK_QUEUE_GRAPHICS_BIT)) {
                returnQueue = &queueFamily;
            }
        }
    }

    if (!returnQueue && flags & VK_QUEUE_TRANSFER_BIT) {
        return getQueueFamily((flags & ~VK_QUEUE_TRANSFER_BIT) | VK_QUEUE_GRAPHICS_BIT, supportPresentation);
    }

    return returnQueue;
}

API::QueueFamily* Device::getQueueFamily(VkQueueFlags flags, bool supportPresentation) {
    API::QueueFamily* returnQueue = nullptr;

    for (auto& queueFamily : _queueFamilies) {
        if ((queueFamily.getFlags() & flags) == flags && (!supportPresentation || queueFamily.supportsPresentation())) {
            if (returnQueue == nullptr || queueFamily.getFlags() == flags || (supportPresentation && flags == 0 && queueFamily.getFlags() & VK_QUEUE_GRAPHICS_BIT)) {
                returnQueue = &queueFamily;
            }
        }
    }

    if (!returnQueue && flags & VK_QUEUE_TRANSFER_BIT) {
        return getQueueFamily((flags & ~VK_QUEUE_TRANSFER_BIT) | VK_QUEUE_GRAPHICS_BIT, supportPresentation);
    }

    return returnQueue;
}

bool Device::waitIdle() const {
    VkResult result = vkDeviceWaitIdle(_device);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("Device::waitIdle: Can't wait for queue work: {}", result);
        return false;
    }

    return true;
}

void Device::destroy() {
    if (_device != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(_device);
        vkDestroyDevice(_device, nullptr);
        _device = VK_NULL_HANDLE;
    }
}

} // API
} // Vulkan
} // Graphics
} // lug
