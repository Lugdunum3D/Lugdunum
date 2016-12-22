#pragma once

#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Image.hpp>
#include <lug/Graphics/Vulkan/ImageView.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Swapchain {
public:
    Swapchain(VkSwapchainKHR swapchain = VK_NULL_HANDLE, const Device* device = nullptr, const VkSurfaceFormatKHR& swapchainFormat = {});

    Swapchain(const Swapchain&) = delete;
    Swapchain(Swapchain&& Swapchain);

    Swapchain& operator=(const Swapchain&) = delete;
    Swapchain& operator=(Swapchain&& Swapchain);

    ~Swapchain();

    operator VkSwapchainKHR() const {
        return _swapchain;
    }

    void destroy();

    bool initImages();

private:
    VkSwapchainKHR _swapchain{ VK_NULL_HANDLE };
    const Device* _device{nullptr};
    std::vector<Image> _images;
    std::vector<ImageView> _imagesViews;
    VkSurfaceFormatKHR _format;
};

} // Vulkan
} // Graphics
} // lug
