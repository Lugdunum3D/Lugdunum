#include <lug/Graphics/Vulkan/Image.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Image::Image(VkImage image, const Device* device, const Extent& extent, bool swapchainImage, VkImageAspectFlags aspect) :
            _image(image), _device(device), _swapchainImage(swapchainImage), _aspect(aspect), _extent(extent) {
                if (_image != VK_NULL_HANDLE) {
                    vkGetImageMemoryRequirements(*device, _image, &_requirements);
                }
            }

Image::Image(Image&& image) {
    _image = image._image;
    _device = image._device;
    _swapchainImage = image._swapchainImage;
    _aspect = image._aspect;
    _extent = image._extent;
    _requirements = image._requirements;
    _deviceMemory = image._deviceMemory;
    image._image = VK_NULL_HANDLE;
    image._device = nullptr;
    image._extent = {0, 0};
    image._requirements = {};
    image._deviceMemory = nullptr;
}

Image& Image::operator=(Image&& image) {
    destroy();

    _image = image._image;
    _device = image._device;
    _swapchainImage = image._swapchainImage;
    _aspect = image._aspect;
    _extent = image._extent;
    _requirements = image._requirements;
    _deviceMemory = image._deviceMemory;
    image._image = VK_NULL_HANDLE;
    image._device = nullptr;
    image._extent = {0, 0};
    image._requirements = {};
    image._deviceMemory = nullptr;

    return *this;
}

Image::~Image() {
    // Swapchain images are automatically destroyed when destroying swapchain
    if (!_swapchainImage) {
        destroy();
    }
}

void Image::changeLayout(CommandBuffer& commandBuffer,
                VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                VkImageLayout oldLayout, VkImageLayout newLayout,
                VkPipelineStageFlags srcStageMask,
                VkPipelineStageFlags dstStageMask,
                uint32_t srcQueueFamilyIndex,
                uint32_t dstQueueFamilyIndex) {

    VkImageMemoryBarrier imageBarrier{
        imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        imageBarrier.pNext = nullptr,
        imageBarrier.srcAccessMask = srcAccessMask,
        imageBarrier.dstAccessMask = dstAccessMask,
        imageBarrier.oldLayout = oldLayout,
        imageBarrier.newLayout = newLayout,
        imageBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex,
        imageBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex,
        imageBarrier.image = _image,
        {} // imageBarrier.subresourceRange
    };

    imageBarrier.subresourceRange.aspectMask = _aspect;
    imageBarrier.subresourceRange.baseMipLevel = 0;
    imageBarrier.subresourceRange.levelCount = 1;
    imageBarrier.subresourceRange.baseArrayLayer = 0;
    imageBarrier.subresourceRange.layerCount = 1;

    vkCmdPipelineBarrier(commandBuffer, // commandBuffer
                        srcStageMask, // srcStageMask
                        dstStageMask, // dstStageMask
                        VK_DEPENDENCY_BY_REGION_BIT, // dependencyFlags
                        0, // memoryBarrierCount
                        nullptr, // pMemoryBarriers
                        0, // bufferMemoryBarrierCount
                        nullptr, // pBufferMemoryBarriers
                        1, // imageMemoryBarrierCount
                        &imageBarrier); // pImageMemoryBarriers
}

void Image::destroy() {
    if (_image != VK_NULL_HANDLE) {
        vkDestroyImage(*_device, _image, nullptr);
        _image = VK_NULL_HANDLE;
    }
}

void Image::bindMemory(DeviceMemory* deviceMemory, VkDeviceSize memoryOffset) {
    _deviceMemory = deviceMemory;
    vkBindImageMemory(*_device, _image, *deviceMemory, memoryOffset);
}

const VkMemoryRequirements& Image::getRequirements() const {
    return _requirements;
}

std::unique_ptr<Image> Image::create(
    const Device* device,
    VkFormat format,
    VkExtent3D extent,
    VkImageUsageFlags usage,
    VkSharingMode sharingMode,
    uint32_t mipLevels,
    uint32_t arrayLayers,
    VkSampleCountFlagBits samples,
    VkImageTiling tiling,
    VkImageCreateFlags createFlag,
    VkImageType imageType
) {
    VkImageCreateInfo createInfo {
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = createFlag,
        createInfo.imageType = imageType,
        createInfo.format = format,
        {}, // createInfo.extent
        createInfo.mipLevels = mipLevels,
        createInfo.arrayLayers = arrayLayers,
        createInfo.samples = samples,
        createInfo.tiling = tiling,
        createInfo.usage = usage,
        createInfo.sharingMode = sharingMode,
        createInfo.queueFamilyIndexCount = 0,
        createInfo.pQueueFamilyIndices = nullptr,
        createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
    };
    createInfo.extent.width = extent.width;
    createInfo.extent.height = extent.height;
    createInfo.extent.depth = extent.depth;

    VkImage imageHandle = VK_NULL_HANDLE;
    VkResult result = vkCreateImage(*device, &createInfo, nullptr, &imageHandle);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create image: {}", result);
        return nullptr;
    }

    return std::unique_ptr<Image>(new Image(imageHandle, device, {extent.width, extent.height}));
}

VkFormat Image::findSupportedFormat(const Device* device, const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (auto format: formats) {
        if (!isFormatSupported(device, format, tiling, features)) {
            return format;
        }
    }

    LUG_LOG.error("Image: Can't find supported format format given features");
    return VK_FORMAT_UNDEFINED;
}

bool Image::isFormatSupported(const Device* device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features) {
    const PhysicalDeviceInfo* physicalDeviceInfo = device->getPhysicalDeviceInfo();

    if (physicalDeviceInfo->formatProperties.find(format) == physicalDeviceInfo->formatProperties.end()) {
        LUG_LOG.warn("Image::isFormatSupported: the format does not exists in physicalDeviceInfo->formatProperties");
        return false;
    }

    const VkFormatProperties& formatProperties = physicalDeviceInfo->formatProperties.at(format);

    if (tiling == VK_IMAGE_TILING_LINEAR && (formatProperties.linearTilingFeatures & features) == features) {
        return true;
    }
    else if (tiling == VK_IMAGE_TILING_OPTIMAL && (formatProperties.optimalTilingFeatures & features) == features) {
        return false;
    }

    return true;
}

} // Vulkan
} // Graphics
} // lug
