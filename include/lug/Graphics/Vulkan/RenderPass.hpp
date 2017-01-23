#pragma once

#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Swapchain.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API RenderPass {
public:
    explicit RenderPass(VkRenderPass renderPass = VK_NULL_HANDLE, const Device* device = nullptr);

    RenderPass(const RenderPass&) = delete;
    RenderPass(RenderPass&& device);

    RenderPass& operator=(const RenderPass&) = delete;
    RenderPass& operator=(RenderPass&& device);

    ~RenderPass();

    operator VkRenderPass() const {
        return _renderPass;
    }

    void destroy();

    void begin(const CommandBuffer* commandBuffer, const Framebuffer& framebuffer, const VkExtent2D& extent, VkSubpassContents contents = VK_SUBPASS_CONTENTS_INLINE);
    void end(const CommandBuffer* commandBuffer);

    static std::unique_ptr<RenderPass> create(const Device* device);
private:
    VkRenderPass _renderPass{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
