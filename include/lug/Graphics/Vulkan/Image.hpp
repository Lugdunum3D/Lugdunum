#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Device;

class LUG_GRAPHICS_API Image {
public:
    // TODO: Declare offset and extent outside
    struct Extent {
        uint32_t width;
        uint32_t height;
    };

public:
    explicit Image(VkImage Image = VK_NULL_HANDLE, const Device* device = nullptr, const Extent& extent = {0, 0}, bool swapchainImage = false, VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT);

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
                    VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                    uint32_t srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
                    uint32_t dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED);

    void destroy();

    void bindMemory(DeviceMemory* deviceMemory, VkDeviceSize memoryOffset = 0);

    const VkMemoryRequirements& getRequirements() const;

    static std::unique_ptr<Image> create(
        const Device* device,
        VkFormat format,
        VkExtent3D extent,
        VkImageUsageFlags usage,
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        uint32_t mipLevels = 1,
        uint32_t arrayLayers = 1,
        VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT,
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
        VkImageCreateFlags createFlag = 0,
        VkImageType imageType = VK_IMAGE_TYPE_2D
    );

    // Return first format supported by given features and tiling
    static VkFormat findSupportedFormat(const Device* device, const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features);

private:
    static bool isFormatSupported(const Device* device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features);

private:
    VkImage _image{VK_NULL_HANDLE};
    const Device* _device{nullptr};
    DeviceMemory* _deviceMemory{nullptr};
    bool _swapchainImage;

    VkImageAspectFlags _aspect;
    Extent _extent;
    VkMemoryRequirements _requirements{};
};

} // Vulkan
} // Graphics
} // lug
