#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/System/Exception.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace RTTI {

const char* toStr(VkResult enumVal) {
    switch (enumVal) {
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

#define LUG_ENUM_TO_STR(enum) \
    case enum:                \
        return #enum;
#define LUG_ENUM_TO_STR_FUNCTION(type, macro)                                                    \
    const char* toStr(type enumVal) {                                                            \
        switch (enumVal) {                                                                       \
            macro(LUG_ENUM_TO_STR)                                                               \
            default:                                                                             \
                LUG_EXCEPT(RTTIException, "One value of the enum " #type " is not implemented"); \
        }                                                                                        \
    }

LUG_ENUM_TO_STR_FUNCTION(VkPhysicalDeviceType, LUG_VULKAN_PHYSICAL_DEVICE_TYPE)
LUG_ENUM_TO_STR_FUNCTION(VkFormat, LUG_VULKAN_FORMAT)
LUG_ENUM_TO_STR_FUNCTION(VkColorSpaceKHR, LUG_VULKAN_COLOR_SPACE_KHR)
LUG_ENUM_TO_STR_FUNCTION(VkPresentModeKHR, LUG_VULKAN_PRESENT_MODE_KHR)

LUG_ENUM_TO_STR_FUNCTION(VkMemoryPropertyFlagBits, LUG_VULKAN_MEMORY_PROPERTY_BIT)
LUG_ENUM_TO_STR_FUNCTION(VkQueueFlagBits, LUG_VULKAN_QUEUE_BIT)
LUG_ENUM_TO_STR_FUNCTION(VkFormatFeatureFlagBits, LUG_VULKAN_FORMAT_FEATURE_BIT)
LUG_ENUM_TO_STR_FUNCTION(VkMemoryHeapFlagBits, LUG_VULKAN_MEMORY_HEAP_BIT)
LUG_ENUM_TO_STR_FUNCTION(VkImageUsageFlagBits, LUG_VULKAN_IMAGE_USAGE_BIT)
LUG_ENUM_TO_STR_FUNCTION(VkSampleCountFlagBits, LUG_VULKAN_SAMPLE_COUNT_BIT)
LUG_ENUM_TO_STR_FUNCTION(VkSurfaceTransformFlagBitsKHR, LUG_VULKAN_SURFACE_TRANSFORM_KHR_BIT)
LUG_ENUM_TO_STR_FUNCTION(VkCompositeAlphaFlagBitsKHR, LUG_VULKAN_COMPOSITE_ALPHA_KHR_BIT)


#define LUG_FLAGS_TO_STR(enum)   \
    if (flag & enum) {           \
        retVal.push_back(#enum); \
    }                            \
    flag &= ~enum;

#define LUG_FLAGS_TO_STR_FUNCTION(type, macro)                                               \
    std::vector<const char*> type ## ToStr(type flag) {                                      \
        std::vector<const char*> retVal;                                                     \
                                                                                             \
        macro(LUG_FLAGS_TO_STR)                                                              \
        if (flag) {                                                                          \
            LUG_EXCEPT(RTTIException, "One value of the flag " #type " is not implemented"); \
        }                                                                                    \
        return retVal;                                                                       \
    }

LUG_FLAGS_TO_STR_FUNCTION(VkMemoryPropertyFlags, LUG_VULKAN_MEMORY_PROPERTY_BIT)
LUG_FLAGS_TO_STR_FUNCTION(VkQueueFlags, LUG_VULKAN_QUEUE_BIT)
LUG_FLAGS_TO_STR_FUNCTION(VkFormatFeatureFlags, LUG_VULKAN_FORMAT_FEATURE_BIT)
LUG_FLAGS_TO_STR_FUNCTION(VkMemoryHeapFlags, LUG_VULKAN_MEMORY_HEAP_BIT)
LUG_FLAGS_TO_STR_FUNCTION(VkImageUsageFlags, LUG_VULKAN_IMAGE_USAGE_BIT)
LUG_FLAGS_TO_STR_FUNCTION(VkSampleCountFlags, LUG_VULKAN_SAMPLE_COUNT_BIT)
LUG_FLAGS_TO_STR_FUNCTION(VkSurfaceTransformFlagsKHR, LUG_VULKAN_SURFACE_TRANSFORM_KHR_BIT)
LUG_FLAGS_TO_STR_FUNCTION(VkCompositeAlphaFlagsKHR, LUG_VULKAN_COMPOSITE_ALPHA_KHR_BIT)

} // RTTI
} // API
} // Vulkan
} // Graphics
} // lug
