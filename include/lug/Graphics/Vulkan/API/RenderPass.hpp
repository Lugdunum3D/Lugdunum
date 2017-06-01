#pragma once

#include <memory>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class RenderPass;
} // Builder

class CommandBuffer;
class Device;
class Framebuffer;

class LUG_GRAPHICS_API RenderPass {
    friend class Builder::RenderPass;

public:
    RenderPass() = default;

    RenderPass(const RenderPass&) = delete;
    RenderPass(RenderPass&& device);

    RenderPass& operator=(const RenderPass&) = delete;
    RenderPass& operator=(RenderPass&& device);

    ~RenderPass();

    explicit operator VkRenderPass() const {
        return _renderPass;
    }

    void destroy();

private:
    explicit RenderPass(VkRenderPass renderPass, const Device* device);

private:
    VkRenderPass _renderPass{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // API
} // Vulkan
} // Graphics
} // lug
