#include <lug/Graphics/Vulkan/Vulkan.hpp>

#define LUG_VULKAN_PHYSICAL_DEVICE_TYPE(macro)      \
    macro(VK_PHYSICAL_DEVICE_TYPE_OTHER)            \
    macro(VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)   \
    macro(VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)     \
    macro(VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)      \
    macro(VK_PHYSICAL_DEVICE_TYPE_CPU)

#define LUG_VULKAN_FORMAT(macro)                 \
    macro(VK_FORMAT_UNDEFINED)                   \
    macro(VK_FORMAT_R4G4_UNORM_PACK8)            \
    macro(VK_FORMAT_R4G4B4A4_UNORM_PACK16)       \
    macro(VK_FORMAT_B4G4R4A4_UNORM_PACK16)       \
    macro(VK_FORMAT_R5G6B5_UNORM_PACK16)         \
    macro(VK_FORMAT_B5G6R5_UNORM_PACK16)         \
    macro(VK_FORMAT_R5G5B5A1_UNORM_PACK16)       \
    macro(VK_FORMAT_B5G5R5A1_UNORM_PACK16)       \
    macro(VK_FORMAT_A1R5G5B5_UNORM_PACK16)       \
    macro(VK_FORMAT_R8_UNORM)                    \
    macro(VK_FORMAT_R8_SNORM)                    \
    macro(VK_FORMAT_R8_USCALED)                  \
    macro(VK_FORMAT_R8_SSCALED)                  \
    macro(VK_FORMAT_R8_UINT)                     \
    macro(VK_FORMAT_R8_SINT)                     \
    macro(VK_FORMAT_R8_SRGB)                     \
    macro(VK_FORMAT_R8G8_UNORM)                  \
    macro(VK_FORMAT_R8G8_SNORM)                  \
    macro(VK_FORMAT_R8G8_USCALED)                \
    macro(VK_FORMAT_R8G8_SSCALED)                \
    macro(VK_FORMAT_R8G8_UINT)                   \
    macro(VK_FORMAT_R8G8_SINT)                   \
    macro(VK_FORMAT_R8G8_SRGB)                   \
    macro(VK_FORMAT_R8G8B8_UNORM)                \
    macro(VK_FORMAT_R8G8B8_SNORM)                \
    macro(VK_FORMAT_R8G8B8_USCALED)              \
    macro(VK_FORMAT_R8G8B8_SSCALED)              \
    macro(VK_FORMAT_R8G8B8_UINT)                 \
    macro(VK_FORMAT_R8G8B8_SINT)                 \
    macro(VK_FORMAT_R8G8B8_SRGB)                 \
    macro(VK_FORMAT_B8G8R8_UNORM)                \
    macro(VK_FORMAT_B8G8R8_SNORM)                \
    macro(VK_FORMAT_B8G8R8_USCALED)              \
    macro(VK_FORMAT_B8G8R8_SSCALED)              \
    macro(VK_FORMAT_B8G8R8_UINT)                 \
    macro(VK_FORMAT_B8G8R8_SINT)                 \
    macro(VK_FORMAT_B8G8R8_SRGB)                 \
    macro(VK_FORMAT_R8G8B8A8_UNORM)              \
    macro(VK_FORMAT_R8G8B8A8_SNORM)              \
    macro(VK_FORMAT_R8G8B8A8_USCALED)            \
    macro(VK_FORMAT_R8G8B8A8_SSCALED)            \
    macro(VK_FORMAT_R8G8B8A8_UINT)               \
    macro(VK_FORMAT_R8G8B8A8_SINT)               \
    macro(VK_FORMAT_R8G8B8A8_SRGB)               \
    macro(VK_FORMAT_B8G8R8A8_UNORM)              \
    macro(VK_FORMAT_B8G8R8A8_SNORM)              \
    macro(VK_FORMAT_B8G8R8A8_USCALED)            \
    macro(VK_FORMAT_B8G8R8A8_SSCALED)            \
    macro(VK_FORMAT_B8G8R8A8_UINT)               \
    macro(VK_FORMAT_B8G8R8A8_SINT)               \
    macro(VK_FORMAT_B8G8R8A8_SRGB)               \
    macro(VK_FORMAT_A8B8G8R8_UNORM_PACK32)       \
    macro(VK_FORMAT_A8B8G8R8_SNORM_PACK32)       \
    macro(VK_FORMAT_A8B8G8R8_USCALED_PACK32)     \
    macro(VK_FORMAT_A8B8G8R8_SSCALED_PACK32)     \
    macro(VK_FORMAT_A8B8G8R8_UINT_PACK32)        \
    macro(VK_FORMAT_A8B8G8R8_SINT_PACK32)        \
    macro(VK_FORMAT_A8B8G8R8_SRGB_PACK32)        \
    macro(VK_FORMAT_A2R10G10B10_UNORM_PACK32)    \
    macro(VK_FORMAT_A2R10G10B10_SNORM_PACK32)    \
    macro(VK_FORMAT_A2R10G10B10_USCALED_PACK32)  \
    macro(VK_FORMAT_A2R10G10B10_SSCALED_PACK32)  \
    macro(VK_FORMAT_A2R10G10B10_UINT_PACK32)     \
    macro(VK_FORMAT_A2R10G10B10_SINT_PACK32)     \
    macro(VK_FORMAT_A2B10G10R10_UNORM_PACK32)    \
    macro(VK_FORMAT_A2B10G10R10_SNORM_PACK32)    \
    macro(VK_FORMAT_A2B10G10R10_USCALED_PACK32)  \
    macro(VK_FORMAT_A2B10G10R10_SSCALED_PACK32)  \
    macro(VK_FORMAT_A2B10G10R10_UINT_PACK32)     \
    macro(VK_FORMAT_A2B10G10R10_SINT_PACK32)     \
    macro(VK_FORMAT_R16_UNORM)                   \
    macro(VK_FORMAT_R16_SNORM)                   \
    macro(VK_FORMAT_R16_USCALED)                 \
    macro(VK_FORMAT_R16_SSCALED)                 \
    macro(VK_FORMAT_R16_UINT)                    \
    macro(VK_FORMAT_R16_SINT)                    \
    macro(VK_FORMAT_R16_SFLOAT)                  \
    macro(VK_FORMAT_R16G16_UNORM)                \
    macro(VK_FORMAT_R16G16_SNORM)                \
    macro(VK_FORMAT_R16G16_USCALED)              \
    macro(VK_FORMAT_R16G16_SSCALED)              \
    macro(VK_FORMAT_R16G16_UINT)                 \
    macro(VK_FORMAT_R16G16_SINT)                 \
    macro(VK_FORMAT_R16G16_SFLOAT)               \
    macro(VK_FORMAT_R16G16B16_UNORM)             \
    macro(VK_FORMAT_R16G16B16_SNORM)             \
    macro(VK_FORMAT_R16G16B16_USCALED)           \
    macro(VK_FORMAT_R16G16B16_SSCALED)           \
    macro(VK_FORMAT_R16G16B16_UINT)              \
    macro(VK_FORMAT_R16G16B16_SINT)              \
    macro(VK_FORMAT_R16G16B16_SFLOAT)            \
    macro(VK_FORMAT_R16G16B16A16_UNORM)          \
    macro(VK_FORMAT_R16G16B16A16_SNORM)          \
    macro(VK_FORMAT_R16G16B16A16_USCALED)        \
    macro(VK_FORMAT_R16G16B16A16_SSCALED)        \
    macro(VK_FORMAT_R16G16B16A16_UINT)           \
    macro(VK_FORMAT_R16G16B16A16_SINT)           \
    macro(VK_FORMAT_R16G16B16A16_SFLOAT)         \
    macro(VK_FORMAT_R32_UINT)                    \
    macro(VK_FORMAT_R32_SINT)                    \
    macro(VK_FORMAT_R32_SFLOAT)                  \
    macro(VK_FORMAT_R32G32_UINT)                 \
    macro(VK_FORMAT_R32G32_SINT)                 \
    macro(VK_FORMAT_R32G32_SFLOAT)               \
    macro(VK_FORMAT_R32G32B32_UINT)              \
    macro(VK_FORMAT_R32G32B32_SINT)              \
    macro(VK_FORMAT_R32G32B32_SFLOAT)            \
    macro(VK_FORMAT_R32G32B32A32_UINT)           \
    macro(VK_FORMAT_R32G32B32A32_SINT)           \
    macro(VK_FORMAT_R32G32B32A32_SFLOAT)         \
    macro(VK_FORMAT_R64_UINT)                    \
    macro(VK_FORMAT_R64_SINT)                    \
    macro(VK_FORMAT_R64_SFLOAT)                  \
    macro(VK_FORMAT_R64G64_UINT)                 \
    macro(VK_FORMAT_R64G64_SINT)                 \
    macro(VK_FORMAT_R64G64_SFLOAT)               \
    macro(VK_FORMAT_R64G64B64_UINT)              \
    macro(VK_FORMAT_R64G64B64_SINT)              \
    macro(VK_FORMAT_R64G64B64_SFLOAT)            \
    macro(VK_FORMAT_R64G64B64A64_UINT)           \
    macro(VK_FORMAT_R64G64B64A64_SINT)           \
    macro(VK_FORMAT_R64G64B64A64_SFLOAT)         \
    macro(VK_FORMAT_B10G11R11_UFLOAT_PACK32)     \
    macro(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32)      \
    macro(VK_FORMAT_D16_UNORM)                   \
    macro(VK_FORMAT_X8_D24_UNORM_PACK32)         \
    macro(VK_FORMAT_D32_SFLOAT)                  \
    macro(VK_FORMAT_S8_UINT)                     \
    macro(VK_FORMAT_D16_UNORM_S8_UINT)           \
    macro(VK_FORMAT_D24_UNORM_S8_UINT)           \
    macro(VK_FORMAT_D32_SFLOAT_S8_UINT)          \
    macro(VK_FORMAT_BC1_RGB_UNORM_BLOCK)         \
    macro(VK_FORMAT_BC1_RGB_SRGB_BLOCK)          \
    macro(VK_FORMAT_BC1_RGBA_UNORM_BLOCK)        \
    macro(VK_FORMAT_BC1_RGBA_SRGB_BLOCK)         \
    macro(VK_FORMAT_BC2_UNORM_BLOCK)             \
    macro(VK_FORMAT_BC2_SRGB_BLOCK)              \
    macro(VK_FORMAT_BC3_UNORM_BLOCK)             \
    macro(VK_FORMAT_BC3_SRGB_BLOCK)              \
    macro(VK_FORMAT_BC4_UNORM_BLOCK)             \
    macro(VK_FORMAT_BC4_SNORM_BLOCK)             \
    macro(VK_FORMAT_BC5_UNORM_BLOCK)             \
    macro(VK_FORMAT_BC5_SNORM_BLOCK)             \
    macro(VK_FORMAT_BC6H_UFLOAT_BLOCK)           \
    macro(VK_FORMAT_BC6H_SFLOAT_BLOCK)           \
    macro(VK_FORMAT_BC7_UNORM_BLOCK)             \
    macro(VK_FORMAT_BC7_SRGB_BLOCK)              \
    macro(VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK)     \
    macro(VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK)      \
    macro(VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK)   \
    macro(VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK)    \
    macro(VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK)   \
    macro(VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK)    \
    macro(VK_FORMAT_EAC_R11_UNORM_BLOCK)         \
    macro(VK_FORMAT_EAC_R11_SNORM_BLOCK)         \
    macro(VK_FORMAT_EAC_R11G11_UNORM_BLOCK)      \
    macro(VK_FORMAT_EAC_R11G11_SNORM_BLOCK)      \
    macro(VK_FORMAT_ASTC_4x4_UNORM_BLOCK)        \
    macro(VK_FORMAT_ASTC_4x4_SRGB_BLOCK)         \
    macro(VK_FORMAT_ASTC_5x4_UNORM_BLOCK)        \
    macro(VK_FORMAT_ASTC_5x4_SRGB_BLOCK)         \
    macro(VK_FORMAT_ASTC_5x5_UNORM_BLOCK)        \
    macro(VK_FORMAT_ASTC_5x5_SRGB_BLOCK)         \
    macro(VK_FORMAT_ASTC_6x5_UNORM_BLOCK)        \
    macro(VK_FORMAT_ASTC_6x5_SRGB_BLOCK)         \
    macro(VK_FORMAT_ASTC_6x6_UNORM_BLOCK)        \
    macro(VK_FORMAT_ASTC_6x6_SRGB_BLOCK)         \
    macro(VK_FORMAT_ASTC_8x5_UNORM_BLOCK)        \
    macro(VK_FORMAT_ASTC_8x5_SRGB_BLOCK)         \
    macro(VK_FORMAT_ASTC_8x6_UNORM_BLOCK)        \
    macro(VK_FORMAT_ASTC_8x6_SRGB_BLOCK)         \
    macro(VK_FORMAT_ASTC_8x8_UNORM_BLOCK)        \
    macro(VK_FORMAT_ASTC_8x8_SRGB_BLOCK)         \
    macro(VK_FORMAT_ASTC_10x5_UNORM_BLOCK)       \
    macro(VK_FORMAT_ASTC_10x5_SRGB_BLOCK)        \
    macro(VK_FORMAT_ASTC_10x6_UNORM_BLOCK)       \
    macro(VK_FORMAT_ASTC_10x6_SRGB_BLOCK)        \
    macro(VK_FORMAT_ASTC_10x8_UNORM_BLOCK)       \
    macro(VK_FORMAT_ASTC_10x8_SRGB_BLOCK)        \
    macro(VK_FORMAT_ASTC_10x10_UNORM_BLOCK)      \
    macro(VK_FORMAT_ASTC_10x10_SRGB_BLOCK)       \
    macro(VK_FORMAT_ASTC_12x10_UNORM_BLOCK)      \
    macro(VK_FORMAT_ASTC_12x10_SRGB_BLOCK)       \
    macro(VK_FORMAT_ASTC_12x12_UNORM_BLOCK)      \
    macro(VK_FORMAT_ASTC_12x12_SRGB_BLOCK)

