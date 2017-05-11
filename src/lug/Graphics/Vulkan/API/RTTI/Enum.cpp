#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/System/Exception.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace RTTI {

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


#define LUG_FLAGS_TO_STR_VEC(enum)   \
    if (flag & enum) {           \
        retVal.push_back(#enum); \
    }                            \
    flag &= ~enum;

#define LUG_FLAGS_TO_STR_VEC_FUNCTION(type, macro)                                           \
    std::vector<const char*> type ## ToStrVec(type flag) {                                   \
        std::vector<const char*> retVal;                                                     \
                                                                                             \
        macro(LUG_FLAGS_TO_STR_VEC)                                                         \
        if (flag) {                                                                          \
            LUG_EXCEPT(RTTIException, "One value of the flag " #type " is not implemented"); \
        }                                                                                    \
        return retVal;                                                                       \
    }

LUG_FLAGS_TO_STR_VEC_FUNCTION(VkMemoryPropertyFlags, LUG_VULKAN_MEMORY_PROPERTY_BIT)
LUG_FLAGS_TO_STR_VEC_FUNCTION(VkQueueFlags, LUG_VULKAN_QUEUE_BIT)
LUG_FLAGS_TO_STR_VEC_FUNCTION(VkFormatFeatureFlags, LUG_VULKAN_FORMAT_FEATURE_BIT)
LUG_FLAGS_TO_STR_VEC_FUNCTION(VkMemoryHeapFlags, LUG_VULKAN_MEMORY_HEAP_BIT)
LUG_FLAGS_TO_STR_VEC_FUNCTION(VkImageUsageFlags, LUG_VULKAN_IMAGE_USAGE_BIT)
LUG_FLAGS_TO_STR_VEC_FUNCTION(VkSampleCountFlags, LUG_VULKAN_SAMPLE_COUNT_BIT)
LUG_FLAGS_TO_STR_VEC_FUNCTION(VkSurfaceTransformFlagsKHR, LUG_VULKAN_SURFACE_TRANSFORM_KHR_BIT)
LUG_FLAGS_TO_STR_VEC_FUNCTION(VkCompositeAlphaFlagsKHR, LUG_VULKAN_COMPOSITE_ALPHA_KHR_BIT)

#define LUG_FLAGS_TO_STR(enum)  \
    if (flag & enum) {          \
        val += #enum;           \
        if ((flag & ~enum))     \
            val += " | ";       \
    }                           \
    flag &= ~enum;

#define LUG_FLAGS_TO_STR_FUNCTION(type, macro)                                              \
    std::string type ## ToStr(type flag) {                                                  \
        std::string val;                                                                    \
                                                                                            \
        macro(LUG_FLAGS_TO_STR)                                                             \
        if (flag) {                                                                         \
            LUG_EXCEPT(RTTIException, "One value of the flag " #type " is not implemented");\
        }                                                                                   \
        return val;                                                                         \
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
