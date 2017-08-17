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
    VkDeviceSize getDeviceMemoryOffset() const;
    VkFormat getFormat() const;

    /**
     * @brief      Return first format supported by given features and tiling
     *
     * @param[in]  device    The device
     * @param[in]  formats   The formats to iterate over
     * @param[in]  tiling    The tiling that must be supported
     * @param[in]  features  The features that must be supported
     *
     * @return     The format that supports both features and tiling given in parameter
     */
    static VkFormat findSupportedFormat(const Device& device, const std::set<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features);

private:
    /**
     * @brief      Private ctor
     *
     * @param[in]  Image           The underlying image
     * @param[in]  device          The device
     * @param[in]  extent          The extent
     * @param[in]  format          The format
     * @param[in]  swapchainImage  True if the swapchain has the ownership on the underlying VkImage type
     */
    explicit Image(VkImage Image, const Device* device, const Extent& extent, VkFormat format, bool swapchainImage);

    /**
     * @brief      Return true if the format uis supported with the given features and tiling
     *
     * @param[in]  device    The device
     * @param[in]  format    The format
     * @param[in]  tiling    The tiling that must be supported
     * @param[in]  features  The features that must be supported
     *
     * @return     Wether the format given in parameter supports both features and tiling given in parameter
     */
    static bool isFormatSupported(const Device& device, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features);

private:
    VkImage _image{VK_NULL_HANDLE};
    const Device* _device{nullptr};

    const DeviceMemory* _deviceMemory{nullptr};
    VkDeviceSize _deviceMemoryOffset{0};

    Extent _extent{};
    VkFormat _format{};
    VkImageAspectFlags _aspect{};
    bool _swapchainImage{};

    VkMemoryRequirements _requirements{};
};

#include <lug/Graphics/Vulkan/API/Image.inl>

} // API
} // Vulkan
} // Graphics
} // lug