#define LUG_VULKAN_COLOR_SPACE_KHR(macro)       \
    macro(VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)

#define LUG_VULKAN_PRESENT_MODE_KHR(macro) \
    macro(VK_PRESENT_MODE_IMMEDIATE_KHR)   \
    macro(VK_PRESENT_MODE_MAILBOX_KHR)     \
    macro(VK_PRESENT_MODE_FIFO_KHR)        \
    macro(VK_PRESENT_MODE_FIFO_RELAXED_KHR)


#define LUG_VULKAN_MEMORY_PROPERTY_BIT(macro)       \
    macro(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)      \
    macro(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)      \
    macro(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)     \
    macro(VK_MEMORY_PROPERTY_HOST_CACHED_BIT)       \
    macro(VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)

#define LUG_VULKAN_QUEUE_BIT(macro)   \
    macro(VK_QUEUE_GRAPHICS_BIT)      \
    macro(VK_QUEUE_COMPUTE_BIT)       \
    macro(VK_QUEUE_TRANSFER_BIT)      \
    macro(VK_QUEUE_SPARSE_BINDING_BIT)

#if VK_HEADER_VERSION >= 39
#define LUG_VULKAN_FORMAT_FEATURE_BIT(macro)                    \
    macro(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)                  \
    macro(VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT)                  \
    macro(VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT)           \
    macro(VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)           \
    macro(VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT)           \
    macro(VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT)    \
    macro(VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)                  \
    macro(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)               \
    macro(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT)         \
    macro(VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)       \
    macro(VK_FORMAT_FEATURE_BLIT_SRC_BIT)                       \
    macro(VK_FORMAT_FEATURE_BLIT_DST_BIT)                       \
    macro(VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)    \
    macro(VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG) \
    macro(VK_FORMAT_FEATURE_TRANSFER_SRC_BIT_KHR)               \
    macro(VK_FORMAT_FEATURE_TRANSFER_DST_BIT_KHR)
