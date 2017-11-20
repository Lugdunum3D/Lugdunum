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

bool Pipeline::initPrefilteredMap() {
    API::Builder::GraphicsPipeline graphicsPipelineBuilder(_renderer.getDevice());

    // Set shaders
    {
        if (!graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_VERTEX_BIT, "main", _renderer.getInfo().shadersRoot + "filtercube.vert.spv")
            || !graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_FRAGMENT_BIT, "main", _renderer.getInfo().shadersRoot + "prefiltered.frag.spv")) {
            LUG_LOG.error("initPipeline: Can't create skybox shader");
            return false;
        }

        // Set vertex input state
        auto vertexBinding = graphicsPipelineBuilder.addInputBinding(sizeof(lug::Math::Vec3f), VK_VERTEX_INPUT_RATE_VERTEX);

        vertexBinding.addAttributes(VK_FORMAT_R32G32B32_SFLOAT, 0); // Position
    }

    // Set input assembly state
    graphicsPipelineBuilder.setInputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false);

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

    // Inverse the front face because we are inside the cube
    auto rasterizationState = graphicsPipelineBuilder.getRasterizationState();
    rasterizationState.setFrontFace(VK_FRONT_FACE_CLOCKWISE);

    // Set color blend state
    const VkPipelineColorBlendAttachmentState colorBlendAttachment{
        /* colorBlendAttachment.blendEnable */ VK_TRUE,
        /* colorBlendAttachment.srcColorBlendFactor */ VK_BLEND_FACTOR_ONE,
        /* colorBlendAttachment.dstColorBlendFactor */ VK_BLEND_FACTOR_CONSTANT_COLOR,
        /* colorBlendAttachment.colorBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.srcAlphaBlendFactor */ VK_BLEND_FACTOR_ONE,
        /* colorBlendAttachment.dstAlphaBlendFactor */ VK_BLEND_FACTOR_CONSTANT_COLOR,
        /* colorBlendAttachment.alphaBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.colorWriteMask */ VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    auto colorBlendState = graphicsPipelineBuilder.getColorBlendState();
    colorBlendState.addAttachment(colorBlendAttachment);

    // Set dynamic states
    graphicsPipelineBuilder.setDynamicStates({
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    });

    // Set descriptorSetLayouts
    std::vector<Vulkan::API::DescriptorSetLayout> descriptorSetLayouts(1);
    {
        // Bindings set 0 : Skybox cube sampler (F)
        {
            API::Builder::DescriptorSetLayout descriptorSetLayoutBuilder(_renderer.getDevice());

            // SkyBox cube sampler
            const VkDescriptorSetLayoutBinding binding{
                /* binding.binding */ 0,
                /* binding.descriptorType */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                /* binding.descriptorCount */ 1,
                /* binding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                /* binding.pImmutableSamplers */ nullptr
            };

            descriptorSetLayoutBuilder.setBindings({binding});
            VkResult result{VK_SUCCESS};
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[0], &result)) {
                LUG_LOG.error("initPipeline: Can't create pipeline descriptor sets layout 1: {}", result);
                return false;
            }
        }
    }

    // Set pipeline layout
    {
        API::PipelineLayout pipelineLayout{};
        API::Builder::PipelineLayout pipelineLayoutBuilder(_renderer.getDevice());

        pipelineLayoutBuilder.setDescriptorSetLayouts(std::move(descriptorSetLayouts));

        // MVP matrix
        const VkPushConstantRange pushConstantMVP{
            /* pushConstant.stageFlags */ VK_SHADER_STAGE_VERTEX_BIT,
            /* pushConstant.offset */ 0,
            /* pushConstant.size */ sizeof(Math::Mat4x4f)
        };

        // Roughness
        const VkPushConstantRange pushConstantRoughness{
            /* pushConstant.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
            /* pushConstant.offset */ sizeof(Math::Mat4x4f),
            /* pushConstant.size */ sizeof(float)
        };

        pipelineLayoutBuilder.setPushConstants({pushConstantMVP, pushConstantRoughness});

        VkResult result{VK_SUCCESS};
        if (!pipelineLayoutBuilder.build(pipelineLayout, &result)) {
            LUG_LOG.error("initPipeline: Can't create pipeline layout: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setPipelineLayout(std::move(pipelineLayout));
    }

    // Set render pass
    {
        API::Builder::RenderPass renderPassBuilder(_renderer.getDevice());

        const VkAttachmentDescription colorAttachment{
            /* colorAttachment.flags */ 0,
            /* colorAttachment.format */ VK_FORMAT_R32G32B32A32_SFLOAT, // TODO: Set the format otherwise
            /* colorAttachment.samples */ VK_SAMPLE_COUNT_1_BIT,
            /* colorAttachment.loadOp */ VK_ATTACHMENT_LOAD_OP_CLEAR,
            /* colorAttachment.storeOp */ VK_ATTACHMENT_STORE_OP_STORE,
            /* colorAttachment.stencilLoadOp */ VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            /* colorAttachment.stencilStoreOp */ VK_ATTACHMENT_STORE_OP_DONT_CARE,
            /* colorAttachment.initialLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            /* colorAttachment.finalLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        auto colorAttachmentIndex = renderPassBuilder.addAttachment(colorAttachment);

        const API::Builder::RenderPass::SubpassDescription subpassDescription{
            /* subpassDescription.pipelineBindPoint */ VK_PIPELINE_BIND_POINT_GRAPHICS,
            /* subpassDescription.inputAttachments */ {},
            /* subpassDescription.colorAttachments */ {{colorAttachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}},
            /* subpassDescription.resolveAttachments */ {},
            /* subpassDescription.depthStencilAttachment */ {},
            /* subpassDescription.preserveAttachments */ {},
        };

        renderPassBuilder.addSubpass(subpassDescription);

        VkResult result{VK_SUCCESS};
        API::RenderPass renderPass;
        if (!renderPassBuilder.build(renderPass, &result)) {
            LUG_LOG.error("initPipeline: Can't create render pass: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setRenderPass(std::move(renderPass), 0);
    }

    VkResult result{VK_SUCCESS};
    if (!graphicsPipelineBuilder.build(_pipeline, &result)) {
        LUG_LOG.error("initPipeline: Can't create pipeline: {}", result);
        return false;
    }

    return true;
}

} // Render
} // Vulkan
} // Graphics
} // lug
