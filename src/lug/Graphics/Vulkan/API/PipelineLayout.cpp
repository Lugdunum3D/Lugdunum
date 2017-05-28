#include <lug/Graphics/Vulkan/API/PipelineLayout.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

PipelineLayout::PipelineLayout(
    VkPipelineLayout pipelineLayout,
    const Device* device,
    std::vector<DescriptorSetLayout> descriptorSetLayouts) :
    _pipelineLayout(pipelineLayout), _device(device), _descriptorSetLayouts(std::move(descriptorSetLayouts)) {}

PipelineLayout::PipelineLayout(PipelineLayout&& pipelineLayout) {
    _pipelineLayout = pipelineLayout._pipelineLayout;
    _device = pipelineLayout._device;
    _descriptorSetLayouts = std::move(pipelineLayout._descriptorSetLayouts);

    pipelineLayout._pipelineLayout = VK_NULL_HANDLE;
    pipelineLayout._device = nullptr;
    pipelineLayout._descriptorSetLayouts.resize(0);
}

PipelineLayout& PipelineLayout::operator=(PipelineLayout&& pipelineLayout) {
    destroy();

    _pipelineLayout = pipelineLayout._pipelineLayout;
    _device = pipelineLayout._device;
    _descriptorSetLayouts = std::move(pipelineLayout._descriptorSetLayouts);

    pipelineLayout._pipelineLayout = VK_NULL_HANDLE;
    pipelineLayout._device = nullptr;
    pipelineLayout._descriptorSetLayouts.resize(0);

    return *this;
}

PipelineLayout::~PipelineLayout() {
    destroy();
}

void PipelineLayout::destroy() {
    if (_pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(static_cast<VkDevice>(*_device), _pipelineLayout, nullptr);
        _pipelineLayout = VK_NULL_HANDLE;
    }

    _descriptorSetLayouts.clear();
}

} // API
} // Vulkan
} // Graphics
} // lug
