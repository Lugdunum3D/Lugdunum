#include <lug/Graphic/Vulkan/Vulkan.hpp>
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
namespace Graphic {
namespace Vulkan {

bool InstanceInfo::containsExtension(const char* extensionName) const {
    auto compareExtensions = [&extensionName](const VkExtensionProperties& extension) {
        return std::strcmp(extension.extensionName, extensionName) == 0;
    };

    return std::find_if(extensions.begin(), extensions.end(), compareExtensions) != extensions.end();
}

bool InstanceInfo::containsLayer(const char* layerName) const {
    auto compareLayers = [&layerName](const VkLayerProperties& layer) {
        return std::strcmp(layer.layerName, layerName) == 0;
    };

    return std::find_if(layers.begin(), layers.end(), compareLayers) != layers.end();
}

} // Vulkan
} // Graphic
} // lug
