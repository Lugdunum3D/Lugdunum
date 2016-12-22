#pragma once

#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Swapchain {
public:
    explicit Swapchain(VkSwapchainKHR swapchain = VK_NULL_HANDLE, const Device* device = nullptr);

    Swapchain(const Swapchain&) = delete;
    Swapchain(Swapchain&& Swapchain);

    Swapchain& operator=(const Swapchain&) = delete;
    Swapchain& operator=(Swapchain&& Swapchain);

    ~Swapchain();

    operator VkSwapchainKHR() const {
        return _swapchain;
    }

    void destroy();

    bool initImages(const VkSurfaceFormatKHR& swapchainFormat);

private:
    VkSwapchainKHR _swapchain{ VK_NULL_HANDLE };
    const Device* _device{nullptr};
    std::vector<VkImage> _images;
    std::vector<VkImageView> _imagesViews;
};

} // Vulkan
} // Graphics
} // lug
