#include <lug/Graphics/Vulkan/Image.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Image::Image(VkImage image, const Device* device) : _image(image), _device(device) {}

Image::Image(Image&& image) {
    _image = image._image;
    _device = image._device;
    image._image = VK_NULL_HANDLE;
    image._device = nullptr;
}

Image& Image::operator=(Image&& image) {
    _image = image._image;
    _device = image._device;
    image._image = VK_NULL_HANDLE;
    image._device = nullptr;

    return *this;
}

} // Vulkan
} // Graphics
} // lug
