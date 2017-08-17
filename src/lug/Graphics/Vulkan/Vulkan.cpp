#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <algorithm>
#include <cstring>

inline namespace Vulkan {

#define LUG_DEFINE_DEFINITION_VULKAN_FUNCTIONS(name) PFN_##name name = nullptr;
LUG_EXPORTED_VULKAN_FUNCTIONS(LUG_DEFINE_DEFINITION_VULKAN_FUNCTIONS);
LUG_CORE_VULKAN_FUNCTIONS(LUG_DEFINE_DEFINITION_VULKAN_FUNCTIONS);
LUG_INSTANCE_VULKAN_FUNCTIONS(LUG_DEFINE_DEFINITION_VULKAN_FUNCTIONS);
LUG_DEVICE_VULKAN_FUNCTIONS(LUG_DEFINE_DEFINITION_VULKAN_FUNCTIONS);
#undef LUG_DEFINE_DEFINITION_VULKAN_FUNCTIONS

} // Vulkan

namespace lug {
namespace Graphics {
namespace Vulkan {

bool InstanceInfo::containsExtension(const char* extensionName) const {
    auto compareExtensions = [&extensionName](const VkExtensionProperties& extension) {
        return std::strcmp(extension.extensionName, extensionName) == 0;
    };

    return std::find_if(extensions.cbegin(), extensions.cend(), compareExtensions) != extensions.cend();
}

bool InstanceInfo::containsLayer(const char* layerName) const {
    auto compareLayers = [&layerName](const VkLayerProperties& layer) {
        return std::strcmp(layer.layerName, layerName) == 0;
    };

    return std::find_if(layers.cbegin(), layers.cend(), compareLayers) != layers.cend();
}

bool PhysicalDeviceInfo::containsExtension(const char* extensionName) const {
    auto compareExtensions = [&extensionName](const VkExtensionProperties& extension) {
        return std::strcmp(extension.extensionName, extensionName) == 0;
    };

    return std::find_if(extensions.cbegin(), extensions.cend(), compareExtensions) != extensions.cend();
}

bool PhysicalDeviceInfo::containsQueueFlags(VkQueueFlags queueFlags, uint8_t& idx) const {
    bool matched = false;

    for (uint8_t i = 0; i < queueFamilies.size(); ++i) {
        // First match a queue that can have other flags
        if (queueFamilies[i].queueCount && (queueFamilies[i].queueFlags & queueFlags) == queueFlags) {
            // If that's the first, match it
            idx = i;
            matched = true;
            // If it's an exact match, no need to look any further
            if (queueFamilies[i].queueFlags == queueFlags) {
                return true;
            }
        }
    }

    return matched;
}

} // Vulkan
} // Graphics
} // lug
