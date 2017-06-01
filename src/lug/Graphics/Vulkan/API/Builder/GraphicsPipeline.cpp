#include <lug/Graphics/Vulkan/API/Builder/GraphicsPipeline.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

GraphicsPipeline::GraphicsPipeline(const API::Device& device) : _device(device) {}

bool GraphicsPipeline::build(API::GraphicsPipeline& graphicPipeline, VkResult* returnResult) {
    // Vertex input state
    const VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{
        /* vertexInputCreateInfo.sType */ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        /* vertexInputCreateInfo.pNext */ nullptr,
        /* vertexInputCreateInfo.flags */ 0,
        /* vertexInputCreateInfo.vertexBindingDescriptionCount */ static_cast<uint32_t>(_inputBindings.size()),
        /* vertexInputCreateInfo.pVertexBindingDescriptions */ _inputBindings.data(),
        /* vertexInputCreateInfo.vertexAttributeDescriptionCount */ static_cast<uint32_t>(_inputAttributes.size()),
        /* vertexInputCreateInfo.pVertexAttributeDescriptions */ _inputAttributes.data()
    };

    // Input assembly state
    const VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo{
        /* inputAssemblyCreateInfo.sType */ VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        /* inputAssemblyCreateInfo.pNext */ nullptr,
        /* inputAssemblyCreateInfo.flags */ 0,
        /* inputAssemblyCreateInfo.topology */ _topology,
        /* inputAssemblyCreateInfo.primitiveRestartEnable */ _primitiveRestartEnable
    };

    // Viewport state
    _viewportState.viewportCount = static_cast<uint32_t>(_viewports.size());
    _viewportState.pViewports = _viewports.data();
    _viewportState.scissorCount = static_cast<uint32_t>(_scissors.size());
    _viewportState.pScissors = _scissors.data();

    // Color Blend state
    _colorBlendState.attachmentCount = static_cast<uint32_t>(_colorBlendAttachments.size());
    _colorBlendState.pAttachments = _colorBlendAttachments.data();

    // Dynamic state
    std::vector<VkDynamicState> dynamicStates(_dynamicStates.begin(), _dynamicStates.end());

    const VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{
        /* dynamicStateCreateInfo.sType */ VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        /* dynamicStateCreateInfo.pNext */ nullptr,
        /* dynamicStateCreateInfo.flags */ 0,
        /* dynamicStateCreateInfo.dynamicStateCount */ static_cast<uint32_t>(dynamicStates.size()),
        /* dynamicStateCreateInfo.pDynamicStates */ dynamicStates.data()
    };

    // Graphics Pipeline
    const VkGraphicsPipelineCreateInfo createInfo{
        /* createInfo.sType */ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        /* createInfo.pNext */ nullptr,
        /* createInfo.flags */ 0,
        /* createInfo.stageCount */ static_cast<uint32_t>(_shaderStages.size()),
        /* createInfo.pStages */ _shaderStages.data(),
        /* createInfo.pVertexInputState */ &vertexInputCreateInfo,
        /* createInfo.pInputAssemblyState */ &inputAssemblyCreateInfo,
        /* createInfo.pTessellationState */ nullptr,
        /* createInfo.pViewportState */ &_viewportState,
        /* createInfo.pRasterizationState */ &_rasterizationState,
        /* createInfo.pMultisampleState */ &_multisampleState,
        /* createInfo.pDepthStencilState */ &_depthStencilState,
        /* createInfo.pColorBlendState */ &_colorBlendState,
        /* createInfo.pDynamicState */ &dynamicStateCreateInfo,
        /* createInfo.layout */ static_cast<VkPipelineLayout>(_pipelineLayout),
        /* createInfo.renderPass */ static_cast<VkRenderPass>(_renderPass),
        /* createInfo.subpass */ _subpass,
        /* createInfo.basePipelineHandle */ VK_NULL_HANDLE,
        /* createInfo.basePipelineIndex */ 0
    };

    // Create the graphics pipeline
    VkPipeline vkGraphicPipeline = VK_NULL_HANDLE;
    VkResult result = vkCreateGraphicsPipelines(static_cast<VkDevice>(_device), _pipelineCache, 1, &createInfo, nullptr, &vkGraphicPipeline);

    if (returnResult) {
        *returnResult = result;
    }

    if (result != VK_SUCCESS) {
        return false;
    }

    graphicPipeline = API::GraphicsPipeline(vkGraphicPipeline, &_device, std::move(_renderPass), std::move(_pipelineLayout));

    return true;
}

std::unique_ptr<API::GraphicsPipeline> GraphicsPipeline::build(VkResult* returnResult) {
    std::unique_ptr<API::GraphicsPipeline> graphicPipeline = std::make_unique<API::GraphicsPipeline>();
    return build(*graphicPipeline, returnResult) ? std::move(graphicPipeline) : nullptr;
}

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
