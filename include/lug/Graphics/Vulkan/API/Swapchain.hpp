#pragma once

#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/Graphics/Vulkan/API/ImageView.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;
class Queue;
class RenderPass;

class LUG_GRAPHICS_API Swapchain {
public:
    explicit Swapchain(VkSwapchainKHR swapchain = VK_NULL_HANDLE, const Device* device = nullptr, const VkSurfaceFormatKHR& swapchainFormat = {}, const VkExtent2D& extent = {});

    Swapchain(const Swapchain&) = delete;
    Swapchain(Swapchain&& Swapchain);

    Swapchain& operator=(const Swapchain&) = delete;
    Swapchain& operator=(Swapchain&& Swapchain);

    ~Swapchain();

    explicit operator VkSwapchainKHR() const {
        return _swapchain;
    }

    void destroy();

    bool getNextImage(uint32_t *imageIndex, VkSemaphore semaphore = VK_NULL_HANDLE);
    bool present(const Queue* presentQueue, uint32_t imageIndex, VkSemaphore semaphore = VK_NULL_HANDLE);

    std::vector<Image>& getImages();
    const std::vector<std::unique_ptr<ImageView>>& getImagesViews() const;

    const VkSurfaceFormatKHR& getFormat() const;
    const VkExtent2D& getExtent() const;

    void setOutOfDate(bool outOfDate);
    bool isOutOfDate() const;
    bool init();

private:
    VkSwapchainKHR _swapchain{VK_NULL_HANDLE};
    const Device* _device{nullptr};
    std::vector<Image> _images;
    std::vector<std::unique_ptr<ImageView>> _imagesViews;
    VkSurfaceFormatKHR _format;
    VkExtent2D _extent;
    bool _outOfDate{false};
};

#include <lug/Graphics/Vulkan/API/Swapchain.inl>

} // API
} // Vulkan
} // Graphics
} // lug
