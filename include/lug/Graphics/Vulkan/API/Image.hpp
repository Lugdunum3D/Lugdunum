#pragma once

#include <memory>
#include <set>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
    class Image;
}

class Device;

class LUG_GRAPHICS_API Image {
    friend class Builder::Image;
    friend class Swapchain;

public:
    // TODO: Declare offset and extent outside
    struct Extent {
        uint32_t width;
        uint32_t height;
    };

public:
    Image() = default;

    Image(const Image&) = delete;
    Image(Image&& Image);

    Image& operator=(const Image&) = delete;
    Image& operator=(Image&& Image);

    ~Image();

    explicit operator VkImage() const {
        return _image;
    }

    const Extent& getExtent() const {
        return _extent;
    }

    void changeLayout(CommandBuffer& commandBuffer,
                    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                    VkImageLayout oldLayout, VkImageLayout newLayout,
                    VkImageAspectFlags aspect,
                    VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    uint32_t srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                    uint32_t dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED) const;

    void destroy();

    void bindMemory(DeviceMemory* deviceMemory, VkDeviceSize memoryOffset = 0);

    const VkMemoryRequirements& getRequirements() const;
    VkFormat getFormat() const;

    // Return first format supported by given features and tiling
    static VkFormat findSupportedFormat(const Device* device, const std::set<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features);

private:
    explicit Image(VkImage Image, const Device* device, const Extent& extent, VkFormat format, bool swapchainImage);

    static bool isFormatSupported(const Device* device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features);

private:
    VkImage _image{VK_NULL_HANDLE};
    const Device* _device{nullptr};

    DeviceMemory* _deviceMemory{nullptr};
    VkDeviceSize _deviceMemoryOffset{0};

    Extent _extent;
    VkFormat _format;
    VkImageAspectFlags _aspect;
    bool _swapchainImage;

    VkMemoryRequirements _requirements{};
};

} // API
} // Vulkan
} // Graphics
} // lug
