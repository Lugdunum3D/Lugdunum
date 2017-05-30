#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class ImageView;
} // Builder

class Device;
class Image;

class LUG_GRAPHICS_API ImageView {
    friend class Builder::ImageView;

public:
    ImageView() = default;

    ImageView(const ImageView&) = delete;
    ImageView(ImageView&& ImageView);

    ImageView& operator=(const ImageView&) = delete;
    ImageView& operator=(ImageView&& ImageView);

    ~ImageView();

    explicit operator VkImageView() const {
        return _imageView;
    }

    /**
     * @brief      Gets the device associated with this ImageView
     *
     * @return     The device.
     */
    const Device* getDevice() const;

    /**
     * @brief      Gets the image associated with this ImageView
     *
     * @return     The image.
     */
    const Image* getImage() const;

    void destroy();

private:
    explicit ImageView(VkImageView ImageView, const Device* device, const Image* extent);

private:
    VkImageView _imageView{VK_NULL_HANDLE};
    const Device* _device{nullptr};

    const Image* _image{nullptr};
};

#include <lug/Graphics/Vulkan/API/ImageView.inl>

} // API
} // Vulkan
} // Graphics
} // lug
