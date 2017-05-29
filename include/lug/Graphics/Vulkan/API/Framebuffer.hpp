#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class Framebuffer;
} // Builder

class Device;

class LUG_GRAPHICS_API Framebuffer {
    friend class Builder::Framebuffer;

public:
    // TODO: Declare offset and extent outside
    struct Extent {
        uint32_t width;
        uint32_t height;
    };

public:
    Framebuffer() = default;

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
    explicit Framebuffer(VkFramebuffer Framebuffer, const Device* device, const Extent& extent);

private:
    VkFramebuffer _framebuffer{VK_NULL_HANDLE};
    const Device* _device{nullptr};

    Extent _extent;
};

} // API
} // Vulkan
} // Graphics
} // lug
