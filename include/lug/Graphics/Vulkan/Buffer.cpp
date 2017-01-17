#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Buffer {
public:
    Buffer(VkBuffer Buffer = VK_NULL_HANDLE, const Device* device = nullptr);

    Buffer(const Buffer&) = delete;
    Buffer(Buffer&& buffer);

    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&& buffer);

    ~Buffer();

    operator VkBuffer() const {
        return _buffer;
    }

    void destroy();

private:
    VkBuffer _buffer{ VK_NULL_HANDLE };
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
