#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>

#include <memory>

#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/ShaderModule.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/System/Exception.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Pipeline::Pipeline(Renderer& renderer, Id id) : Resource(Resource::Type::Pipeline), _renderer(renderer), _id(id) {}

bool Pipeline::init() {
    Pipeline::Id::PrimitivePart primitivePart = _id.getPrimitivePart();
    Pipeline::Id::MaterialPart materialPart = _id.getMaterialPart();

    // Create vertex shader
    API::ShaderModule vertexShader{};
    {
        std::vector<uint32_t> shaderCode{};

        try {
            shaderCode = Pipeline::ShaderBuilder::buildShader(
                _renderer.getInfo().shadersRoot,
                _renderer.getInfo().renderTechnique,
                Pipeline::ShaderBuilder::Type::Vertex,
                _id
            );
        } catch(const System::Exception& e) {
            LUG_LOG.error("{}", e.what());
            return false;
        }

        VkShaderModuleCreateInfo createInfo{
            createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            createInfo.pNext = nullptr,
            createInfo.flags = 0,
            createInfo.codeSize = shaderCode.size() * 4,
            createInfo.pCode = shaderCode.data(),
        };

        VkShaderModule shaderModule = VK_NULL_HANDLE;
        VkResult result = vkCreateShaderModule(static_cast<VkDevice>(_renderer.getDevice()), &createInfo, nullptr, &shaderModule);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("Pipeline: Can't create vertex shader: {}", result);
            return false;
        }

        vertexShader = API::ShaderModule(shaderModule, &_renderer.getDevice());
    }

    // Create fragment shader
    API::ShaderModule fragmentShader{};
    {
        std::vector<uint32_t> shaderCode{};

        try {
            shaderCode = Pipeline::ShaderBuilder::buildShader(
                _renderer.getInfo().shadersRoot,
                _renderer.getInfo().renderTechnique,
                Pipeline::ShaderBuilder::Type::Fragment,
                _id
            );
        } catch(const System::Exception& e) {
            LUG_LOG.error("{}", e.what());
            return false;
        }

        VkShaderModuleCreateInfo createInfo{
            createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            createInfo.pNext = nullptr,
            createInfo.flags = 0,
            createInfo.codeSize = shaderCode.size() * 4,
            createInfo.pCode = shaderCode.data(),
        };

        VkShaderModule shaderModule = VK_NULL_HANDLE;
        VkResult result = vkCreateShaderModule(static_cast<VkDevice>(_renderer.getDevice()), &createInfo, nullptr, &shaderModule);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("Pipeline: Can't create fragment shader: {}", result);
            return false;
        }

        fragmentShader = API::ShaderModule(shaderModule, &_renderer.getDevice());
    }

    // Create pipeline layout
    std::unique_ptr<API::PipelineLayout> pipelineLayout{};
    {
        std::vector<std::unique_ptr<API::DescriptorSetLayout>> descriptorSetLayouts;

        // Bindings set 0 : Camera uniform buffer (V/F)
        {
            // Camera uniform buffer
            VkDescriptorSetLayoutBinding cameraBinding ={
                cameraBinding.binding = 0,
                cameraBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                cameraBinding.descriptorCount = 1,
                cameraBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                cameraBinding.pImmutableSamplers = nullptr
            };

            std::vector<VkDescriptorSetLayoutBinding> bindings = {cameraBinding};

            descriptorSetLayouts.push_back(API::DescriptorSetLayout::create(&_renderer.getDevice(), bindings.data(), static_cast<uint32_t>(bindings.size())));
        }

        // Bindings set 1 : Light uniform buffer (F)
        {
            VkDescriptorSetLayoutBinding lightBinding = {
                lightBinding.binding = 0,
                lightBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                lightBinding.descriptorCount = 1,
                lightBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                lightBinding.pImmutableSamplers = nullptr
            };

            std::vector<VkDescriptorSetLayoutBinding> bindings = {lightBinding};

            descriptorSetLayouts.push_back(API::DescriptorSetLayout::create(&_renderer.getDevice(), bindings.data(), static_cast<uint32_t>(bindings.size())));
        }

        // Bindings set 2 : Material uniform buffer (F) + Material Samplers (F)
        {
            VkDescriptorSetLayoutBinding materialBinding = {
                materialBinding.binding = 0,
                materialBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                materialBinding.descriptorCount = 1,
                materialBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                materialBinding.pImmutableSamplers = nullptr
            };

            std::vector<VkDescriptorSetLayoutBinding> bindings = {materialBinding};

            // Set binding for samples of the material
            uint8_t binding = 1;

            if (materialPart.baseColorInfo != 0b11) {
                VkDescriptorSetLayoutBinding textureBinding = {
                    textureBinding.binding = binding++,
                    textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    textureBinding.descriptorCount = 1,
                    textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                    textureBinding.pImmutableSamplers = nullptr
                };

                bindings.push_back(textureBinding);
            }

            if (materialPart.metallicRoughnessInfo != 0b11) {
                VkDescriptorSetLayoutBinding textureBinding = {
                    textureBinding.binding = binding++,
                    textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    textureBinding.descriptorCount = 1,
                    textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                    textureBinding.pImmutableSamplers = nullptr
                };

                bindings.push_back(textureBinding);
            }

            if (materialPart.normalInfo != 0b11) {
                VkDescriptorSetLayoutBinding textureBinding = {
                    textureBinding.binding = binding++,
                    textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    textureBinding.descriptorCount = 1,
                    textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                    textureBinding.pImmutableSamplers = nullptr
                };

                bindings.push_back(textureBinding);
            }

            if (materialPart.occlusionInfo != 0b11) {
                VkDescriptorSetLayoutBinding textureBinding = {
                    textureBinding.binding = binding++,
                    textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    textureBinding.descriptorCount = 1,
                    textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                    textureBinding.pImmutableSamplers = nullptr
                };

                bindings.push_back(textureBinding);
            }

            if (materialPart.emissiveInfo != 0b11) {
                VkDescriptorSetLayoutBinding textureBinding = {
                    textureBinding.binding = binding++,
                    textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    textureBinding.descriptorCount = 1,
                    textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                    textureBinding.pImmutableSamplers = nullptr
                };

                bindings.push_back(textureBinding);
            }

            descriptorSetLayouts.push_back(API::DescriptorSetLayout::create(&_renderer.getDevice(), bindings.data(), static_cast<uint32_t>(bindings.size())));
        }

        if (!descriptorSetLayouts[0] || !descriptorSetLayouts[1] || !descriptorSetLayouts[2]) {
            LUG_LOG.error("Pipeline: Can't create pipeline descriptor sets layout");
            return false;
        }

        const std::vector<VkDescriptorSetLayout> vkDescriptorSetLayouts = {
            static_cast<VkDescriptorSetLayout>(*descriptorSetLayouts[0]),
            static_cast<VkDescriptorSetLayout>(*descriptorSetLayouts[1]),
            static_cast<VkDescriptorSetLayout>(*descriptorSetLayouts[2])
        };

        // Model transformation (V)
        VkPushConstantRange pushConstants[] = {
            {
                pushConstants[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                pushConstants[0].offset = 0,
                pushConstants[0].size = sizeof(Math::Mat4x4f)
            }
        };

        VkPipelineLayoutCreateInfo createInfo{
            createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            createInfo.pNext = nullptr,
            createInfo.flags = 0,
            createInfo.setLayoutCount = static_cast<uint32_t>(vkDescriptorSetLayouts.size()),
            createInfo.pSetLayouts = vkDescriptorSetLayouts.data(),
            createInfo.pushConstantRangeCount = 1,
            createInfo.pPushConstantRanges = pushConstants
        };

        VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
        VkResult result = vkCreatePipelineLayout(static_cast<VkDevice>(_renderer.getDevice()), &createInfo, nullptr, &vkPipelineLayout);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("Pipeline: Can't create pipeline layout: {}", result);
            return false;
        }

        pipelineLayout = std::make_unique<API::PipelineLayout>(descriptorSetLayouts, vkPipelineLayout, &_renderer.getDevice());
    }

    // Create the pipeline
    {
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

        std::vector<VkVertexInputBindingDescription> vertexInputBindingDescs{};
        std::vector<VkVertexInputAttributeDescription> vertexInputAttributesDescs{};

        // Set vertex data attributes
        {
            // We always have position
            VkVertexInputBindingDescription positionInputBindingDesc{
                positionInputBindingDesc.binding = 0,
                positionInputBindingDesc.stride = sizeof(Math::Vec3f),
                positionInputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            };

            VkVertexInputAttributeDescription positionInputAttributesDesc{
                positionInputAttributesDesc.location = 0,
                positionInputAttributesDesc.binding = 0,
                positionInputAttributesDesc.format = VK_FORMAT_R32G32B32_SFLOAT,
                positionInputAttributesDesc.offset = 0
            };

            vertexInputBindingDescs.push_back(positionInputBindingDesc);
            vertexInputAttributesDescs.push_back(positionInputAttributesDesc);

            // We always have normal
            VkVertexInputBindingDescription normalInputBindingDesc{
                normalInputBindingDesc.binding = 1,
                normalInputBindingDesc.stride = sizeof(Math::Vec3f),
                normalInputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
            };

            VkVertexInputAttributeDescription normalInputAttributesDesc{
                normalInputAttributesDesc.location = 1,
                normalInputAttributesDesc.binding = 1,
                normalInputAttributesDesc.format = VK_FORMAT_R32G32B32_SFLOAT,
                normalInputAttributesDesc.offset = 0
            };

            vertexInputBindingDescs.push_back(normalInputBindingDesc);
            vertexInputAttributesDescs.push_back(normalInputAttributesDesc);

            // Set vertex data attributes for dynamic attributes
            uint8_t location = 2;

            if (primitivePart.tangentVertexData) {
                VkVertexInputBindingDescription tangentInputBindingDesc{
                    tangentInputBindingDesc.binding = location,
                    tangentInputBindingDesc.stride = sizeof(Math::Vec3f),
                    tangentInputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
                };

                VkVertexInputAttributeDescription tangentInputAttributesDesc{
                    tangentInputAttributesDesc.location = location,
                    tangentInputAttributesDesc.binding = location++,
                    tangentInputAttributesDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT,
                    tangentInputAttributesDesc.offset = 0
                };

                vertexInputBindingDescs.push_back(tangentInputBindingDesc);
                vertexInputAttributesDescs.push_back(tangentInputAttributesDesc);
            }

            for (uint8_t i = 0; i < primitivePart.countTexCoord; ++i) {
                VkVertexInputBindingDescription uvInputBindingDesc{
                    uvInputBindingDesc.binding = location,
                    uvInputBindingDesc.stride = sizeof(Math::Vec2f),
                    uvInputBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
                };

                VkVertexInputAttributeDescription uvInputAttributesDesc{
                    uvInputAttributesDesc.location = location,
                    uvInputAttributesDesc.binding = location++,
                    uvInputAttributesDesc.format = VK_FORMAT_R32G32_SFLOAT,
                    uvInputAttributesDesc.offset = 0
                };

                vertexInputBindingDescs.push_back(uvInputBindingDesc);
                vertexInputAttributesDescs.push_back(uvInputAttributesDesc);
            }
        }

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            vertexInputInfo.pNext = nullptr,
            vertexInputInfo.flags = 0,
            vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindingDescs.size()),
            vertexInputInfo.pVertexBindingDescriptions = vertexInputBindingDescs.data(),
            vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributesDescs.size()),
            vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributesDescs.data()
        };

        // Input assembly state
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{
            inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            inputAssemblyInfo.pNext = nullptr,
            inputAssemblyInfo.flags = 0,
            inputAssemblyInfo.topology = [](Render::Mesh::PrimitiveSet::Mode mode) {
                switch(mode) {
                    case Render::Mesh::PrimitiveSet::Mode::Points:
                        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
                    case Render::Mesh::PrimitiveSet::Mode::Lines:
                        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                    case Render::Mesh::PrimitiveSet::Mode::LineStrip:
                        return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
                    case Render::Mesh::PrimitiveSet::Mode::Triangles:
                        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                    case Render::Mesh::PrimitiveSet::Mode::TriangleStrip:
                        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
                    case Render::Mesh::PrimitiveSet::Mode::TriangleFan:
                        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
                }

                return VkPrimitiveTopology{};
            }(static_cast<Render::Mesh::PrimitiveSet::Mode>(primitivePart.primitiveMode)),
            inputAssemblyInfo.primitiveRestartEnable = VK_FALSE
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

        auto renderPass = API::RenderPass::create(&_renderer.getDevice(), _renderer.getRenderWindow()->getSwapchain().getFormat().format);

        if (!renderPass) {
            return false;
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
        VkResult result = vkCreateGraphicsPipelines(static_cast<VkDevice>(_renderer.getDevice()), VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipeline);

        if (result != VK_SUCCESS) {
            LUG_LOG.error("Pipeline: Can't create graphics pipeline: {}", result);
            return false;
        }

        _pipeline = API::Pipeline(pipeline, &_renderer.getDevice(), std::move(pipelineLayout), std::move(renderPass));
    }

    return false;
}

Resource::SharedPtr<Pipeline> Pipeline::create(Renderer& renderer, Id id) {
    if (renderer.containsPipeline(id)) {
        return renderer.getPipeline(id);
    }

    std::unique_ptr<Resource> resource{new Pipeline(renderer, id)};
    Pipeline* pipeline = static_cast<Pipeline*>(resource.get());

    if (!pipeline->init()) {
        return nullptr;
    }

    Resource::SharedPtr<Pipeline> sharedPtrPipeline = renderer.getResourceManager()->add<Pipeline>(std::move(resource));
    renderer.addPipeline(sharedPtrPipeline);

    return sharedPtrPipeline;
}

} // Render
} // Vulkan
} // Graphics
} // lug
