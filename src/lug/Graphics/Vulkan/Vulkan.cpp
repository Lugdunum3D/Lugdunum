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

const char * enumToStr(VkResult enumVal) {
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

const char * enumToStr(VkPhysicalDeviceType enumVal) {
    switch(enumVal)
    {
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
        return "VK_PHYSICAL_DEVICE_TYPE_OTHER";
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
        return "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
        return "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
        return "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
        return "VK_PHYSICAL_DEVICE_TYPE_CPU";
    }
}

std::vector<const char*> enumToStr(VkMemoryPropertyFlags enumVal) {
    std::vector<const char*> retVal;

    if (enumVal & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) { retVal.push_back("VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT"); }
    if (enumVal & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) { retVal.push_back("VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT"); }
    if (enumVal & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) { retVal.push_back("VK_MEMORY_PROPERTY_HOST_COHERENT_BIT"); }
    if (enumVal & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) { retVal.push_back("VK_MEMORY_PROPERTY_HOST_CACHED_BIT"); }
    if (enumVal & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) { retVal.push_back("VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT"); }
    if (enumVal & VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM) { retVal.push_back("VK_MEMORY_PROPERTY_FLAG_BITS_MAX_ENUM"); }

    return retVal;
}

std::vector<const char*> enumToStr(VkQueueFlags enumVal) {
    std::vector<const char*> retVal;

    if (enumVal & VK_QUEUE_GRAPHICS_BIT) { retVal.push_back("VK_QUEUE_GRAPHICS_BIT"); }
    if (enumVal & VK_QUEUE_COMPUTE_BIT) { retVal.push_back("VK_QUEUE_COMPUTE_BIT"); }
    if (enumVal & VK_QUEUE_TRANSFER_BIT) { retVal.push_back("VK_QUEUE_TRANSFER_BIT"); }
    if (enumVal & VK_QUEUE_SPARSE_BINDING_BIT) { retVal.push_back("VK_QUEUE_SPARSE_BINDING_BIT"); }

    return retVal;
}

const char* enumToStr(VkFormat enumVal) {
    switch (enumVal)
    {
    case VK_FORMAT_UNDEFINED: return                    "VK_FORMAT_UNDEFINED";
    case VK_FORMAT_R4G4_UNORM_PACK8: return            "VK_FORMAT_R4G4_UNORM_PACK8";
    case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return       "VK_FORMAT_R4G4B4A4_UNORM_PACK16";
    case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return       "VK_FORMAT_B4G4R4A4_UNORM_PACK16";
    case VK_FORMAT_R5G6B5_UNORM_PACK16: return         "VK_FORMAT_R5G6B5_UNORM_PACK16";
    case VK_FORMAT_B5G6R5_UNORM_PACK16: return         "VK_FORMAT_B5G6R5_UNORM_PACK16";
    case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return       "VK_FORMAT_R5G5B5A1_UNORM_PACK16";
    case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return       "VK_FORMAT_B5G5R5A1_UNORM_PACK16";
    case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return       "VK_FORMAT_A1R5G5B5_UNORM_PACK16";
    case VK_FORMAT_R8_UNORM: return                    "VK_FORMAT_R8_UNORM";
    case VK_FORMAT_R8_SNORM: return                    "VK_FORMAT_R8_SNORM";
    case VK_FORMAT_R8_USCALED: return                  "VK_FORMAT_R8_USCALED";
    case VK_FORMAT_R8_SSCALED: return                  "VK_FORMAT_R8_SSCALED";
    case VK_FORMAT_R8_UINT: return                     "VK_FORMAT_R8_UINT";
    case VK_FORMAT_R8_SINT: return                     "VK_FORMAT_R8_SINT";
    case VK_FORMAT_R8_SRGB: return                     "VK_FORMAT_R8_SRGB";
    case VK_FORMAT_R8G8_UNORM: return                  "VK_FORMAT_R8G8_UNORM";
    case VK_FORMAT_R8G8_SNORM: return                  "VK_FORMAT_R8G8_SNORM";
    case VK_FORMAT_R8G8_USCALED: return                "VK_FORMAT_R8G8_USCALED";
    case VK_FORMAT_R8G8_SSCALED: return                "VK_FORMAT_R8G8_SSCALED";
    case VK_FORMAT_R8G8_UINT: return                   "VK_FORMAT_R8G8_UINT";
    case VK_FORMAT_R8G8_SINT: return                   "VK_FORMAT_R8G8_SINT";
    case VK_FORMAT_R8G8_SRGB: return                   "VK_FORMAT_R8G8_SRGB";
    case VK_FORMAT_R8G8B8_UNORM: return                "VK_FORMAT_R8G8B8_UNORM";
    case VK_FORMAT_R8G8B8_SNORM: return                "VK_FORMAT_R8G8B8_SNORM";
    case VK_FORMAT_R8G8B8_USCALED: return              "VK_FORMAT_R8G8B8_USCALED";
    case VK_FORMAT_R8G8B8_SSCALED: return              "VK_FORMAT_R8G8B8_SSCALED";
    case VK_FORMAT_R8G8B8_UINT: return                 "VK_FORMAT_R8G8B8_UINT";
    case VK_FORMAT_R8G8B8_SINT: return                 "VK_FORMAT_R8G8B8_SINT";
    case VK_FORMAT_R8G8B8_SRGB: return                 "VK_FORMAT_R8G8B8_SRGB";
    case VK_FORMAT_B8G8R8_UNORM: return                "VK_FORMAT_B8G8R8_UNORM";
    case VK_FORMAT_B8G8R8_SNORM: return                "VK_FORMAT_B8G8R8_SNORM";
    case VK_FORMAT_B8G8R8_USCALED: return              "VK_FORMAT_B8G8R8_USCALED";
    case VK_FORMAT_B8G8R8_SSCALED: return              "VK_FORMAT_B8G8R8_SSCALED";
    case VK_FORMAT_B8G8R8_UINT: return                 "VK_FORMAT_B8G8R8_UINT";
    case VK_FORMAT_B8G8R8_SINT: return                 "VK_FORMAT_B8G8R8_SINT";
    case VK_FORMAT_B8G8R8_SRGB: return                 "VK_FORMAT_B8G8R8_SRGB";
    case VK_FORMAT_R8G8B8A8_UNORM: return              "VK_FORMAT_R8G8B8A8_UNORM";
    case VK_FORMAT_R8G8B8A8_SNORM: return              "VK_FORMAT_R8G8B8A8_SNORM";
    case VK_FORMAT_R8G8B8A8_USCALED: return            "VK_FORMAT_R8G8B8A8_USCALED";
    case VK_FORMAT_R8G8B8A8_SSCALED: return            "VK_FORMAT_R8G8B8A8_SSCALED";
    case VK_FORMAT_R8G8B8A8_UINT: return               "VK_FORMAT_R8G8B8A8_UINT";
    case VK_FORMAT_R8G8B8A8_SINT: return               "VK_FORMAT_R8G8B8A8_SINT";
    case VK_FORMAT_R8G8B8A8_SRGB: return               "VK_FORMAT_R8G8B8A8_SRGB";
    case VK_FORMAT_B8G8R8A8_UNORM: return              "VK_FORMAT_B8G8R8A8_UNORM";
    case VK_FORMAT_B8G8R8A8_SNORM: return              "VK_FORMAT_B8G8R8A8_SNORM";
    case VK_FORMAT_B8G8R8A8_USCALED: return            "VK_FORMAT_B8G8R8A8_USCALED";
    case VK_FORMAT_B8G8R8A8_SSCALED: return            "VK_FORMAT_B8G8R8A8_SSCALED";
    case VK_FORMAT_B8G8R8A8_UINT: return               "VK_FORMAT_B8G8R8A8_UINT";
    case VK_FORMAT_B8G8R8A8_SINT: return               "VK_FORMAT_B8G8R8A8_SINT";
    case VK_FORMAT_B8G8R8A8_SRGB: return               "VK_FORMAT_B8G8R8A8_SRGB";
    case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return       "VK_FORMAT_A8B8G8R8_UNORM_PACK32";
    case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return       "VK_FORMAT_A8B8G8R8_SNORM_PACK32";
    case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return     "VK_FORMAT_A8B8G8R8_USCALED_PACK32";
    case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return     "VK_FORMAT_A8B8G8R8_SSCALED_PACK32";
    case VK_FORMAT_A8B8G8R8_UINT_PACK32: return        "VK_FORMAT_A8B8G8R8_UINT_PACK32";
    case VK_FORMAT_A8B8G8R8_SINT_PACK32: return        "VK_FORMAT_A8B8G8R8_SINT_PACK32";
    case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return        "VK_FORMAT_A8B8G8R8_SRGB_PACK32";
    case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return    "VK_FORMAT_A2R10G10B10_UNORM_PACK32";
    case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return    "VK_FORMAT_A2R10G10B10_SNORM_PACK32";
    case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return  "VK_FORMAT_A2R10G10B10_USCALED_PACK32";
    case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return  "VK_FORMAT_A2R10G10B10_SSCALED_PACK32";
    case VK_FORMAT_A2R10G10B10_UINT_PACK32: return     "VK_FORMAT_A2R10G10B10_UINT_PACK32";
    case VK_FORMAT_A2R10G10B10_SINT_PACK32: return     "VK_FORMAT_A2R10G10B10_SINT_PACK32";
    case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return    "VK_FORMAT_A2B10G10R10_UNORM_PACK32";
    case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return    "VK_FORMAT_A2B10G10R10_SNORM_PACK32";
    case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return  "VK_FORMAT_A2B10G10R10_USCALED_PACK32";
    case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return  "VK_FORMAT_A2B10G10R10_SSCALED_PACK32";
    case VK_FORMAT_A2B10G10R10_UINT_PACK32: return     "VK_FORMAT_A2B10G10R10_UINT_PACK32";
    case VK_FORMAT_A2B10G10R10_SINT_PACK32: return     "VK_FORMAT_A2B10G10R10_SINT_PACK32";
    case VK_FORMAT_R16_UNORM: return                   "VK_FORMAT_R16_UNORM";
    case VK_FORMAT_R16_SNORM: return                   "VK_FORMAT_R16_SNORM";
    case VK_FORMAT_R16_USCALED: return                 "VK_FORMAT_R16_USCALED";
    case VK_FORMAT_R16_SSCALED: return                 "VK_FORMAT_R16_SSCALED";
    case VK_FORMAT_R16_UINT: return                    "VK_FORMAT_R16_UINT";
    case VK_FORMAT_R16_SINT: return                    "VK_FORMAT_R16_SINT";
    case VK_FORMAT_R16_SFLOAT: return                  "VK_FORMAT_R16_SFLOAT";
    case VK_FORMAT_R16G16_UNORM: return                "VK_FORMAT_R16G16_UNORM";
    case VK_FORMAT_R16G16_SNORM: return                "VK_FORMAT_R16G16_SNORM";
    case VK_FORMAT_R16G16_USCALED: return              "VK_FORMAT_R16G16_USCALED";
    case VK_FORMAT_R16G16_SSCALED: return              "VK_FORMAT_R16G16_SSCALED";
    case VK_FORMAT_R16G16_UINT: return                 "VK_FORMAT_R16G16_UINT";
    case VK_FORMAT_R16G16_SINT: return                 "VK_FORMAT_R16G16_SINT";
    case VK_FORMAT_R16G16_SFLOAT: return               "VK_FORMAT_R16G16_SFLOAT";
    case VK_FORMAT_R16G16B16_UNORM: return             "VK_FORMAT_R16G16B16_UNORM";
    case VK_FORMAT_R16G16B16_SNORM: return             "VK_FORMAT_R16G16B16_SNORM";
    case VK_FORMAT_R16G16B16_USCALED: return           "VK_FORMAT_R16G16B16_USCALED";
    case VK_FORMAT_R16G16B16_SSCALED: return           "VK_FORMAT_R16G16B16_SSCALED";
    case VK_FORMAT_R16G16B16_UINT: return              "VK_FORMAT_R16G16B16_UINT";
    case VK_FORMAT_R16G16B16_SINT: return              "VK_FORMAT_R16G16B16_SINT";
    case VK_FORMAT_R16G16B16_SFLOAT: return            "VK_FORMAT_R16G16B16_SFLOAT";
    case VK_FORMAT_R16G16B16A16_UNORM: return          "VK_FORMAT_R16G16B16A16_UNORM";
    case VK_FORMAT_R16G16B16A16_SNORM: return          "VK_FORMAT_R16G16B16A16_SNORM";
    case VK_FORMAT_R16G16B16A16_USCALED: return        "VK_FORMAT_R16G16B16A16_USCALED";
    case VK_FORMAT_R16G16B16A16_SSCALED: return        "VK_FORMAT_R16G16B16A16_SSCALED";
    case VK_FORMAT_R16G16B16A16_UINT: return           "VK_FORMAT_R16G16B16A16_UINT";
    case VK_FORMAT_R16G16B16A16_SINT: return           "VK_FORMAT_R16G16B16A16_SINT";
    case VK_FORMAT_R16G16B16A16_SFLOAT: return         "VK_FORMAT_R16G16B16A16_SFLOAT";
    case VK_FORMAT_R32_UINT: return                    "VK_FORMAT_R32_UINT";
    case VK_FORMAT_R32_SINT: return                    "VK_FORMAT_R32_SINT";
    case VK_FORMAT_R32_SFLOAT: return                  "VK_FORMAT_R32_SFLOAT";
    case VK_FORMAT_R32G32_UINT: return                 "VK_FORMAT_R32G32_UINT";
    case VK_FORMAT_R32G32_SINT: return                 "VK_FORMAT_R32G32_SINT";
    case VK_FORMAT_R32G32_SFLOAT: return               "VK_FORMAT_R32G32_SFLOAT";
    case VK_FORMAT_R32G32B32_UINT: return              "VK_FORMAT_R32G32B32_UINT";
    case VK_FORMAT_R32G32B32_SINT: return              "VK_FORMAT_R32G32B32_SINT";
    case VK_FORMAT_R32G32B32_SFLOAT: return            "VK_FORMAT_R32G32B32_SFLOAT";
    case VK_FORMAT_R32G32B32A32_UINT: return           "VK_FORMAT_R32G32B32A32_UINT";
    case VK_FORMAT_R32G32B32A32_SINT: return           "VK_FORMAT_R32G32B32A32_SINT";
    case VK_FORMAT_R32G32B32A32_SFLOAT: return         "VK_FORMAT_R32G32B32A32_SFLOAT";
    case VK_FORMAT_R64_UINT: return                    "VK_FORMAT_R64_UINT";
    case VK_FORMAT_R64_SINT: return                    "VK_FORMAT_R64_SINT";
    case VK_FORMAT_R64_SFLOAT: return                  "VK_FORMAT_R64_SFLOAT ";
    case VK_FORMAT_R64G64_UINT: return                 "VK_FORMAT_R64G64_UINT";
    case VK_FORMAT_R64G64_SINT: return                 "VK_FORMAT_R64G64_SINT";
    case VK_FORMAT_R64G64_SFLOAT: return               "VK_FORMAT_R64G64_SFLOAT";
    case VK_FORMAT_R64G64B64_UINT: return              "VK_FORMAT_R64G64B64_UINT";
    case VK_FORMAT_R64G64B64_SINT: return              "VK_FORMAT_R64G64B64_SINT";
    case VK_FORMAT_R64G64B64_SFLOAT: return            "VK_FORMAT_R64G64B64_SFLOAT";
    case VK_FORMAT_R64G64B64A64_UINT: return           "VK_FORMAT_R64G64B64A64_UINT";
    case VK_FORMAT_R64G64B64A64_SINT: return           "VK_FORMAT_R64G64B64A64_SINT";
    case VK_FORMAT_R64G64B64A64_SFLOAT: return         "VK_FORMAT_R64G64B64A64_SFLOAT";
    case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return     "VK_FORMAT_B10G11R11_UFLOAT_PACK32";
    case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return      "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32";
    case VK_FORMAT_D16_UNORM: return                   "VK_FORMAT_D16_UNORM";
    case VK_FORMAT_X8_D24_UNORM_PACK32: return         "VK_FORMAT_X8_D24_UNORM_PACK32";
    case VK_FORMAT_D32_SFLOAT: return                  "VK_FORMAT_D32_SFLOAT";
    case VK_FORMAT_S8_UINT: return                     "VK_FORMAT_S8_UINT";
    case VK_FORMAT_D16_UNORM_S8_UINT: return           "VK_FORMAT_D16_UNORM_S8_UINT";
    case VK_FORMAT_D24_UNORM_S8_UINT: return           "VK_FORMAT_D24_UNORM_S8_UINT";
    case VK_FORMAT_D32_SFLOAT_S8_UINT: return          "VK_FORMAT_D32_SFLOAT_S8_UINT";
    case VK_FORMAT_BC1_RGB_UNORM_BLOCK: return         "VK_FORMAT_BC1_RGB_UNORM_BLOCK";
    case VK_FORMAT_BC1_RGB_SRGB_BLOCK: return          "VK_FORMAT_BC1_RGB_SRGB_BLOCK";
    case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: return        "VK_FORMAT_BC1_RGBA_UNORM_BLOCK";
    case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: return         "VK_FORMAT_BC1_RGBA_SRGB_BLOCK";
    case VK_FORMAT_BC2_UNORM_BLOCK: return             "VK_FORMAT_BC2_UNORM_BLOCK";
    case VK_FORMAT_BC2_SRGB_BLOCK: return              "VK_FORMAT_BC2_SRGB_BLOCK";
    case VK_FORMAT_BC3_UNORM_BLOCK: return             "VK_FORMAT_BC3_UNORM_BLOCK";
    case VK_FORMAT_BC3_SRGB_BLOCK: return              "VK_FORMAT_BC3_SRGB_BLOCK";
    case VK_FORMAT_BC4_UNORM_BLOCK: return             "VK_FORMAT_BC4_UNORM_BLOCK";
    case VK_FORMAT_BC4_SNORM_BLOCK: return             "VK_FORMAT_BC4_SNORM_BLOCK";
    case VK_FORMAT_BC5_UNORM_BLOCK: return             "VK_FORMAT_BC5_UNORM_BLOCK";
    case VK_FORMAT_BC5_SNORM_BLOCK: return             "VK_FORMAT_BC5_SNORM_BLOCK";
    case VK_FORMAT_BC6H_UFLOAT_BLOCK: return           "VK_FORMAT_BC6H_UFLOAT_BLOCK";
    case VK_FORMAT_BC6H_SFLOAT_BLOCK: return           "VK_FORMAT_BC6H_SFLOAT_BLOCK";
    case VK_FORMAT_BC7_UNORM_BLOCK: return             "VK_FORMAT_BC7_UNORM_BLOCK";
    case VK_FORMAT_BC7_SRGB_BLOCK: return              "VK_FORMAT_BC7_SRGB_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: return     "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: return      "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: return   "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: return    "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: return   "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK";
    case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: return    "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK";
    case VK_FORMAT_EAC_R11_UNORM_BLOCK: return         "VK_FORMAT_EAC_R11_UNORM_BLOCK";
    case VK_FORMAT_EAC_R11_SNORM_BLOCK: return         "VK_FORMAT_EAC_R11_SNORM_BLOCK";
    case VK_FORMAT_EAC_R11G11_UNORM_BLOCK: return      "VK_FORMAT_EAC_R11G11_UNORM_BLOCK";
    case VK_FORMAT_EAC_R11G11_SNORM_BLOCK: return      "VK_FORMAT_EAC_R11G11_SNORM_BLOCK";
    case VK_FORMAT_ASTC_4x4_UNORM_BLOCK: return        "VK_FORMAT_ASTC_4x4_UNORM_BLOCK";
    case VK_FORMAT_ASTC_4x4_SRGB_BLOCK: return         "VK_FORMAT_ASTC_4x4_SRGB_BLOCK";
    case VK_FORMAT_ASTC_5x4_UNORM_BLOCK: return        "VK_FORMAT_ASTC_5x4_UNORM_BLOCK";
    case VK_FORMAT_ASTC_5x4_SRGB_BLOCK: return         "VK_FORMAT_ASTC_5x4_SRGB_BLOCK";
    case VK_FORMAT_ASTC_5x5_UNORM_BLOCK: return        "VK_FORMAT_ASTC_5x5_UNORM_BLOCK";
    case VK_FORMAT_ASTC_5x5_SRGB_BLOCK: return         "VK_FORMAT_ASTC_5x5_SRGB_BLOCK";
    case VK_FORMAT_ASTC_6x5_UNORM_BLOCK: return        "VK_FORMAT_ASTC_6x5_UNORM_BLOCK";
    case VK_FORMAT_ASTC_6x5_SRGB_BLOCK: return         "VK_FORMAT_ASTC_6x5_SRGB_BLOCK";
    case VK_FORMAT_ASTC_6x6_UNORM_BLOCK: return        "VK_FORMAT_ASTC_6x6_UNORM_BLOCK";
    case VK_FORMAT_ASTC_6x6_SRGB_BLOCK: return         "VK_FORMAT_ASTC_6x6_SRGB_BLOCK";
    case VK_FORMAT_ASTC_8x5_UNORM_BLOCK: return        "VK_FORMAT_ASTC_8x5_UNORM_BLOCK";
    case VK_FORMAT_ASTC_8x5_SRGB_BLOCK: return         "VK_FORMAT_ASTC_8x5_SRGB_BLOCK";
    case VK_FORMAT_ASTC_8x6_UNORM_BLOCK: return        "VK_FORMAT_ASTC_8x6_UNORM_BLOCK";
    case VK_FORMAT_ASTC_8x6_SRGB_BLOCK: return         "VK_FORMAT_ASTC_8x6_SRGB_BLOCK";
    case VK_FORMAT_ASTC_8x8_UNORM_BLOCK: return        "VK_FORMAT_ASTC_8x8_UNORM_BLOCK";
    case VK_FORMAT_ASTC_8x8_SRGB_BLOCK: return         "VK_FORMAT_ASTC_8x8_SRGB_BLOCK";
    case VK_FORMAT_ASTC_10x5_UNORM_BLOCK: return       "VK_FORMAT_ASTC_10x5_UNORM_BLOCK";
    case VK_FORMAT_ASTC_10x5_SRGB_BLOCK: return        "VK_FORMAT_ASTC_10x5_SRGB_BLOCK";
    case VK_FORMAT_ASTC_10x6_UNORM_BLOCK: return       "VK_FORMAT_ASTC_10x6_UNORM_BLOCK";
    case VK_FORMAT_ASTC_10x6_SRGB_BLOCK: return        "VK_FORMAT_ASTC_10x6_SRGB_BLOCK";
    case VK_FORMAT_ASTC_10x8_UNORM_BLOCK: return       "VK_FORMAT_ASTC_10x8_UNORM_BLOCK ";
    case VK_FORMAT_ASTC_10x8_SRGB_BLOCK: return        "VK_FORMAT_ASTC_10x8_SRGB_BLOCK";
    case VK_FORMAT_ASTC_10x10_UNORM_BLOCK: return      "VK_FORMAT_ASTC_10x10_UNORM_BLOCK";
    case VK_FORMAT_ASTC_10x10_SRGB_BLOCK: return       "VK_FORMAT_ASTC_10x10_SRGB_BLOCK";
    case VK_FORMAT_ASTC_12x10_UNORM_BLOCK: return      "VK_FORMAT_ASTC_12x10_UNORM_BLOCK";
    case VK_FORMAT_ASTC_12x10_SRGB_BLOCK: return       "VK_FORMAT_ASTC_12x10_SRGB_BLOCK";
    case VK_FORMAT_ASTC_12x12_UNORM_BLOCK: return      "VK_FORMAT_ASTC_12x12_UNORM_BLOCK";
    case VK_FORMAT_ASTC_12x12_SRGB_BLOCK: return       "VK_FORMAT_ASTC_12x12_SRGB_BLOCK";
    case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG";
    case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG";
    case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG";
    case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG";
    case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG: return  "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG";
    case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG: return  "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG";
    case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG: return  "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG";
    case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG: return  "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG";
    }
}

std::vector<const char*> enumToStr(VkFormatFeatureFlags enumVal) {
    std::vector<const char*> retVal;

    if (enumVal & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) { retVal.push_back("VK_IMAGE_USAGE_TRANSFER_SRC_BIT"); }
    if (enumVal & VK_IMAGE_USAGE_TRANSFER_DST_BIT) { retVal.push_back("VK_IMAGE_USAGE_TRANSFER_DST_BIT"); }
    if (enumVal & VK_IMAGE_USAGE_SAMPLED_BIT) { retVal.push_back("VK_IMAGE_USAGE_SAMPLED_BIT"); }
    if (enumVal & VK_IMAGE_USAGE_STORAGE_BIT) { retVal.push_back("VK_IMAGE_USAGE_STORAGE_BIT"); }
    if (enumVal & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) { retVal.push_back("VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT"); }
    if (enumVal & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) { retVal.push_back("VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT"); }
    if (enumVal & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) { retVal.push_back("VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT"); }
    if (enumVal & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) { retVal.push_back("VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT"); }

    return retVal;
}

} // Vulkan
} // Graphics
} // lug
