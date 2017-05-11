#include <lug/Graphics/Vulkan/API/Builder/Device.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

Device::Device(const PhysicalDeviceInfo& physicalDeviceInfo): _physicalDeviceInfo(physicalDeviceInfo) {}

int8_t Device::addQueue(VkQueueFlags queueFlags, uint8_t queueCount) {
    int8_t queueFamilyIndex = -1;

    // Search the queue family index corresponding to the queue flags
    bool queueFound = _physicalDeviceInfo.containsQueueFlags(queueFlags, queueFamilyIndex);

    // queueFlags contains VK_QUEUE_TRANSFER_BIT and queue is not found,
    // we can use either VK_QUEUE_GRAPHICS_BIT or VK_QUEUE_COMPUTE_BIT instead of VK_QUEUE_TRANSFER_BIT
    if (!queueFound && (queueFlags & VK_QUEUE_TRANSFER_BIT)) {
        // We try to find a queue first by replacing VK_QUEUE_TRANSFER_BIT with VK_QUEUE_GRAPHICS_BIT
        // then with VK_QUEUE_COMPUTE_BIT
        VkQueueFlags queueFlagsTmp = queueFlags & ~VK_QUEUE_TRANSFER_BIT;
        queueFound = _physicalDeviceInfo.containsQueueFlags(queueFlagsTmp | VK_QUEUE_GRAPHICS_BIT, queueFamilyIndex) ||
            _physicalDeviceInfo.containsQueueFlags(queueFlagsTmp | VK_QUEUE_COMPUTE_BIT, queueFamilyIndex);
    }

    // Queue not found
    if (!queueFound) {
        LUG_LOG.error("Device::addQueue: Can't find queue {}", RTTI::VkQueueFlagsToStr(queueFlags));
        return -1;
    }

    // Fill queueCreateInfo
    QueueCreateInfo queueCreateInfo {
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex,
        {} // queueCreateInfo.queuePriorities
    };
    queueCreateInfo.queuePriorities.resize(queueCount, 1.0f);

    _queueCreateInfos.push_back(queueCreateInfo);

    return queueFamilyIndex;
}

bool Device::build(API::Device& device, VkResult* returnResult) {
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(_queueCreateInfos.size());
    uint8_t i = 0;

    // Fill the queueCreateInfos
    for (const auto& queueCreateInfo : _queueCreateInfos) {
        queueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfos[i].queueFamilyIndex = queueCreateInfo.queueFamilyIndex;
        queueCreateInfos[i].queueCount = static_cast<uint32_t>(queueCreateInfo.queuePriorities.size());
        queueCreateInfos[i].pQueuePriorities = queueCreateInfo.queuePriorities.data();

        ++i;
    }

    // Create the device creation information for vkCreateDevice
    VkDeviceCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        createInfo.pQueueCreateInfos = queueCreateInfos.data(),
        createInfo.enabledLayerCount = 0, // Deprecated
        createInfo.ppEnabledLayerNames = nullptr, // Deprecated
        createInfo.enabledExtensionCount = static_cast<uint32_t>(_extensions.size()),
        createInfo.ppEnabledExtensionNames = _extensions.data(),
        createInfo.pEnabledFeatures = &_features
    };

    // Create the device
    VkDevice vkDevice{VK_NULL_HANDLE};
    VkResult result = vkCreateDevice(_physicalDeviceInfo.handle, &createInfo, nullptr, &vkDevice);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    device = API::Device(vkDevice, &_physicalDeviceInfo);

    return true;
}

std::unique_ptr<API::Device> Device::build(VkResult* returnResult) {
    std::unique_ptr<API::Device> device = std::make_unique<API::Device>();
    return build(*device, returnResult) ? std::move(device) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
