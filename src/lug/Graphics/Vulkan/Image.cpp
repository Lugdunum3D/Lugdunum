#include <lug/Graphics/Vulkan/Image.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Image::Image(VkImage image, const Device* device, bool swapchainImage) : _image(image), _device(device), _swapchainImage(swapchainImage) {}

Image::Image(Image&& image) {
    _image = image._image;
    _device = image._device;
    _swapchainImage = image._swapchainImage;
    image._image = VK_NULL_HANDLE;
    image._device = nullptr;
}

Image& Image::operator=(Image&& image) {
    _image = image._image;
    _device = image._device;
    _swapchainImage = image._swapchainImage;
    image._image = VK_NULL_HANDLE;
    image._device = nullptr;

    return *this;
}

Image::~Image() {
    // Swapchain images are automatically destroyed when destroying swapchain
    if (!_swapchainImage) {
        destroy();
    }
}

void Image::destroy() {
    if (_image != VK_NULL_HANDLE) {
        vkDestroyImage(*_device, _image, nullptr);
        _image = VK_NULL_HANDLE;
    }
}

} // Vulkan
} // Graphics
} // lug
