#pragma once

#include <cstdint>
#include <memory>

#include <lug/Graphics/Vulkan/API/ImageView.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;
class Image;

namespace Builder {

class ImageView {
public:
    ImageView(const API::Device& device, const API::Image& image);

    ImageView(const ImageView&) = delete;
    ImageView(ImageView&&) = delete;

    ImageView& operator=(const ImageView&) = delete;
    ImageView& operator=(ImageView&&) = delete;

    ~ImageView() = default;

    // Setters
    void setFormat(VkFormat format);
    void setViewType(VkImageViewType viewType);
    void setAspectFlags(VkImageAspectFlags aspectFlags);
    void setLayerCount(uint32_t layerCount);
    void setLevelCount(uint32_t levelCount);

    // Build methods
    bool build(API::ImageView& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::ImageView> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;

    const API::Image& _image;
    VkFormat _format{VK_FORMAT_UNDEFINED};
    VkImageViewType _viewType{VK_IMAGE_VIEW_TYPE_2D};
    VkImageAspectFlags _aspectFlags{VK_IMAGE_ASPECT_COLOR_BIT};
    uint32_t _layerCount{1};
    uint32_t _levelCount{1};
};

#include <lug/Graphics/Vulkan/API/Builder/ImageView.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
