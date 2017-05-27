#include <lug/Graphics/Vulkan/API/Pipeline.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/RenderPass.hpp>
#include <lug/Graphics/Vulkan/API/Builder/ShaderModule.hpp>
#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/Graphics/Vulkan/Render/Mesh.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

Pipeline::Pipeline(
    VkPipeline pipeline,
    const Device* device,
    std::unique_ptr<PipelineLayout> layout,
    std::unique_ptr<RenderPass> renderPass)
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
        vkDestroyPipeline(static_cast<VkDevice>(*_device), _pipeline, nullptr);
        _pipeline = VK_NULL_HANDLE;
    }

    if (_layout != nullptr) {
        _layout->destroy();
    }

    if (_renderPass != nullptr) {
        _renderPass->destroy();
    }
}

const RenderPass* Pipeline::getRenderPass() const {
    return _renderPass.get();
}

const PipelineLayout* Pipeline::getLayout() const {
    return _layout.get();
}

void Pipeline::bind(const CommandBuffer* commandBuffer) {
    commandBuffer->bindPipeline(*this, VK_PIPELINE_BIND_POINT_GRAPHICS);
}

std::unique_ptr<Pipeline> Pipeline::createGraphicsPipeline(const Device* device, const std::string& vertexShaderFile, const std::string& fragmentShaderFile, VkFormat colorFormat) {
    API::ShaderModule vertexShader;
    {
        Builder::ShaderModule shaderModuleBuilder(*device);

        if (!shaderModuleBuilder.loadFromFile(vertexShaderFile)) {
            return nullptr;
        }

        VkResult result{VK_SUCCESS};
        if (!shaderModuleBuilder.build(vertexShader, &result)) {
            LUG_LOG.error("Pipeline::createGraphicsPipeline: Can't create vertex shader: {}", result);
            return nullptr;
        }
    }

    API::ShaderModule fragmentShader;
    {
        Builder::ShaderModule shaderModuleBuilder(*device);

        if (!shaderModuleBuilder.loadFromFile(fragmentShaderFile)) {
            return nullptr;
        }

        VkResult result{VK_SUCCESS};
        if (!shaderModuleBuilder.build(fragmentShader, &result)) {
            LUG_LOG.error("Pipeline::createGraphicsPipeline: Can't create fragment shader: {}", result);
            return nullptr;
        }
    }

    // Vertex shader stage
    VkPipelineShaderStageCreateInfo vertexShaderStage{
        vertexShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        vertexShaderStage.pNext = nullptr,
        vertexShaderStage.flags = 0,
        vertexShaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT,
        vertexShaderStage.module = static_cast<VkShaderModule>(vertexShader),
        vertexShaderStage.pName = "main",
        vertexShaderStage.pSpecializationInfo = nullptr
    };

    // Fragment shader stage
    VkPipelineShaderStageCreateInfo fragmentShaderStage{
        fragmentShaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        fragmentShaderStage.pNext = nullptr,
        fragmentShaderStage.flags = 0,
        fragmentShaderStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
        fragmentShaderStage.module = static_cast<VkShaderModule>(fragmentShader),
        fragmentShaderStage.pName = "main",
        fragmentShaderStage.pSpecializationInfo = nullptr
    };

    VkPipelineShaderStageCreateInfo shaderStages[]{
        vertexShaderStage,
        fragmentShaderStage
    };


    VkVertexInputBindingDescription vertexInputBindingDesc{
        vertexInputBindingDesc.binding = 0,
        vertexInputBindingDesc.stride = sizeof(Render::Mesh::Vertex),
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
            vertexInputAttributesDesc[1].offset = offsetof(Render::Mesh::Vertex, color)
        },
        {
            vertexInputAttributesDesc[2].location = 2,
            vertexInputAttributesDesc[2].binding = 0,
            vertexInputAttributesDesc[2].format = VK_FORMAT_R32G32B32_SFLOAT,
            vertexInputAttributesDesc[2].offset = offsetof(Render::Mesh::Vertex, normal)
        },
        {
            vertexInputAttributesDesc[3].location = 3,
            vertexInputAttributesDesc[3].binding = 0,
            vertexInputAttributesDesc[3].format = VK_FORMAT_R32G32_SFLOAT,
            vertexInputAttributesDesc[3].offset = offsetof(Render::Mesh::Vertex, uv)
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

    VkPipelineDepthStencilStateCreateInfo depthStencil{
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
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE,
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD,
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
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

    VkResult result{VK_SUCCESS};
    std::unique_ptr<API::PipelineLayout> pipelineLayout = nullptr;

    // Create pipeline layout
    {
        std::vector<DescriptorSetLayout> descriptorSetLayouts(2);
        Builder::DescriptorSetLayout descriptorSetLayoutBuilder(*device);

        // Bindings set 0
        {
            // Camera uniform buffer
            VkDescriptorSetLayoutBinding binding{
                binding.binding = 0,
                binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                binding.descriptorCount = 1,
                binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                binding.pImmutableSamplers = nullptr // Only used for descriptorType VK_DESCRIPTOR_TYPE_SAMPLER or VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
            };

            descriptorSetLayoutBuilder.setBindings({binding});
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[0], &result)) {
                LUG_LOG.error("Pipeline::createGraphicsPipeline: Can't create pipeline descriptor sets layout 0: {}", result);
                return nullptr;
            }
        }

        // Bindings set 1
        {
            // Light uniform buffer
            VkDescriptorSetLayoutBinding binding{
                binding.binding = 0,
                binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                binding.descriptorCount = 1,
                binding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                binding.pImmutableSamplers = nullptr // Only used for descriptorType VK_DESCRIPTOR_TYPE_SAMPLER or VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
            };

            descriptorSetLayoutBuilder.setBindings({binding});
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[1], &result)) {
                LUG_LOG.error("Pipeline::createGraphicsPipeline: Can't create pipeline descriptor sets layout 1: {}", result);
                return nullptr;
            }
        }

        // Model transformation
        VkPushConstantRange pushConstant{
            pushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
            pushConstant.offset = 0,
            pushConstant.size = sizeof(Math::Mat4x4f)
        };

        API::Builder::PipelineLayout pipelineLayoutBuilder(*device);
        pipelineLayoutBuilder.setPushConstants({pushConstant});
        pipelineLayoutBuilder.setDescriptorSetLayouts(std::move(descriptorSetLayouts));

        pipelineLayout = pipelineLayoutBuilder.build(&result);
        if (!pipelineLayout) {
            LUG_LOG.error("Pipeline::createGraphicsPipeline: Can't create pipeline layout: {}", result);
            return nullptr;
        }
    }

    std::unique_ptr<API::RenderPass> renderPass;
    {
        Builder::RenderPass renderPassBuilder(*device);

        VkAttachmentDescription colorAttachment{
            colorAttachment.flags = 0,
            colorAttachment.format = colorFormat,
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT,
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        auto colorAttachmentIndex = renderPassBuilder.addAttachment(colorAttachment);

        VkFormat depthFormat = Image::findSupportedFormat(
            device,
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

        VkAttachmentDescription depthAttachment{
            depthAttachment.flags = 0,
            depthAttachment.format = depthFormat,
            depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT,
            depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };

        auto depthAttachmentIndex = renderPassBuilder.addAttachment(depthAttachment);

        Builder::RenderPass::SubpassDescription subpassDescription{
            subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            /* subpassDescription.inputAttachments */ {},
            /* subpassDescription.colorAttachments */ {{colorAttachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}},
            /* subpassDescription.resolveAttachments */ {},
            /* subpassDescription.depthStencilAttachment */ {depthAttachmentIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL},
            /* subpassDescription.preserveAttachments */ {},
        };

        renderPassBuilder.addSubpass(subpassDescription);

        renderPass = renderPassBuilder.build(&result);
        if (!renderPass) {
            LUG_LOG.error("Pipeline::createGraphicsPipeline: Can't create render pass: {}", result);
            return nullptr;
        }
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
        createInfo.layout = static_cast<VkPipelineLayout>(*pipelineLayout),
        createInfo.renderPass = static_cast<VkRenderPass>(*renderPass),
        createInfo.subpass = 0,
        createInfo.basePipelineHandle = VK_NULL_HANDLE,
        createInfo.basePipelineIndex = 0
    };

    VkPipeline pipeline = VK_NULL_HANDLE;

    // TODO: create with VkPipelineCache
    // TODO: create multiple pipelines with one call
    result = vkCreateGraphicsPipelines(static_cast<VkDevice>(*device), VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipeline);

    if (result != VK_SUCCESS) {
        LUG_LOG.error("RendererVulkan: Can't create graphics pipeline: {}", result);
        return nullptr;
    }

    return std::unique_ptr<Pipeline>(new Pipeline(pipeline, device, std::move(pipelineLayout), std::move(renderPass)));
}

} // API
} // Vulkan
} // Graphics
} // lug
