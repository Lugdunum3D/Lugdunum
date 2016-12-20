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

bool PhysicalDeviceInfo::containsQueueFlags(VkQueueFlags queueFlags, int8_t& idx) const {
    idx = -1;

    for (uint8_t i = 0; i < queueFamilies.size(); ++i) {
        if (queueFamilies[i].queueCount && (queueFamilies[i].queueFlags & queueFlags) == queueFlags) {
            if (idx == -1 || queueFamilies[i].queueFlags == queueFlags) {
                idx = i;
            }
        }
    }

    return idx != -1;
}

const char* resultToStr(VkResult result) {
    switch (result) {
        case VK_SUCCESS:
            return "Success";
        case VK_NOT_READY:
            return "A fence or query has not yet completed";
        case VK_TIMEOUT:
            return "A wait operation has not completed in the specified time";
        case VK_EVENT_SET:
            return "An event is signaled";
        case VK_EVENT_RESET:
            return "An event is unsignaled";
        case VK_INCOMPLETE:
            return "A return array was too small for the result";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "A host memory allocation has failed";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "A device memory allocation has failed";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "Initialization of an object could not be completed for implementation-specific reasons";
        case VK_ERROR_DEVICE_LOST:
            return "The logical or physical device has been lost";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "Mapping of a memory object has failed";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "A requested layer is not present or could not be loaded";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "A requested extension is not supported";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "A requested feature is not supported";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "Too many objects of the type have already been created";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "A requested format is not supported on this device";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "A surface is no longer available";
        case VK_SUBOPTIMAL_KHR:
            return "A swapchain no longer matches the surface properties exactly, but can still be used";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "A surface has changed in such a way that it is no longer compatible with the swapchain";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "The display used by a swapchain does not use the same presentable image layout";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "A validation layer found an error";
        default:
            return "ERROR: UNKNOWN VULKAN ERROR";
    }
}

} // Vulkan
} // Graphics
} // lug