#else
#define LUG_VULKAN_FORMAT_FEATURE_BIT(macro)                    \
    macro(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)                  \
    macro(VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT)                  \
    macro(VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT)           \
    macro(VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)           \
    macro(VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT)           \
    macro(VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT)    \
    macro(VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)                  \
    macro(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)               \
    macro(VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT)         \
    macro(VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)       \
    macro(VK_FORMAT_FEATURE_BLIT_SRC_BIT)                       \
    macro(VK_FORMAT_FEATURE_BLIT_DST_BIT)                       \
    macro(VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)    \
    macro(VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_IMG)
#endif

#define LUG_VULKAN_MEMORY_HEAP_BIT(macro) \
    macro(VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)

#define LUG_VULKAN_IMAGE_USAGE_BIT(macro)               \
    macro(VK_IMAGE_USAGE_TRANSFER_SRC_BIT)              \
    macro(VK_IMAGE_USAGE_TRANSFER_DST_BIT)              \
    macro(VK_IMAGE_USAGE_SAMPLED_BIT)                   \
    macro(VK_IMAGE_USAGE_STORAGE_BIT)                   \
    macro(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)          \
    macro(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT)  \
    macro(VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT)      \
    macro(VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT)

#define LUG_VULKAN_SAMPLE_COUNT_BIT(macro)  \
    macro(VK_SAMPLE_COUNT_1_BIT)            \
    macro(VK_SAMPLE_COUNT_2_BIT)            \
    macro(VK_SAMPLE_COUNT_4_BIT)            \
    macro(VK_SAMPLE_COUNT_8_BIT)            \
    macro(VK_SAMPLE_COUNT_16_BIT)           \
    macro(VK_SAMPLE_COUNT_32_BIT)           \
    macro(VK_SAMPLE_COUNT_64_BIT)

