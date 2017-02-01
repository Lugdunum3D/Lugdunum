#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Image.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Device;

class LUG_GRAPHICS_API ImageView {
public:
    // TODO: Declare offset and extent outside
    struct Extent{
        uint32_t width;
        uint32_t height;
    };

public:
    ImageView(VkImageView ImageView = VK_NULL_HANDLE, const Device* device = nullptr, const Extent& extent = {0, 0});

    ImageView(const ImageView&) = delete;
    ImageView(ImageView&& ImageView);

    ImageView& operator=(const ImageView&) = delete;
    ImageView& operator=(ImageView&& ImageView);

    ~ImageView();

    operator VkImageView() const {
        return _imageView;
    }

    const Extent& getExtent() const {
        return _extent;
    }

    void destroy();

    static std::unique_ptr<ImageView> create(
        const Device* device,
        const Image* image,
        VkFormat format,
        VkImageAspectFlags imageAspect = VK_IMAGE_ASPECT_COLOR_BIT,
        VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D
    );

private:
    VkImageView _imageView{ VK_NULL_HANDLE };
    const Device* _device{nullptr};

    Extent _extent;
};

} // Vulkan
} // Graphics
} // lug
