#pragma once

#include <unordered_map>
#include <vector>
#include <ostream>

#include <lug/Graphics/Export.hpp>

#if defined(LUG_SYSTEM_WINDOWS)
    #define VK_USE_PLATFORM_WIN32_KHR
#elif defined(LUG_SYSTEM_LINUX)
    #define VK_USE_PLATFORM_XLIB_KHR
#elif defined(LUG_SYSTEM_ANDROID)
    #define VK_USE_PLATFORM_ANDROID_KHR
#endif

#define VK_NO_PROTOTYPES
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <vulkan/vulkan.h>

#if defined(LUG_SYSTEM_LINUX)
    // Fuck you X11
    #undef None
#endif

#define LUG_EXPORTED_VULKAN_FUNCTIONS(macro)    \
    macro(vkGetInstanceProcAddr)

#define LUG_CORE_VULKAN_FUNCTIONS(macro)                \
    macro(vkEnumerateInstanceExtensionProperties)       \
    macro(vkEnumerateInstanceLayerProperties)           \
    macro(vkCreateInstance)

#if defined(LUG_SYSTEM_WINDOWS)
    #define LUG_VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    #define LUG_INSTANCE_VULKAN_FUNCTIONS_PLATFORM(macro)   \
        macro(vkCreateWin32SurfaceKHR) // VK_KHR_win32_surface
#elif defined(LUG_SYSTEM_LINUX)
    #define LUG_VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_XLIB_SURFACE_EXTENSION_NAME
    #define LUG_INSTANCE_VULKAN_FUNCTIONS_PLATFORM(macro)   \
        macro(vkCreateXlibSurfaceKHR) // VK_KHR_xlib_surface
#elif defined(LUG_SYSTEM_ANDROID)
    #define LUG_VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
    #define LUG_INSTANCE_VULKAN_FUNCTIONS_PLATFORM(macro)   \
        macro(vkCreateAndroidSurfaceKHR) // VK_KHR_android_surface
#endif

/* VK_KHR_surface */
#define LUG_INSTANCE_VULKAN_FUNCTIONS_KHR_SURFACE(macro)    \
    macro(vkGetPhysicalDeviceSurfaceSupportKHR)             \
    macro(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)        \
    macro(vkGetPhysicalDeviceSurfaceFormatsKHR)             \
    macro(vkGetPhysicalDeviceSurfacePresentModesKHR)        \
    macro(vkDestroySurfaceKHR)

#define LUG_INSTANCE_VULKAN_FUNCTIONS(macro)            \
    macro(vkEnumeratePhysicalDevices)                   \
    macro(vkGetPhysicalDeviceProperties)                \
    macro(vkGetPhysicalDeviceFeatures)                  \
    macro(vkGetPhysicalDeviceQueueFamilyProperties)     \
    macro(vkGetPhysicalDeviceFormatProperties)          \
    macro(vkGetDeviceProcAddr)                          \
    macro(vkCreateDevice)                               \
    macro(vkDestroyInstance)                            \
    macro(vkEnumerateDeviceExtensionProperties)         \
    macro(vkGetPhysicalDeviceMemoryProperties)          \
    LUG_INSTANCE_VULKAN_FUNCTIONS_PLATFORM(macro)       \
    LUG_INSTANCE_VULKAN_FUNCTIONS_KHR_SURFACE(macro)

#define LUG_DEVICE_VULKAN_FUNCTIONS_KHR_SWAPCHAIN(macro)    \
    macro(vkCreateSwapchainKHR)                             \
    macro(vkGetSwapchainImagesKHR)                          \
    macro(vkAcquireNextImageKHR)                            \
    macro(vkQueuePresentKHR)                                \
    macro(vkDestroySwapchainKHR)                            \

#define LUG_DEVICE_VULKAN_FUNCTIONS(macro)              \
    macro(vkQueueWaitIdle)                              \
    macro(vkGetDeviceQueue)                             \
    macro(vkDeviceWaitIdle)                             \
    macro(vkDestroyDevice)                              \
    macro(vkCreateSemaphore)                            \
    macro(vkCreateCommandPool)                          \
    macro(vkAllocateCommandBuffers)                     \
    macro(vkBeginCommandBuffer)                         \
    macro(vkCmdPipelineBarrier)                         \
    macro(vkCmdClearColorImage)                         \
    macro(vkEndCommandBuffer)                           \
    macro(vkResetCommandBuffer)                         \
    macro(vkQueueSubmit)                                \
    macro(vkFreeCommandBuffers)                         \
    macro(vkResetCommandPool)                           \
    macro(vkDestroyCommandPool)                         \
    macro(vkDestroySemaphore)                           \
    macro(vkCreateImageView)                            \
    macro(vkCreateRenderPass)                           \
    macro(vkCreateFramebuffer)                          \
    macro(vkCreateShaderModule)                         \
    macro(vkCreatePipelineLayout)                       \
    macro(vkCreateGraphicsPipelines)                    \
    macro(vkCmdBeginRenderPass)                         \
    macro(vkCmdBindPipeline)                            \
    macro(vkCmdDraw)                                    \
    macro(vkCmdDrawIndexed)                             \
    macro(vkCmdEndRenderPass)                           \
    macro(vkDestroyShaderModule)                        \
    macro(vkDestroyPipelineLayout)                      \
    macro(vkDestroyPipeline)                            \
    macro(vkDestroyRenderPass)                          \
    macro(vkDestroyFramebuffer)                         \
    macro(vkDestroyImageView)                           \
    macro(vkCreateFence)                                \
    macro(vkCreateBuffer)                               \
    macro(vkGetBufferMemoryRequirements)                \
    macro(vkAllocateMemory)                             \
    macro(vkBindBufferMemory)                           \
    macro(vkMapMemory)                                  \
    macro(vkFlushMappedMemoryRanges)                    \
    macro(vkUnmapMemory)                                \
    macro(vkCmdSetViewport)                             \
    macro(vkCmdSetScissor)                              \
    macro(vkCmdSetBlendConstants)                       \
    macro(vkCmdBindVertexBuffers)                       \
    macro(vkCmdBindIndexBuffer)                         \
    macro(vkWaitForFences)                              \
    macro(vkResetFences)                                \
    macro(vkGetFenceStatus)                             \
    macro(vkFreeMemory)                                 \
    macro(vkDestroyBuffer)                              \
    macro(vkDestroyFence)                               \
    macro(vkCmdCopyBuffer)                              \
    macro(vkCreateImage)                                \
    macro(vkGetImageMemoryRequirements)                 \
    macro(vkBindImageMemory)                            \
    macro(vkCreateSampler)                              \
    macro(vkCmdCopyBufferToImage)                       \
    macro(vkCreateDescriptorSetLayout)                  \
    macro(vkCreateDescriptorPool)                       \
    macro(vkAllocateDescriptorSets)                     \
    macro(vkUpdateDescriptorSets)                       \
    macro(vkCmdUpdateBuffer)                            \
    macro(vkCmdBindDescriptorSets)                      \
    macro(vkDestroyDescriptorPool)                      \
    macro(vkDestroyDescriptorSetLayout)                 \
    macro(vkDestroySampler)                             \
    macro(vkDestroyImage)                               \
    macro(vkCmdPushConstants)                           \
    LUG_DEVICE_VULKAN_FUNCTIONS_KHR_SWAPCHAIN(macro)

