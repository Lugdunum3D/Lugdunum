#include <lug/Graphics/Vulkan/API/ImageView.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

ImageView::ImageView(VkImageView imageView, const Device* device, const Image* image) : _imageView(imageView), _device(device), _image(image) {}

ImageView::ImageView(ImageView&& imageView) {
    _imageView = imageView._imageView;
    _device = imageView._device;
    _image = imageView._image;
    imageView._imageView = VK_NULL_HANDLE;
    imageView._device = nullptr;
    imageView._image = nullptr;
}

ImageView& ImageView::operator=(ImageView&& imageView) {
    destroy();

    _imageView = imageView._imageView;
    _device = imageView._device;
    _image = imageView._image;
    imageView._imageView = VK_NULL_HANDLE;
    imageView._device = nullptr;
    imageView._image = nullptr;

    return *this;
}

ImageView::~ImageView() {
    destroy();
}

void ImageView::destroy() {
    if (_imageView != VK_NULL_HANDLE) {
        vkDestroyImageView(static_cast<VkDevice>(*_device), _imageView, nullptr);
        _imageView = VK_NULL_HANDLE;
        _device = nullptr;
        _image = nullptr;
    }
}

} // API
} // Vulkan
} // Graphics
} // lug
