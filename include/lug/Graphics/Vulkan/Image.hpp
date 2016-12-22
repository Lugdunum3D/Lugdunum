#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Image {
public:
    Image(VkImage Image = VK_NULL_HANDLE, const Device* device = nullptr);

    Image(const Image&) = delete;
    Image(Image&& Image);

    Image& operator=(const Image&) = delete;
    Image& operator=(Image&& Image);

    ~Image() = default;

    operator VkImage() const {
        return _image;
    }

private:
    VkImage _image{ VK_NULL_HANDLE };
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