#define LUG_VULKAN_SURFACE_TRANSFORM_KHR_BIT(macro)                  \
    macro(VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)                     \
    macro(VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR)                    \
    macro(VK_SURFACE_TRANSFORM_ROTATE_180_BIT_KHR)                   \
    macro(VK_SURFACE_TRANSFORM_ROTATE_270_BIT_KHR)                   \
    macro(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_BIT_KHR)            \
    macro(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_90_BIT_KHR)  \
    macro(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_180_BIT_KHR) \
    macro(VK_SURFACE_TRANSFORM_HORIZONTAL_MIRROR_ROTATE_270_BIT_KHR) \
    macro(VK_SURFACE_TRANSFORM_INHERIT_BIT_KHR)

#define LUG_VULKAN_COMPOSITE_ALPHA_KHR_BIT(macro)       \
    macro(VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)            \
    macro(VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)    \
    macro(VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)   \
    macro(VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace RTTI {

LUG_GRAPHICS_API const char* toStr(VkResult enumVal);
LUG_GRAPHICS_API const char* toStr(VkPhysicalDeviceType enumVal);
LUG_GRAPHICS_API const char* toStr(VkFormat enumVal);
LUG_GRAPHICS_API const char* toStr(VkColorSpaceKHR enumVal);
LUG_GRAPHICS_API const char* toStr(VkPresentModeKHR enumVal);
LUG_GRAPHICS_API const char* toStr(VkSurfaceTransformFlagBitsKHR enumVal);
LUG_GRAPHICS_API const char* toStr(VkCompositeAlphaFlagBitsKHR enumVal);

LUG_GRAPHICS_API std::vector<const char*> VkMemoryPropertyFlagsToStrVec(VkMemoryPropertyFlags flag);
LUG_GRAPHICS_API std::vector<const char*> VkQueueFlagsToStrVec(VkQueueFlags flag);
LUG_GRAPHICS_API std::vector<const char*> VkFormatFeatureFlagsToStrVec(VkFormatFeatureFlags flag);
LUG_GRAPHICS_API std::vector<const char*> VkMemoryHeapFlagsToStrVec(VkMemoryHeapFlags flag);
LUG_GRAPHICS_API std::vector<const char*> VkImageUsageFlagsToStrVec(VkImageUsageFlags flag);
LUG_GRAPHICS_API std::vector<const char*> VkSampleCountFlagsToStrVec(VkSampleCountFlags flag);
LUG_GRAPHICS_API std::vector<const char*> VkSurfaceTransformFlagsKHRToStrVec(VkSurfaceTransformFlagsKHR flag);
LUG_GRAPHICS_API std::vector<const char*> VkCompositeAlphaFlagsKHRToStrVec(VkCompositeAlphaFlagsKHR flag);

LUG_GRAPHICS_API std::string VkMemoryPropertyFlagsToStr(VkMemoryPropertyFlags flag);
LUG_GRAPHICS_API std::string VkQueueFlagsToStr(VkQueueFlags flag);
LUG_GRAPHICS_API std::string VkFormatFeatureFlagsToStr(VkFormatFeatureFlags flag);
LUG_GRAPHICS_API std::string VkMemoryHeapFlagsToStr(VkMemoryHeapFlags flag);
LUG_GRAPHICS_API std::string VkImageUsageFlagsToStr(VkImageUsageFlags flag);
LUG_GRAPHICS_API std::string VkSampleCountFlagsToStr(VkSampleCountFlags flag);
LUG_GRAPHICS_API std::string VkSurfaceTransformFlagsKHRToStr(VkSurfaceTransformFlagsKHR flag);
LUG_GRAPHICS_API std::string VkCompositeAlphaFlagsKHRToStr(VkCompositeAlphaFlagsKHR flag);

} // RTTI
} // API
} // Vulkan
} // Graphics
} // lug
