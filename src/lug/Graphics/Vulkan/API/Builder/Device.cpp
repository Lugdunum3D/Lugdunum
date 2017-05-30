#include <lug/Graphics/Vulkan/API/Builder/Device.hpp>

#define NOMINMAX
#include <algorithm>

#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

Device::Device(const PhysicalDeviceInfo& physicalDeviceInfo): _physicalDeviceInfo(physicalDeviceInfo) {
    // Initialize the list of queues to the available queues
    _queueFamiliesInfos.resize(_physicalDeviceInfo.queueFamilies.size());
    for (uint32_t i = 0; i < _physicalDeviceInfo.queueFamilies.size(); ++i) {
        _queueFamiliesInfos[i].flags = _physicalDeviceInfo.queueFamilies[i].queueFlags;
        _queueFamiliesInfos[i].queues.resize(_physicalDeviceInfo.queueFamilies[i].queueCount);
    }
}

uint8_t Device::addQueues(VkQueueFlags queueFlags, const std::vector<std::string>& queuesNames) {
    int8_t queuesNb = 0;

    for (const std::string& queueName: queuesNames) {
        if (!addQueue(queueFlags, queueName)) {
            // If queueFlags contains VK_QUEUE_TRANSFER_BIT and queue is not found,
            // we can use either VK_QUEUE_GRAPHICS_BIT or VK_QUEUE_COMPUTE_BIT instead of VK_QUEUE_TRANSFER_BIT
            if (queueFlags & VK_QUEUE_TRANSFER_BIT) {
                // We try to find a queue first by replacing VK_QUEUE_TRANSFER_BIT with VK_QUEUE_GRAPHICS_BIT
                // then with VK_QUEUE_COMPUTE_BIT
                VkQueueFlags queueFlagsTmp = queueFlags & ~VK_QUEUE_TRANSFER_BIT;
                if (!addQueue(queueFlagsTmp | VK_QUEUE_GRAPHICS_BIT, queueName) &&
                    !addQueue(queueFlagsTmp | VK_QUEUE_COMPUTE_BIT, queueName)) {
                    return queuesNb;
                }
            } else {
                return queuesNb;
            }
        }

        ++queuesNb;
    }

    return queuesNb;
}

bool Device::build(API::Device& device, VkResult* returnResult) {
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
    const float priorities[256] = {1.0f};

    // Fill the queueCreateInfos
    for (uint32_t i = 0; i < _queueFamiliesInfos.size(); ++i) {
        if (_queueFamiliesInfos[i].queuesUsed == 0) {
            continue;
        }
        const VkDeviceQueueCreateInfo queueCreateInfo {
            /* queueCreateInfo.sType */ VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            /* queueCreateInfo.pNext */ nullptr,
            /* queueCreateInfo.flags */ 0,
            /* queueCreateInfo.queueFamilyIndex */ i,
            /* queueCreateInfo.queueCount */ _queueFamiliesInfos[i].queuesUsed,
            /* queueCreateInfo.pQueuePriorities */ priorities,
        };

        queueCreateInfos.push_back(std::move(queueCreateInfo));
    }

    // Create the device creation information for vkCreateDevice
    const VkDeviceCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.queueCreateInfoCount */ static_cast<uint32_t>(queueCreateInfos.size()),
        /* createInfo.pQueueCreateInfos */ queueCreateInfos.data(),
        /* createInfo.enabledLayerCount */ 0, // Deprecated
        /* createInfo.ppEnabledLayerNames */ nullptr, // Deprecated
        /* createInfo.enabledExtensionCount */ static_cast<uint32_t>(_extensions.size()),
        /* createInfo.ppEnabledExtensionNames */ _extensions.data(),
        /* createInfo.pEnabledFeatures */ &_features
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

    auto getDeviceQueue = device.getProcAddr<PFN_vkGetDeviceQueue>("vkGetDeviceQueue");
    if (!getDeviceQueue) {
        LUG_LOG.error("Device::addQueue: Can't load function vkGetDeviceQueue");
        return false;
    }

    // Create queues
    {
        device._queueFamilies.resize(_queueFamiliesInfos.size());

        for (uint32_t i = 0; i <  _queueFamiliesInfos.size(); ++i) {
            VkQueueFlags queueFlags = _queueFamiliesInfos[i].flags;

            device._queueFamilies[i] = API::QueueFamily(i, queueFlags);

            // Create queues for the queue family
            for (uint8_t j = 0; j < _queueFamiliesInfos[i].queuesUsed; ++j) {
                VkQueue queue{VK_NULL_HANDLE};
                getDeviceQueue(static_cast<VkDevice>(device), i, j, &queue);

                device._queueFamilies[i]._queues.push_back(API::Queue(queue));

                for (const std::string& name: _queueFamiliesInfos[i].queues[j].names) {
                    device._queueFamilies[i]._queuesIndices[name] = static_cast<uint32_t>(device._queueFamilies[i]._queues.size() - 1);
                }
            }
        }
    }

    return true;
}

std::unique_ptr<API::Device> Device::build(VkResult* returnResult) {
    std::unique_ptr<API::Device> device = std::make_unique<API::Device>();
    return build(*device, returnResult) ? std::move(device) : nullptr;
}

bool Device::addQueue(VkQueueFlags queueFlags, const std::string& queueName) {
    for (uint32_t queueFamilyIdx = 0; queueFamilyIdx < _physicalDeviceInfo.queueFamilies.size(); ++queueFamilyIdx) {
        if ((_queueFamiliesInfos[queueFamilyIdx].flags & queueFlags) != queueFlags) {
            continue;
        }

        for (uint32_t i = 0; i < _queueFamiliesInfos[queueFamilyIdx].queues.size(); ++i) {
            if (!_queueFamiliesInfos[queueFamilyIdx].queues[i].flagsUsed) {
                _queueFamiliesInfos[queueFamilyIdx].queuesUsed++;
                _queueFamiliesInfos[queueFamilyIdx].queues[i].flagsUsed |= queueFlags;
                _queueFamiliesInfos[queueFamilyIdx].queues[i].names.push_back(queueName);

                return true;
            }
        }
    }

    for (uint32_t queueFamilyIdx = 0; queueFamilyIdx < _physicalDeviceInfo.queueFamilies.size(); ++queueFamilyIdx) {
        if ((_queueFamiliesInfos[queueFamilyIdx].flags & queueFlags) != queueFlags) {
            continue;
        }

        for (uint32_t i = 0; i < _queueFamiliesInfos[queueFamilyIdx].queues.size(); ++i) {
            if (!(_queueFamiliesInfos[queueFamilyIdx].queues[i].flagsUsed & queueFlags)) {
                _queueFamiliesInfos[queueFamilyIdx].queuesUsed += _queueFamiliesInfos[queueFamilyIdx].queues[i].flagsUsed ? 0 : 1;
                _queueFamiliesInfos[queueFamilyIdx].queues[i].flagsUsed |= queueFlags;
                _queueFamiliesInfos[queueFamilyIdx].queues[i].names.push_back(queueName);

                return true;
            }
        }
    }

    return false;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
