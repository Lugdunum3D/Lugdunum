#pragma once

#include <set>

#include <lug/Graphics/Vulkan/API/Image.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {

class Image {
public:
    Image(const API::Device& device);

    Image(const Image&&) = delete;
    Image(Image&&) = delete;

    Image& operator=(const Image&&) = delete;
    Image& operator=(Image&&) = delete;

    ~Image() = default;

    // Setters
    void setCreateFlags(VkImageCreateFlags createFlags);
    void setImageType(VkImageType imageType);
    void setPreferedFormats(const std::set<VkFormat>& preferedFormats);
    void setFeatureFlags(VkFormatFeatureFlags featureFlags);
    void setExtent(VkExtent3D extent);
    void setMipLevels(uint32_t mipLevels);
    void setArrayLayers(uint32_t arrayLayers);
    void setSampleCount(VkSampleCountFlagBits sampleCount);
    void setTiling(VkImageTiling tiling);
    void setUsage(VkImageUsageFlags usage);
    void setExclusive(bool exclusive);
    void setQueueFamilyIndices(const std::set<uint32_t>& queueFamilyIndices);

    // Build methods
    bool build(API::Image& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::Image> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    VkImageCreateFlags _createFlags{0};
    VkImageType _imageType{VK_IMAGE_TYPE_2D};
    std::set<VkFormat> _preferedFormats;
    VkFormatFeatureFlags _featureFlags{0};
    VkExtent3D _extent{};
    uint32_t _mipLevels{1};
    uint32_t _arrayLayers{1};
    VkSampleCountFlagBits _sampleCount{VK_SAMPLE_COUNT_1_BIT};
    VkImageTiling _tiling{VK_IMAGE_TILING_OPTIMAL};
    VkImageUsageFlags _usage{VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT};
    bool _exclusive{false};
    std::set<uint32_t> _queueFamilyIndices;
    VkImageLayout _initialLayout{VK_IMAGE_LAYOUT_UNDEFINED};
};

#include <lug/Graphics/Vulkan/API/Builder/Image.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
