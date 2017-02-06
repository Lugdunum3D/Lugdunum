#include <lug/Graphics/Vulkan/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/Device.hpp>
#include <lug/Graphics/Vulkan/Mesh.hpp>
#include <lug/Graphics/Vulkan/Pipeline.hpp>
#include <lug/Graphics/Vulkan/ShaderModule.hpp>
#include <lug/System/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Pipeline::Pipeline(VkPipeline pipeline, const Device* device,
    std::unique_ptr<PipelineLayout> layout, std::unique_ptr<RenderPass> renderPass)
: _pipeline(pipeline), _device(device), _layout(std::move(layout)), _renderPass(std::move(renderPass)) {}

Pipeline::Pipeline(Pipeline&& pipeline) {
    _pipeline = pipeline._pipeline;
    _device = pipeline._device;
    _layout = std::move(pipeline._layout);
    _renderPass = std::move(pipeline._renderPass);
    pipeline._pipeline = VK_NULL_HANDLE;
    pipeline._device = nullptr;
    pipeline._layout = nullptr;
    pipeline._renderPass = nullptr;
}

Pipeline& Pipeline::operator=(Pipeline&& pipeline) {
    destroy();

    _pipeline = pipeline._pipeline;
    _device = pipeline._device;
    _layout = std::move(pipeline._layout);
    _renderPass = std::move(pipeline._renderPass);
    pipeline._pipeline = VK_NULL_HANDLE;
    pipeline._device = nullptr;
    pipeline._layout = nullptr;
    pipeline._renderPass = nullptr;

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

    if (_layout != nullptr) {
        _layout->destroy();
    }

    if (_renderPass != nullptr) {
        _renderPass->destroy();
    }
}

RenderPass* Pipeline::getRenderPass() const {
    return _renderPass.get();
}

PipelineLayout* Pipeline::getLayout() const {
    return _layout.get();
}

void Pipeline::bind(const CommandBuffer* commandBuffer) {
    vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
}

