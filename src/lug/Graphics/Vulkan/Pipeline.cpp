#include <lug/Graphics/Vulkan/Pipeline.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Pipeline::Pipeline(VkPipeline pipeline, const Device* device) : _pipeline(pipeline), _device(device) {}

Pipeline::Pipeline(Pipeline&& pipeline) {
    _pipeline = pipeline._pipeline;
    _device = pipeline._device;
    pipeline._pipeline = VK_NULL_HANDLE;
    pipeline._device = nullptr;
}

Pipeline& Pipeline::operator=(Pipeline&& pipeline) {
    destroy();

    _pipeline = pipeline._pipeline;
    _device = pipeline._device;
    pipeline._pipeline = VK_NULL_HANDLE;
    pipeline._device = nullptr;

    return *this;
}

Pipeline::~Pipeline() {
    destroy();
}

void Pipeline::destroy() {
    if (_pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(*_device, _pipeline, nullptr);
        _pipeline = VK_NULL_HANDLE;
    }
}

void Pipeline::bind(const CommandBuffer* commandBuffer) {
    vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
}

} // Vulkan
} // Graphics
} // lug
