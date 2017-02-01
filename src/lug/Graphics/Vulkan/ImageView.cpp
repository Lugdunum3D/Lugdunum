#include <lug/Graphics/Vulkan/ImageView.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

ImageView::ImageView(VkImageView imageView, const Device* device, const Extent& extent) : _imageView(imageView), _device(device), _extent(extent) {}

ImageView::ImageView(ImageView&& imageView) {
    _imageView = imageView._imageView;
    _device = imageView._device;
    _extent = imageView._extent;
    imageView._imageView = VK_NULL_HANDLE;
    imageView._device = nullptr;
    imageView._extent = {0, 0};
}

ImageView& ImageView::operator=(ImageView&& imageView) {
    destroy();

    _imageView = imageView._imageView;
    _device = imageView._device;
    _extent = imageView._extent;
    imageView._imageView = VK_NULL_HANDLE;
    imageView._device = nullptr;
    imageView._extent = {0, 0};

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