inline namespace Vulkan {

#define LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS(name) extern PFN_##name LUG_GRAPHICS_API name;
LUG_EXPORTED_VULKAN_FUNCTIONS(LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS);
LUG_CORE_VULKAN_FUNCTIONS(LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS);
LUG_INSTANCE_VULKAN_FUNCTIONS(LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS);
LUG_DEVICE_VULKAN_FUNCTIONS(LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS);
#undef LUG_DEFINE_DECLARATION__VULKAN_FUNCTIONS

} // Vulkan

namespace lug {
namespace Graphics {
namespace Vulkan {

struct InstanceInfo {
    std::vector<VkExtensionProperties> extensions;
    std::vector<VkLayerProperties> layers;

    bool containsExtension(const char* extensionName) const;
    bool containsLayer(const char* layerName) const;
};

struct PhysicalDeviceInfo {
    VkPhysicalDevice handle;

    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;

    VkPhysicalDeviceMemoryProperties memoryProperties;

    std::vector<VkQueueFamilyProperties> queueFamilies;

    std::vector<VkExtensionProperties> extensions;

    std::unordered_map<VkFormat, VkFormatProperties> formatProperties;

    struct Swapchain {
        VkSurfaceCapabilitiesKHR capabilities;

        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    } swapchain;

    bool containsExtension(const char* extensionName) const;
    bool containsQueueFlags(VkQueueFlags queueFlags, int8_t& idx) const;
};

} // Vulkan
} // Graphics
} // lug

/**
 * PSA: This is outside of the namespace because VkResult is in the root namespace
 */
inline std::ostream& operator<<(std::ostream& ss, VkResult result) {
    switch (result) {
        case VK_SUCCESS:
            ss << "Success";
            break;
        case VK_NOT_READY:
            ss << "A fence or query has not yet completed";
            break;
        case VK_TIMEOUT:
            ss << "A wait operation has not completed in the specified time";
            break;
        case VK_EVENT_SET:
            ss << "An event is signaled";
            break;
        case VK_EVENT_RESET:
            ss << "An event is unsignaled";
            break;
        case VK_INCOMPLETE:
            ss << "A return array was too small for the result";
            break;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            ss << "A host memory allocation has failed";
            break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            ss << "A device memory allocation has failed";
            break;
        case VK_ERROR_INITIALIZATION_FAILED:
            ss << "Initialization of an object could not be completed for implementation-specific reasons";
            break;
        case VK_ERROR_DEVICE_LOST:
            ss << "The logical or physical device has been lost";
            break;
        case VK_ERROR_MEMORY_MAP_FAILED:
            ss << "Mapping of a memory object has failed";
            break;
        case VK_ERROR_LAYER_NOT_PRESENT:
            ss << "A requested layer is not present or could not be loaded";
            break;
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            ss << "A requested extension is not supported";
            break;
        case VK_ERROR_FEATURE_NOT_PRESENT:
            ss << "A requested feature is not supported";
            break;
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            ss << "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
            break;
        case VK_ERROR_TOO_MANY_OBJECTS:
            ss << "Too many objects of the type have already been created";
            break;
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            ss << "A requested format is not supported on this device";
            break;
        case VK_ERROR_SURFACE_LOST_KHR:
            ss << "A surface is no longer available";
            break;
        case VK_SUBOPTIMAL_KHR:
            ss << "A swapchain no longer matches the surface properties exactly, but can still be used";
            break;
        case VK_ERROR_OUT_OF_DATE_KHR:
            ss << "A surface has changed in such a way that it is no longer compatible with the swapchain";
            break;
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            ss << "The display used by a swapchain does not use the same presentable image layout";
            break;
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            ss << "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
            break;
        case VK_ERROR_VALIDATION_FAILED_EXT:
            ss << "A validation layer found an error";
            break;
        default:
            ss << "ERROR: UNKNOWN VULKAN ERROR";
            break;
    }

    return ss;
}
