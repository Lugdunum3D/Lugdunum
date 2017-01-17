    #pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API Framebuffer {
public:
    Framebuffer(VkFramebuffer Framebuffer = VK_NULL_HANDLE, const Device* device = nullptr);

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer(Framebuffer&& Framebuffer);

    Framebuffer& operator=(const Framebuffer&) = delete;
    Framebuffer& operator=(Framebuffer&& Framebuffer);

    ~Framebuffer();

    operator VkFramebuffer() const {
        return _framebuffer;
    }

    void destroy();

private:
    VkFramebuffer _framebuffer{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
