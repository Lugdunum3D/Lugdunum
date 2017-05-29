#pragma once

#include <memory>
#include <set>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class Image;
} // Builder

class Device;
class DeviceMemory;

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

    void destroy();

    void bindMemory(const DeviceMemory& deviceMemory, VkDeviceSize memoryOffset = 0);

    const VkMemoryRequirements& getRequirements() const;
    const DeviceMemory* getDeviceMemory() const;
    VkFormat getFormat() const;

    // Return first format supported by given features and tiling
    static VkFormat findSupportedFormat(const Device& device, const std::set<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features);

private:
    explicit Image(VkImage Image, const Device* device, const Extent& extent, VkFormat format, bool swapchainImage);

    static bool isFormatSupported(const Device& device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features);

private:
    VkImage _image{VK_NULL_HANDLE};
    const Device* _device{nullptr};

    const DeviceMemory* _deviceMemory{nullptr};
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
