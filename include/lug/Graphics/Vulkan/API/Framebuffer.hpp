    #pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

class LUG_GRAPHICS_API Framebuffer {
public:
    // TODO: Declare offset and extent outside
    struct Extent {
        uint32_t width;
        uint32_t height;
    };

public:
    explicit Framebuffer(VkFramebuffer Framebuffer = VK_NULL_HANDLE, const Device* device = nullptr, const Extent& extent = {0, 0});

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer(Framebuffer&& Framebuffer);

    Framebuffer& operator=(const Framebuffer&) = delete;
    Framebuffer& operator=(Framebuffer&& Framebuffer);

    ~Framebuffer();

    explicit operator VkFramebuffer() const {
        return _framebuffer;
    }

    const Extent& getExtent() const {
        return _extent;
    }

    void destroy();

private:
    VkFramebuffer _framebuffer{VK_NULL_HANDLE};
    const Device* _device{nullptr};

    Extent _extent;
};

} // API
} // Vulkan
} // Graphics
} // lug
