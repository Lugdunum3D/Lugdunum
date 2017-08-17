#include <lug/Graphics/Vulkan/API/Image.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

Image::Image(VkImage image, const Device* device, const Extent& extent, VkFormat format, bool swapchainImage) :
    _image(image), _device(device), _extent(extent), _format(format), _swapchainImage(swapchainImage) {
    if (_image != VK_NULL_HANDLE) {
        vkGetImageMemoryRequirements(static_cast<VkDevice>(*device), _image, &_requirements);
    }
}

Image::Image(Image&& image) {
    _image = image._image;
    _device = image._device;
    _swapchainImage = image._swapchainImage;
    _extent = image._extent;
    _requirements = image._requirements;
    _deviceMemory = image._deviceMemory;
    _deviceMemoryOffset = image._deviceMemoryOffset;
    _format = image._format;

    image._image = VK_NULL_HANDLE;
    image._device = nullptr;
    image._extent = {0, 0};
    image._requirements = {};
    image._deviceMemory = nullptr;
    image._deviceMemoryOffset = 0;
    image._format = VK_FORMAT_UNDEFINED;
}

Image& Image::operator=(Image&& image) {
    destroy();

    _image = image._image;
    _device = image._device;
    _swapchainImage = image._swapchainImage;
    _extent = image._extent;
    _requirements = image._requirements;
    _deviceMemory = image._deviceMemory;
    _deviceMemoryOffset = image._deviceMemoryOffset;
    _format = image._format;

    image._image = VK_NULL_HANDLE;
    image._device = nullptr;
    image._extent = {0, 0};
    image._requirements = {};
    image._deviceMemory = nullptr;
    image._deviceMemoryOffset = 0;
    image._format = VK_FORMAT_UNDEFINED;

    return *this;
}

Image::~Image() {
    // Swapchain images are automatically destroyed when destroying swapchain
    if (!_swapchainImage) {
        destroy();
    }
}

void Image::destroy() {
    if (_image != VK_NULL_HANDLE) {
        vkDestroyImage(static_cast<VkDevice>(*_device), _image, nullptr);
        _image = VK_NULL_HANDLE;
    }
}

void Image::bindMemory(const DeviceMemory& deviceMemory, VkDeviceSize memoryOffset) {
    _deviceMemory = &deviceMemory;
    _deviceMemoryOffset = memoryOffset;

    vkBindImageMemory(static_cast<VkDevice>(*_device), _image, static_cast<VkDeviceMemory>(deviceMemory), memoryOffset);
}

VkFormat Image::findSupportedFormat(const Device& device, const std::set<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (auto format: formats) {
        if (isFormatSupported(device, format, tiling, features)) {
            return format;
        }
    }

    LUG_LOG.error("Image: Can't find supported format format given features");
    return VK_FORMAT_UNDEFINED;
}

bool Image::isFormatSupported(const Device& device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features) {
    const PhysicalDeviceInfo* physicalDeviceInfo = device.getPhysicalDeviceInfo();

    if (physicalDeviceInfo->formatProperties.find(format) == physicalDeviceInfo->formatProperties.end()) {
        LUG_LOG.warn("Image::isFormatSupported: the format does not exists in physicalDeviceInfo->formatProperties");
        return false;
    }

    const VkFormatProperties& formatProperties = physicalDeviceInfo->formatProperties.at(format);

    if (tiling == VK_IMAGE_TILING_LINEAR && (formatProperties.linearTilingFeatures & features) == features) {
        return true;
    }
    if (tiling == VK_IMAGE_TILING_OPTIMAL && (formatProperties.optimalTilingFeatures & features) == features) {
        return true;
    }

    return false;
}

} // API
} // Vulkan
} // Graphics
} // lug
