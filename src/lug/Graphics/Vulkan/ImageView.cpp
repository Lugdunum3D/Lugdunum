#include <lug/Graphics/Vulkan/ImageView.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

ImageView::ImageView(VkImageView imageView, const Device* device) : _imageView(imageView), _device(device) {}

ImageView::ImageView(ImageView&& imageView) {
    _imageView = imageView._imageView;
    _device = imageView._device;
    imageView._imageView = VK_NULL_HANDLE;
    imageView._device = nullptr;
}

ImageView& ImageView::operator=(ImageView&& imageView) {
    _imageView = imageView._imageView;
    _device = imageView._device;
    imageView._imageView = VK_NULL_HANDLE;
    imageView._device = nullptr;

    return *this;
}

void ImageView::destroy() {
    vkDestroyImageView(*_device, _imageView, nullptr);
}

} // Vulkan
} // Graphics
} // lug
