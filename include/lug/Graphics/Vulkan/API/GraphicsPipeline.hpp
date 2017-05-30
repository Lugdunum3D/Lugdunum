#pragma once

#include <lug/Graphics/Vulkan/API/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/RenderPass.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class GraphicsPipeline;
} // Builder

class Device;

class LUG_GRAPHICS_API GraphicsPipeline {
    friend class Builder::GraphicsPipeline;

public:
    GraphicsPipeline() = default;

    GraphicsPipeline(const GraphicsPipeline&) = delete;
    GraphicsPipeline(GraphicsPipeline&& pipeline);

    GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
    GraphicsPipeline& operator=(GraphicsPipeline&& pipeline);

    ~GraphicsPipeline();

    explicit operator VkPipeline() const {
        return _pipeline;
    }

    void destroy();

    const RenderPass* getRenderPass() const;
    const PipelineLayout* getLayout() const;

private:
    explicit GraphicsPipeline(VkPipeline pipeline, const Device* device, RenderPass renderPass, PipelineLayout pipelineLayout);

private:
    VkPipeline _pipeline{VK_NULL_HANDLE};
    const Device* _device;

    RenderPass _renderPass;
    PipelineLayout _pipelineLayout;
};

} // API
} // Vulkan
} // Graphics
} // lug