std::unique_ptr<Pipeline> Pipeline::createGraphicsPipeline(const Device* device, const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
    auto vertexShader = ShaderModule::create(vertexShaderFile, device);
    auto fragmentShader = ShaderModule::create(fragmentShaderFile, device);
    if (vertexShader == nullptr || fragmentShader == nullptr) {
        return nullptr;
    }

    // Vertex shader stage
    VkPipelineShaderStageCreateInfo vertexShaderStage{
        vertexShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        vertexShaderStage.pNext = nullptr,
        vertexShaderStage.flags = 0,
        vertexShaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT,
        vertexShaderStage.module = *vertexShader,
        vertexShaderStage.pName = "main",
        vertexShaderStage.pSpecializationInfo = nullptr
    };

    // Fragment shader stage
    VkPipelineShaderStageCreateInfo fragmentShaderStage{
        fragmentShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        fragmentShaderStage.pNext = nullptr,
        fragmentShaderStage.flags = 0,
        fragmentShaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        fragmentShaderStage.module = *fragmentShader,
        fragmentShaderStage.pName = "main",
        fragmentShaderStage.pSpecializationInfo = nullptr
    };

    VkPipelineShaderStageCreateInfo shaderStages[]{
        vertexShaderStage,
        fragmentShaderStage
    };


    VkVertexInputBindingDescription vertexInputBindingDesc{
        vertexInputBindingDesc.binding = 0,
        vertexInputBindingDesc.stride = sizeof(Mesh::Vertex),
        vertexInputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    VkVertexInputAttributeDescription vertexInputAttributesDesc[4] = {
        {
            vertexInputAttributesDesc[0].location = 0,
            vertexInputAttributesDesc[0].binding = 0,
            vertexInputAttributesDesc[0].format = VK_FORMAT_R32G32B32_SFLOAT,
            vertexInputAttributesDesc[0].offset = 0
        },
        {
            vertexInputAttributesDesc[1].location = 1,
            vertexInputAttributesDesc[1].binding = 0,
            vertexInputAttributesDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT,
            vertexInputAttributesDesc[1].offset = offsetof(Mesh::Vertex, color)
        },
        {
            vertexInputAttributesDesc[2].location = 2,
            vertexInputAttributesDesc[2].binding = 0,
            vertexInputAttributesDesc[2].format = VK_FORMAT_R32G32B32_SFLOAT,
            vertexInputAttributesDesc[2].offset = offsetof(Mesh::Vertex, normal)
        },
        {
            vertexInputAttributesDesc[3].location = 3,
            vertexInputAttributesDesc[3].binding = 0,
            vertexInputAttributesDesc[3].format = VK_FORMAT_R32G32_SFLOAT,
            vertexInputAttributesDesc[3].offset = offsetof(Mesh::Vertex, uv)
        }
    };

    // Vertex input state
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        vertexInputInfo.pNext = nullptr,
        vertexInputInfo.flags = 0,
        vertexInputInfo.vertexBindingDescriptionCount = 1,
        vertexInputInfo.pVertexBindingDescriptions = &vertexInputBindingDesc,
        vertexInputInfo.vertexAttributeDescriptionCount = 4,
        vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributesDesc
   };

    // Input assembly state
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{
        inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        inputAssemblyInfo.pNext = nullptr,
        inputAssemblyInfo.flags = 0,
        inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        inputAssemblyInfo.primitiveRestartEnable = VK_FALSE // because VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    };

    VkPipelineRasterizationStateCreateInfo rasterizer{
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        rasterizer.pNext = nullptr,
        rasterizer.flags = 0,
        rasterizer.depthClampEnable = VK_FALSE,
        rasterizer.rasterizerDiscardEnable = VK_FALSE,
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL,
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT,
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE,
        rasterizer.depthBiasEnable = VK_FALSE,
        rasterizer.depthBiasConstantFactor = 0.0f,
        rasterizer.depthBiasClamp = 0.0f,
        rasterizer.depthBiasSlopeFactor = 0.0f,
        rasterizer.lineWidth = 1.0f
    };

    VkPipelineMultisampleStateCreateInfo multisampling{
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        multisampling.pNext = nullptr,
        multisampling.flags = 0,
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        multisampling.sampleShadingEnable = VK_FALSE,
        multisampling.minSampleShading = 0.0f,
        multisampling.pSampleMask = nullptr,
        multisampling.alphaToCoverageEnable = VK_FALSE,
        multisampling.alphaToOneEnable = VK_FALSE
    };

    VkPipelineDepthStencilStateCreateInfo depthStencil {
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        depthStencil.pNext = nullptr,
        depthStencil.flags = 0,
        depthStencil.depthTestEnable = VK_TRUE,
        depthStencil.depthWriteEnable = VK_TRUE,
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL,
        depthStencil.depthBoundsTestEnable = VK_FALSE,
        depthStencil.stencilTestEnable = VK_FALSE,
        {}, // depthStencil.front (Used for stencil, we don't need)
        {}, // depthStencil.back (Used for stencil, we don't need)
        depthStencil.minDepthBounds = 0.0f, // For depth bound, we don't care
        depthStencil.maxDepthBounds = 0.0f // For depth bound, we don't care
    };

    VkPipelineColorBlendAttachmentState colorBlendAttachment{
        colorBlendAttachment.blendEnable = VK_TRUE,
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR,
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD,
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_CONSTANT_COLOR,
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD,
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    VkPipelineColorBlendStateCreateInfo colorBlending{
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        colorBlending.pNext = nullptr,
        colorBlending.flags = 0,
        colorBlending.logicOpEnable = VK_FALSE,
        colorBlending.logicOp = VK_LOGIC_OP_COPY,
        colorBlending.attachmentCount = 1,
        colorBlending.pAttachments = &colorBlendAttachment,
        {} // colorBlending.blendConstants
    };

    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    auto pipelineLayout = PipelineLayout::create(device);
    auto renderPass = RenderPass::create(device);

    if (!pipelineLayout || !renderPass) {
        return nullptr;
    }

    VkViewport viewport{
        viewport.x = 0.0f,
        viewport.y = 0.0f,
        viewport.width = 0.0f,
        viewport.height = 0.0f,
        viewport.minDepth = 0.0f,
        viewport.maxDepth = 1.0f,
    };

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {0, 0};

    VkPipelineViewportStateCreateInfo viewportState{
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        viewportState.pNext = nullptr,
        viewportState.flags = 0,
        viewportState.viewportCount = 1,
        viewportState.pViewports = &viewport,
        viewportState.scissorCount = 1,
        viewportState.pScissors = &scissor
    };

    const VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_BLEND_CONSTANTS
    };

    VkPipelineDynamicStateCreateInfo dynamicStateInfo{
        dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        dynamicStateInfo.pNext = nullptr,
        dynamicStateInfo.flags = 0,
        dynamicStateInfo.dynamicStateCount = 3,
        dynamicStateInfo.pDynamicStates = dynamicStates
    };

    VkGraphicsPipelineCreateInfo createInfo{
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        createInfo.pNext = nullptr,
        createInfo.flags = 0,
        createInfo.stageCount = 2,
        createInfo.pStages = shaderStages,
        createInfo.pVertexInputState = &vertexInputInfo,
        createInfo.pInputAssemblyState = &inputAssemblyInfo,
        createInfo.pTessellationState = nullptr,
        createInfo.pViewportState = &viewportState,
        createInfo.pRasterizationState = &rasterizer,
        createInfo.pMultisampleState = &multisampling,
        createInfo.pDepthStencilState = &depthStencil,
        createInfo.pColorBlendState = &colorBlending,
        createInfo.pDynamicState = &dynamicStateInfo,
        createInfo.layout = *pipelineLayout,
        createInfo.renderPass = *renderPass,
        createInfo.subpass = 0,
        createInfo.basePipelineHandle = VK_NULL_HANDLE,
        createInfo.basePipelineIndex = 0
    };

    VkPipeline pipeline = VK_NULL_HANDLE;

    // TODO: create with VkPipelineCache
    // TODO: create multiple pipelines with one call
    VkResult result = vkCreateGraphicsPipelines(*device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipeline);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create graphics pipeline: {}", result);
        return nullptr;
    }

    return std::unique_ptr<Pipeline>(new Pipeline(pipeline, device, std::move(pipelineLayout), std::move(renderPass)));
}

} // Vulkan
} // Graphics
} // lug
