#pragma once

#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/Image.hpp>
#include <lug/Graphics/Vulkan/ImageView.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Device;
class Queue;
class RenderPass;

class LUG_GRAPHICS_API Swapchain {
public:
    Swapchain(VkSwapchainKHR swapchain = VK_NULL_HANDLE, const Device* device = nullptr, const VkSurfaceFormatKHR& swapchainFormat = {}, const VkExtent2D& extent = {});

    Swapchain(const Swapchain&) = delete;
    Swapchain(Swapchain&& Swapchain);

    Swapchain& operator=(const Swapchain&) = delete;
    Swapchain& operator=(Swapchain&& Swapchain);

    ~Swapchain();

    operator VkSwapchainKHR() const {
        return _swapchain;
    }

    void destroy();

    bool getNextImage(uint32_t *imageIndex, VkSemaphore semaphore = VK_NULL_HANDLE);
    bool present(const Queue* presentQueue, uint32_t imageIndex, VkSemaphore semaphore = VK_NULL_HANDLE);
    bool init(RenderPass* renderPass);

    std::vector<Image>& getImages();
    const std::vector<Framebuffer>& getFramebuffers() const;

    const VkSurfaceFormatKHR& getFormat() const;
    const VkExtent2D& getExtent() const;

    void setOutOfDate(bool outOfDate);
    bool isOutOfDate() const;

private:
    bool initImages();
    bool initFramebuffers(RenderPass* renderPass);

private:
    VkSwapchainKHR _swapchain{VK_NULL_HANDLE};
    const Device* _device{nullptr};
    std::vector<Image> _images;
    std::vector<ImageView> _imagesViews;
    std::vector<Framebuffer> _framebuffers;
    VkSurfaceFormatKHR _format;
    VkExtent2D _extent;
    bool _outOfDate{false};
};

#include <lug/Graphics/Vulkan/Swapchain.inl>

} // Vulkan
} // Graphics
} // lug
