#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Device;

class LUG_GRAPHICS_API ImageView {
public:
    ImageView(VkImageView ImageView = VK_NULL_HANDLE, const Device* device = nullptr);

    ImageView(const ImageView&) = delete;
    ImageView(ImageView&& ImageView);

    ImageView& operator=(const ImageView&) = delete;
    ImageView& operator=(ImageView&& ImageView);

    ~ImageView();

    operator VkImageView() const {
        return _imageView;
    }

    void destroy();

private:
    VkImageView _imageView{ VK_NULL_HANDLE };
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
