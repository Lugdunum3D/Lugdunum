#include <lug/Graphics/Vulkan/Image.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Image::Image(VkImage image, const Device* device, bool swapchainImage, VkImageAspectFlags aspect) :
            _image(image), _device(device), _swapchainImage(swapchainImage), _aspect(aspect) {}

Image::Image(Image&& image) {
    _image = image._image;
    _device = image._device;
    _swapchainImage = image._swapchainImage;
    _aspect = image._aspect;
    image._image = VK_NULL_HANDLE;
    image._device = nullptr;
}

Image& Image::operator=(Image&& image) {
    destroy();

    _image = image._image;
    _device = image._device;
    _swapchainImage = image._swapchainImage;
    _aspect = image._aspect;
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
