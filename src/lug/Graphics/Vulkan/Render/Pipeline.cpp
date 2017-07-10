#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>

#include <memory>

#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/Builder/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/RenderPass.hpp>
#include <lug/Graphics/Vulkan/API/Builder/ShaderModule.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/System/Exception.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Pipeline::Pipeline(Renderer& renderer, Id id) : Resource(Resource::Type::Pipeline, "Pipeline"), _renderer(renderer), _id(id) {}

bool Pipeline::init() {
    Pipeline::Id::PrimitivePart primitivePart = _id.getPrimitivePart();
    Pipeline::Id::MaterialPart materialPart = _id.getMaterialPart();

    API::Builder::GraphicsPipeline graphicsPipelineBuilder(_renderer.getDevice());

    // Set shaders state
    {
        // Vertex shader
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

            if (!graphicsPipelineBuilder.setShaderFromData(VK_SHADER_STAGE_VERTEX_BIT, "main", shaderCode)) {
                LUG_LOG.error("Vulkan::Render::Pipeline: Can't create vertex shader.");
                return false;
            }
        }

        // Frament shader
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

            if (!graphicsPipelineBuilder.setShaderFromData(VK_SHADER_STAGE_FRAGMENT_BIT, "main", shaderCode)) {
                LUG_LOG.error("Vulkan::Render::Pipeline: Can't create fragment shader.");
                return false;
            }
        }
    }

    // Set vertex input state
    {
        // We always have position
        auto positionBinding = graphicsPipelineBuilder.addInputBinding(sizeof(Math::Vec3f), VK_VERTEX_INPUT_RATE_VERTEX);
        positionBinding.addAttributes(VK_FORMAT_R32G32B32_SFLOAT, 0);

        // We always have normal
        auto normalBinding = graphicsPipelineBuilder.addInputBinding(sizeof(Math::Vec3f), VK_VERTEX_INPUT_RATE_VERTEX);
        normalBinding.addAttributes(VK_FORMAT_R32G32B32_SFLOAT, 0);

        // Set vertex data attributes for dynamic attributes
        if (primitivePart.tangentVertexData) {
            auto tangentBinding = graphicsPipelineBuilder.addInputBinding(sizeof(Math::Vec4f), VK_VERTEX_INPUT_RATE_VERTEX);
            tangentBinding.addAttributes(VK_FORMAT_R32G32B32A32_SFLOAT, 0);
        }

        for (uint8_t i = 0; i < primitivePart.countTexCoord; ++i) {
            auto uvBinding = graphicsPipelineBuilder.addInputBinding(sizeof(Math::Vec2f), VK_VERTEX_INPUT_RATE_VERTEX);
            uvBinding.addAttributes(VK_FORMAT_R32G32_SFLOAT, 0);
        }

        for (uint8_t i = 0; i < primitivePart.countColor; ++i) {
            auto uvBinding = graphicsPipelineBuilder.addInputBinding(sizeof(Math::Vec4f), VK_VERTEX_INPUT_RATE_VERTEX);
            uvBinding.addAttributes(VK_FORMAT_R32G32B32A32_SFLOAT, 0);
        }
    }

    // Set input assembly state
    switch(static_cast<Render::Mesh::PrimitiveSet::Mode>(primitivePart.primitiveMode)) {
        case Render::Mesh::PrimitiveSet::Mode::Points:
            graphicsPipelineBuilder.setInputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_POINT_LIST, false);
            break;
        case Render::Mesh::PrimitiveSet::Mode::Lines:
            graphicsPipelineBuilder.setInputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_LINE_LIST, false);
            break;
        case Render::Mesh::PrimitiveSet::Mode::LineStrip:
            graphicsPipelineBuilder.setInputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_LINE_STRIP, false);
            break;
        case Render::Mesh::PrimitiveSet::Mode::Triangles:
            graphicsPipelineBuilder.setInputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false);
            break;
        case Render::Mesh::PrimitiveSet::Mode::TriangleStrip:
            graphicsPipelineBuilder.setInputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP, false);
            break;
        case Render::Mesh::PrimitiveSet::Mode::TriangleFan:
            graphicsPipelineBuilder.setInputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN, false);
            break;
    }

    // Set viewport state
    const VkViewport viewport{
        /* viewport.x */ 0.0f,
        /* viewport.y */ 0.0f,
        /* viewport.width */ 0.0f,
        /* viewport.height */ 0.0f,
        /* viewport.minDepth */ 0.0f,
        /* viewport.maxDepth */ 1.0f,
    };

    const VkRect2D scissor{
        /* scissor.offset */ {0, 0},
        /* scissor.extent */ {0, 0}
    };

    auto viewportState = graphicsPipelineBuilder.getViewportState();
    viewportState.addViewport(viewport);
    viewportState.addScissor(scissor);

    // Set depth stencil state
    auto depthStencilState = graphicsPipelineBuilder.getDepthStencilState();
    depthStencilState.enableDepthTest(VK_COMPARE_OP_LESS_OR_EQUAL);
    depthStencilState.enableDepthWrite();

    // Set color blend state
    const VkPipelineColorBlendAttachmentState colorBlendAttachment{
        /* colorBlendAttachment.blendEnable */ VK_TRUE,
        /* colorBlendAttachment.srcColorBlendFactor */ VK_BLEND_FACTOR_ONE,
        /* colorBlendAttachment.dstColorBlendFactor */ VK_BLEND_FACTOR_CONSTANT_COLOR,
        /* colorBlendAttachment.colorBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.srcAlphaBlendFactor */ VK_BLEND_FACTOR_ZERO,
        /* colorBlendAttachment.dstAlphaBlendFactor */ VK_BLEND_FACTOR_CONSTANT_COLOR,
        /* colorBlendAttachment.alphaBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.colorWriteMask */ VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    auto colorBlendState = graphicsPipelineBuilder.getColorBlendState();
    colorBlendState.addAttachment(colorBlendAttachment);

    // Set dynamic states
    graphicsPipelineBuilder.setDynamicStates({
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_BLEND_CONSTANTS
    });

    // Set pipeline layout
    {
        VkResult result{VK_SUCCESS};
        std::vector<API::DescriptorSetLayout> descriptorSetLayouts(3);
        API::Builder::DescriptorSetLayout descriptorSetLayoutBuilder(_renderer.getDevice());

        // Bindings set 0 : Camera uniform buffer (V/F)
        {
            // Camera uniform buffer
            const VkDescriptorSetLayoutBinding binding{
                /* binding.binding */ 0,
                /* binding.descriptorType */ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                /* binding.descriptorCount */ 1,
                /* binding.stageFlags */ VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                /* binding.pImmutableSamplers */ nullptr
            };

            descriptorSetLayoutBuilder.setBindings({binding});
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[0], &result)) {
                LUG_LOG.error("Vulkan::Render::Pipeline: Can't create pipeline descriptor sets layout 0: {}", result);
                return false;
            }
        }

        // Bindings set 1 : Light uniform buffer (F)
        {
            const std::vector<VkDescriptorSetLayoutBinding> bindings{
                // Light array uniform buffer
                {
                    /* binding.binding */ 0,
                    /* binding.descriptorType */ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                    /* binding.descriptorCount */ 1,
                    /* binding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                    /* binding.pImmutableSamplers */ nullptr
                }
            };

            descriptorSetLayoutBuilder.setBindings(bindings);
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[1], &result)) {
                LUG_LOG.error("Vulkan::Render::Pipeline: Can't create pipeline descriptor sets layout 1: {}", result);
                return false;
            }
        }

        // Bindings set 2 : Material uniform buffer (F) + Material Samplers (F)
        {
            const VkDescriptorSetLayoutBinding materialBinding{
                /* materialBinding.binding */ 0,
                /* materialBinding.descriptorType */ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                /* materialBinding.descriptorCount */ 1,
                /* materialBinding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                /* materialBinding.pImmutableSamplers */ nullptr
            };

            std::vector<VkDescriptorSetLayoutBinding> bindings = {materialBinding};

            // Set binding for samples of the material
            uint8_t binding = 1;

            if (materialPart.baseColorInfo != 0b11) {
                const VkDescriptorSetLayoutBinding textureBinding = {
                    /* textureBinding.binding */ binding++,
                    /* textureBinding.descriptorType */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    /* textureBinding.descriptorCount */ 1,
                    /* textureBinding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                    /* textureBinding.pImmutableSamplers */ nullptr
                };

                bindings.push_back(std::move(textureBinding));
            }

            if (materialPart.metallicRoughnessInfo != 0b11) {
                const VkDescriptorSetLayoutBinding textureBinding = {
                    /* textureBinding.binding */ binding++,
                    /* textureBinding.descriptorType */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    /* textureBinding.descriptorCount */ 1,
                    /* textureBinding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                    /* textureBinding.pImmutableSamplers */ nullptr
                };

                bindings.push_back(std::move(textureBinding));
            }

            if (materialPart.normalInfo != 0b11) {
                const VkDescriptorSetLayoutBinding textureBinding = {
                    /* textureBinding.binding */ binding++,
                    /* textureBinding.descriptorType */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    /* textureBinding.descriptorCount */ 1,
                    /* textureBinding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                    /* textureBinding.pImmutableSamplers */ nullptr
                };

                bindings.push_back(std::move(textureBinding));
            }

            if (materialPart.occlusionInfo != 0b11) {
                const VkDescriptorSetLayoutBinding textureBinding = {
                    /* textureBinding.binding */ binding++,
                    /* textureBinding.descriptorType */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    /* textureBinding.descriptorCount */ 1,
                    /* textureBinding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                    /* textureBinding.pImmutableSamplers */ nullptr
                };

                bindings.push_back(std::move(textureBinding));
            }

            if (materialPart.emissiveInfo != 0b11) {
                const VkDescriptorSetLayoutBinding textureBinding = {
                    /* textureBinding.binding */ binding++,
                    /* textureBinding.descriptorType */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                    /* textureBinding.descriptorCount */ 1,
                    /* textureBinding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                    /* textureBinding.pImmutableSamplers */ nullptr
                };

                bindings.push_back(std::move(textureBinding));
            }

            descriptorSetLayoutBuilder.setBindings(bindings);
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[2], &result)) {
                LUG_LOG.error("Vulkan::Render::Pipeline: Can't create pipeline descriptor sets layout 2: {}", result);
                return false;
            }
        }

        // Model transformation
        const VkPushConstantRange pushConstant{
            /* pushConstant.stageFlags */ VK_SHADER_STAGE_VERTEX_BIT,
            /* pushConstant.offset */ 0,
            /* pushConstant.size */ sizeof(Math::Mat4x4f)
        };

        API::Builder::PipelineLayout pipelineLayoutBuilder(_renderer.getDevice());

        pipelineLayoutBuilder.setPushConstants({pushConstant});
        pipelineLayoutBuilder.setDescriptorSetLayouts(std::move(descriptorSetLayouts));

        API::PipelineLayout pipelineLayout;
        if (!pipelineLayoutBuilder.build(pipelineLayout, &result)) {
            LUG_LOG.error("Vulkan::Render::Pipeline: Can't create pipeline layout: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setPipelineLayout(std::move(pipelineLayout));
    }

    // Set render pass
    {
        API::Builder::RenderPass renderPassBuilder(_renderer.getDevice());

        const VkAttachmentDescription colorAttachment{
            /* colorAttachment.flags */ 0,
            /* colorAttachment.format */ _renderer.getRenderWindow()->getSwapchain().getFormat().format,
            /* colorAttachment.samples */ VK_SAMPLE_COUNT_1_BIT,
            /* colorAttachment.loadOp */ VK_ATTACHMENT_LOAD_OP_CLEAR,
            /* colorAttachment.storeOp */ VK_ATTACHMENT_STORE_OP_STORE,
            /* colorAttachment.stencilLoadOp */ VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            /* colorAttachment.stencilStoreOp */ VK_ATTACHMENT_STORE_OP_DONT_CARE,
            /* colorAttachment.initialLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            /* colorAttachment.finalLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        auto colorAttachmentIndex = renderPassBuilder.addAttachment(colorAttachment);

        const VkFormat depthFormat = API::Image::findSupportedFormat(
            _renderer.getDevice(),
            {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );

        const VkAttachmentDescription depthAttachment{
            /* depthAttachment.flags */ 0,
            /* depthAttachment.format */ depthFormat,
            /* depthAttachment.samples */ VK_SAMPLE_COUNT_1_BIT,
            /* depthAttachment.loadOp */ VK_ATTACHMENT_LOAD_OP_CLEAR,
            /* depthAttachment.storeOp */ VK_ATTACHMENT_STORE_OP_STORE,
            /* depthAttachment.stencilLoadOp */ VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            /* depthAttachment.stencilStoreOp */ VK_ATTACHMENT_STORE_OP_DONT_CARE,
            /* depthAttachment.initialLayout */ VK_IMAGE_LAYOUT_UNDEFINED,
            /* depthAttachment.finalLayout */ VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        };

        auto depthAttachmentIndex = renderPassBuilder.addAttachment(depthAttachment);

        const API::Builder::RenderPass::SubpassDescription subpassDescription{
            /* subpassDescription.pipelineBindPoint */ VK_PIPELINE_BIND_POINT_GRAPHICS,
            /* subpassDescription.inputAttachments */ {},
            /* subpassDescription.colorAttachments */ {{colorAttachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}},
            /* subpassDescription.resolveAttachments */ {},
            /* subpassDescription.depthStencilAttachment */ {depthAttachmentIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL},
            /* subpassDescription.preserveAttachments */ {},
        };

        renderPassBuilder.addSubpass(subpassDescription);

        VkResult result{VK_SUCCESS};
        API::RenderPass renderPass;
        if (!renderPassBuilder.build(renderPass, &result)) {
            LUG_LOG.error("Vulkan::Render::Pipeline: Can't create render pass: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setRenderPass(std::move(renderPass), 0);
    }

    VkResult result{VK_SUCCESS};
    if (!graphicsPipelineBuilder.build(_pipeline, &result)) {
        LUG_LOG.error("Vulkan::Render::Pipeline: Can't create pipeline: {}", result);
        return false;
    }

    return true;
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
