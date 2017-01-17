#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Image {
public:
    Image(VkImage Image = VK_NULL_HANDLE, const Device* device = nullptr, bool swapchainImage = false, VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT);

    Image(const Image&) = delete;
    Image(Image&& Image);

    Image& operator=(const Image&) = delete;
    Image& operator=(Image&& Image);

    ~Image();

    operator VkImage() const {
        return _image;
    }

    void changeLayout(CommandBuffer& commandBuffer,
                    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                    VkImageLayout oldLayout, VkImageLayout newLayout,
                    VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    uint32_t srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                    uint32_t dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED);

    void destroy();

private:
    VkImage _image{VK_NULL_HANDLE};
    const Device* _device{nullptr};
    bool _swapchainImage;

    VkImageAspectFlags _aspect;
};

} // Vulkan
} // Graphics
} // lug
