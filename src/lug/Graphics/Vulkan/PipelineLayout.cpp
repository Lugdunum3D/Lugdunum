#include <lug/Graphics/Vulkan/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

PipelineLayout::PipelineLayout(VkPipelineLayout pipelineLayout, const Device* device) : _pipelineLayout(pipelineLayout), _device(device) {}

PipelineLayout::PipelineLayout(PipelineLayout&& pipelineLayout) {
    _pipelineLayout = pipelineLayout._pipelineLayout;
    _device = pipelineLayout._device;
    pipelineLayout._pipelineLayout = VK_NULL_HANDLE;
    pipelineLayout._device = nullptr;
}

PipelineLayout& PipelineLayout::operator=(PipelineLayout&& pipelineLayout) {
    destroy();

    _pipelineLayout = pipelineLayout._pipelineLayout;
    _device = pipelineLayout._device;
    pipelineLayout._pipelineLayout = VK_NULL_HANDLE;
    pipelineLayout._device = nullptr;

    return *this;
}

PipelineLayout::~PipelineLayout() {
    destroy();
}

void PipelineLayout::destroy() {
    if (_pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(*_device, _pipelineLayout, nullptr);
        _pipelineLayout = VK_NULL_HANDLE;
    }
}

std::unique_ptr<PipelineLayout> PipelineLayout::create(const Device* device) {
    VkPipelineLayoutCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.setLayoutCount = 0,
        createInfo.pSetLayouts = nullptr,
        createInfo.pushConstantRangeCount = 0,
        createInfo.pPushConstantRanges = nullptr
    };

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkResult result = vkCreatePipelineLayout(*device, &createInfo, nullptr, &pipelineLayout);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create pipeline layout: {}", result);
        return nullptr;
    }

    return std::unique_ptr<PipelineLayout>(new PipelineLayout(pipelineLayout, device));
}

} // Vulkan
} // Graphics
} // lug
