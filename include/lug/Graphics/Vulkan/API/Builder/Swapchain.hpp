#pragma once

#include <memory>
#include <set>

#include <lug/Graphics/Vulkan/API/Swapchain.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {

class Swapchain {
public:
    Swapchain(const API::Device& device);

    Swapchain(const Swapchain&&) = delete;
    Swapchain(Swapchain&&) = delete;

    Swapchain& operator=(const Swapchain&&) = delete;
    Swapchain& operator=(Swapchain&&) = delete;

    ~Swapchain() = default;

    // Setters
    void setPreferences(const Renderer::Preferences::Swapchain& preferences);
    void setSurface(VkSurfaceKHR surface);
    void setMinImageCount(uint32_t minImageCount);
    void setImageFormat(VkFormat imageFormat);
    void setImageColorSpace(VkColorSpaceKHR imageColorSpace);
    void setImageExtent(VkExtent2D imageExtent);
    void setImageUsage(VkImageUsageFlags imageUsage);
    void setExclusive(bool exclusive);
    void setQueueFamilyIndices(const std::set<uint32_t>& queueFamilyIndices);
    void setPreTransform(VkSurfaceTransformFlagBitsKHR preTransform);
    void setCompositeAlpha(VkCompositeAlphaFlagBitsKHR compositeAlpha);
    void setPresentMode(VkPresentModeKHR presentMode);
    void setClipped(VkBool32 clipped);
    void setOldSwapchain(VkSwapchainKHR oldSwapchain);

    // Build methods
    bool build(API::Swapchain& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::Swapchain> build(VkResult* returnResult = nullptr);

private:
    bool setFromPreferences();

private:
    const API::Device& _device;

    const Renderer::Preferences::Swapchain* _preferences{nullptr};
    VkSurfaceKHR _surface{VK_NULL_HANDLE};
    uint32_t _minImageCount;
    VkFormat _imageFormat{VK_FORMAT_MAX_ENUM};
    VkColorSpaceKHR _imageColorSpace{VK_COLOR_SPACE_MAX_ENUM_KHR};
    VkExtent2D _imageExtent{};
    VkImageUsageFlags _imageUsage{VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT};
    bool _exclusive{false};
    std::set<uint32_t> _queueFamilyIndices;
    VkSurfaceTransformFlagBitsKHR _preTransform{VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR};
    VkCompositeAlphaFlagBitsKHR _compositeAlpha{VK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR};
    VkPresentModeKHR _presentMode{VK_PRESENT_MODE_MAX_ENUM_KHR};
    VkBool32 _clipped{VK_TRUE};
    VkSwapchainKHR _oldSwapchain{VK_NULL_HANDLE};
};

#include <lug/Graphics/Vulkan/API/Builder/Swapchain.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
