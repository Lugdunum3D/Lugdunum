#include <lug/Graphics/Vulkan/API/RenderPass.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

RenderPass::RenderPass(VkRenderPass renderPass, const Device* device) : _renderPass(renderPass), _device(device) {}

RenderPass::RenderPass(RenderPass&& renderPass) {
    _renderPass = renderPass._renderPass;
    _device = renderPass._device;
    renderPass._renderPass = VK_NULL_HANDLE;
    renderPass._device = nullptr;
}

RenderPass& RenderPass::operator=(RenderPass&& renderPass) {
    destroy();

    _renderPass = renderPass._renderPass;
    _device = renderPass._device;
    renderPass._renderPass = VK_NULL_HANDLE;
    renderPass._device = nullptr;

    return *this;
}

RenderPass::~RenderPass() {
    destroy();
}

void RenderPass::destroy() {
    if (_renderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(static_cast<VkDevice>(*_device), _renderPass, nullptr);
        _renderPass = VK_NULL_HANDLE;
    }
}

} // API
} // Vulkan
} // Graphics
} // lug
