#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>

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

private:
    VkRenderPass _renderPass{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // Vulkan
} // Graphics
} // lug
