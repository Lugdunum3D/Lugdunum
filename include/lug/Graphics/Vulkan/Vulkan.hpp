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

struct LUG_GRAPHICS_API Version {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;

    static Version fromInt(uint32_t version);
};

} // Vulkan
} // Graphics
} // lug
