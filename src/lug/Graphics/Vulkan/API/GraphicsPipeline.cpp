#include <lug/Graphics/Vulkan/API/GraphicsPipeline.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

GraphicsPipeline::GraphicsPipeline(VkPipeline pipeline, const Device* device, RenderPass renderPass, PipelineLayout pipelineLayout) :
    _pipeline(pipeline), _device(device), _renderPass(std::move(renderPass)), _pipelineLayout(std::move(pipelineLayout)) {}

GraphicsPipeline::GraphicsPipeline(GraphicsPipeline&& pipeline) {
    _pipeline = pipeline._pipeline;
    _device = pipeline._device;
    _pipelineLayout = std::move(pipeline._pipelineLayout);
    _renderPass = std::move(pipeline._renderPass);
    pipeline._pipeline = VK_NULL_HANDLE;
    pipeline._device = nullptr;
}

GraphicsPipeline& GraphicsPipeline::operator=(GraphicsPipeline&& pipeline) {
    destroy();

    _pipeline = pipeline._pipeline;
    _device = pipeline._device;
    _pipelineLayout = std::move(pipeline._pipelineLayout);
    _renderPass = std::move(pipeline._renderPass);
    pipeline._pipeline = VK_NULL_HANDLE;
    pipeline._device = nullptr;

    return *this;
}

GraphicsPipeline::~GraphicsPipeline() {
    destroy();
}

void GraphicsPipeline::destroy() {
    if (_pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(static_cast<VkDevice>(*_device), _pipeline, nullptr);
        _pipeline = VK_NULL_HANDLE;
    }

    _pipelineLayout.destroy();
    _renderPass.destroy();
}

const RenderPass* GraphicsPipeline::getRenderPass() const {
    return &_renderPass;
}

const PipelineLayout* GraphicsPipeline::getLayout() const {
    return &_pipelineLayout;
}

} // API
} // Vulkan
} // Graphics
} // lug
