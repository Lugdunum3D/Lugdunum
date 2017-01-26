#include <lug/Graphics/Vulkan/ImageView.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>

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
    destroy();

    _imageView = imageView._imageView;
    _device = imageView._device;
    imageView._imageView = VK_NULL_HANDLE;
    imageView._device = nullptr;

    return *this;
}

ImageView::~ImageView() {
    destroy();
}

void ImageView::destroy() {
    if (_imageView != VK_NULL_HANDLE) {
        vkDestroyImageView(*_device, _imageView, nullptr);
        _imageView = VK_NULL_HANDLE;
    }
}

} // Vulkan
} // Graphics
} // lug
