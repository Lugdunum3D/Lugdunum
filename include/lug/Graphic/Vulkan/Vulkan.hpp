#pragma once

#include <vector>

#include <lug/Graphic/Export.hpp>

#if defined(LUG_SYSTEM_WINDOWS)
    #define VK_USE_PLATFORM_WIN32_KHR
#elif defined(LUG_SYSTEM_LINUX)
    #define VK_USE_PLATFORM_XLIB_KHR
#elif defined(LUG_SYSTEM_ANDROID)
    #define VK_USE_PLATFORM_ANDROID_KHR
#endif

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#define LUG_CORE_VULKAN_FUNCTIONS(macro)                \
    macro(vkGetInstanceProcAddr)                        \
    macro(vkGetDeviceProcAddr)                          \
    macro(vkEnumerateInstanceExtensionProperties)       \
    macro(vkEnumerateInstanceLayerProperties)           \
    macro(vkCreateInstance)

#if defined(LUG_SYSTEM_WINDOWS)
    #define LUG_INSTANCE_VULKAN_FUNCTIONS_PLATFORM(macro)   \
        macro(vkCreateWin32SurfaceKHR) // VK_KHR_win32_surface
#elif defined(LUG_SYSTEM_LINUX)
    #define LUG_INSTANCE_VULKAN_FUNCTIONS_PLATFORM(macro)   \
        macro(vkCreateXlibSurfaceKHR) // VK_KHR_xlib_surface
#elif defined(LUG_SYSTEM_ANDROID)
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
    macro(vkQueueSubmit)                                \
    macro(vkFreeCommandBuffers)                         \
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
    macro(vkCmdBindVertexBuffers)                       \
    macro(vkWaitForFences)                              \
    macro(vkResetFences)                                \
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
    macro(vkCmdBindDescriptorSets)                      \
    macro(vkDestroyDescriptorPool)                      \
    macro(vkDestroyDescriptorSetLayout)                 \
    macro(vkDestroySampler)                             \
    macro(vkDestroyImage)                               \
    LUG_DEVICE_VULKAN_FUNCTIONS_KHR_SWAPCHAIN(macro)

#define LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS(name) extern PFN_##name LUG_GRAPHIC_API name;
LUG_CORE_VULKAN_FUNCTIONS(LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS);
LUG_INSTANCE_VULKAN_FUNCTIONS(LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS);
LUG_DEVICE_VULKAN_FUNCTIONS(LUG_DEFINE_DECLARATION_VULKAN_FUNCTIONS);
#undef LUG_DEFINE_DECLARATION__VULKAN_FUNCTIONS

