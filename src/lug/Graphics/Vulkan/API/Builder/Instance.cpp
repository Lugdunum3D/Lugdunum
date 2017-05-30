#include <lug/Graphics/Vulkan/API/Builder/Instance.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

bool Instance::build(API::Instance& instance, VkResult* returnResult) {
    // Create the application information for vkCreateInstance
    const VkApplicationInfo applicationInfo {
        /* applicationInfo.sType */ VK_STRUCTURE_TYPE_APPLICATION_INFO,
        /* applicationInfo.pNext */ nullptr,
        /* applicationInfo.pApplicationName */ _appInfo.appName.data(),
        /* applicationInfo.applicationVersion */ static_cast<uint32_t>(_appInfo.appVersion),
        /* applicationInfo.pEngineName */ _appInfo.engineName.data(),
        /* applicationInfo.engineVersion */ static_cast<uint32_t>(_appInfo.engineVersion),
        /* applicationInfo.apiVersion */ static_cast<uint32_t>(_appInfo.apiVersion),
    };

    // Create the instance creation information for vkCreateInstance
    const VkInstanceCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.pApplicationInfo */ &applicationInfo,
        /* createInfo.enabledLayerCount */ static_cast<uint32_t>(_layers.size()),
        /* createInfo.ppEnabledLayerNames */ _layers.data(),
        /* createInfo.enabledExtensionCount */ static_cast<uint32_t>(_extensions.size()),
        /* createInfo.ppEnabledExtensionNames */ _extensions.data()
    };

    // Create the instance
    VkInstance vkInstance{VK_NULL_HANDLE};
    VkResult result = vkCreateInstance(&createInfo, nullptr, &vkInstance);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    instance = API::Instance(vkInstance);

    return true;
}

std::unique_ptr<API::Instance> Instance::build(VkResult* returnResult) {
    std::unique_ptr<API::Instance> instance = std::make_unique<API::Instance>();
    return build(*instance, returnResult) ? std::move(instance) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
