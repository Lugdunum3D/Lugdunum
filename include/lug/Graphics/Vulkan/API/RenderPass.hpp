#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class CommandBuffer;
class Device;
class Framebuffer;

namespace Builder {
class RenderPass;
} // Builder

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

    // TODO: Replace Math::Vec2f with Extent struct (not defined yet)
    void begin(const CommandBuffer* commandBuffer,
                const Framebuffer& framebuffer,
                const Math::Vec2f& renderExtent = {0, 0},
                const Math::Vec2f& renderOffset = {0, 0},
                VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
    void end(const CommandBuffer* commandBuffer);

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
