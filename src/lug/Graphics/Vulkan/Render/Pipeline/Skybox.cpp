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

bool Pipeline::initSkybox() {
    Pipeline::Id::Skybox::ExtraPart extraPart = _id.getSkyboxExtraPart();

    API::Builder::GraphicsPipeline graphicsPipelineBuilder(_renderer.getDevice());

    // Set shaders
    {
        if (!graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_VERTEX_BIT, "main", _renderer.getInfo().shadersRoot + "skybox.vert.spv")
            || !graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_FRAGMENT_BIT, "main", _renderer.getInfo().shadersRoot + "skybox.frag.spv")) {
            LUG_LOG.error("Vulkan::Render::Pipeline: Can't create skybox shader");
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
        /* colorBlendAttachment.srcAlphaBlendFactor */ VK_BLEND_FACTOR_ZERO,
        /* colorBlendAttachment.dstAlphaBlendFactor */ VK_BLEND_FACTOR_CONSTANT_COLOR,
        /* colorBlendAttachment.alphaBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.colorWriteMask */ VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
    };

    auto colorBlendState = graphicsPipelineBuilder.getColorBlendState();
    colorBlendState.addAttachment(colorBlendAttachment);

    auto depthStencilState = graphicsPipelineBuilder.getDepthStencilState();
    depthStencilState.enableDepthTest(VK_COMPARE_OP_LESS_OR_EQUAL);

    // Set dynamic states
    graphicsPipelineBuilder.setDynamicStates({
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    });

    // Set descriptorSetLayouts
    std::vector<Vulkan::API::DescriptorSetLayout> descriptorSetLayouts(2);
    {
        // Bindings set 0 : Camera uniform buffer (V)
        {
            API::Builder::DescriptorSetLayout descriptorSetLayoutBuilder(_renderer.getDevice());

            // Camera uniform buffer
            // We only need it in the vertex shader, but we still use VK_SHADER_STAGE_FRAGMENT_BIT
            // because it needs to be compatible with the objects pipeline layout to use the same camera descriptor set
            const VkDescriptorSetLayoutBinding binding{
                /* binding.binding */ 0,
                /* binding.descriptorType */ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                /* binding.descriptorCount */ 1,
                /* binding.stageFlags */ VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                /* binding.pImmutableSamplers */ nullptr
            };

            descriptorSetLayoutBuilder.setBindings({binding});
            VkResult result{VK_SUCCESS};
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[0], &result)) {
                LUG_LOG.error("Vulkan::Render::Pipeline: Can't create pipeline descriptor sets layout 0: {}", result);
                return false;
            }
        }

        // Bindings set 1 : Skybox cube sampler (F)
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
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[1], &result)) {
                LUG_LOG.error("Vulkan::Render::Pipeline: Can't create pipeline descriptor sets layout 1: {}", result);
                return false;
            }
        }
    }

    // Set pipeline layout
    {
        API::PipelineLayout pipelineLayout{};
        API::Builder::PipelineLayout pipelineLayoutBuilder(_renderer.getDevice());

        pipelineLayoutBuilder.setDescriptorSetLayouts(std::move(descriptorSetLayouts));

        VkResult result{VK_SUCCESS};
        if (!pipelineLayoutBuilder.build(pipelineLayout, &result)) {
            LUG_LOG.error("Vulkan::Render::Pipeline: Can't create pipeline layout: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setPipelineLayout(std::move(pipelineLayout));
    }

    const VkSampleCountFlagBits nbSamples = [](Renderer::Antialiasing antialiasing) {
        switch (antialiasing) {
            case Renderer::Antialiasing::MSAA2X:
                return VK_SAMPLE_COUNT_2_BIT;
            case Renderer::Antialiasing::MSAA4X:
                return VK_SAMPLE_COUNT_4_BIT;
            case Renderer::Antialiasing::MSAA8X:
                return VK_SAMPLE_COUNT_8_BIT;
            case Renderer::Antialiasing::MSAA16X:
                return VK_SAMPLE_COUNT_16_BIT;
            default:
                return VK_SAMPLE_COUNT_1_BIT;
        };
    }(static_cast<Renderer::Antialiasing>(extraPart.antialiasing));

    auto multisampleState = graphicsPipelineBuilder.getMultisampleState();
    multisampleState.setRasterizationSamples(nbSamples);

    // Set render pass
    {
        API::Builder::RenderPass renderPassBuilder(_renderer.getDevice());

        const VkAttachmentDescription colorAttachment{
            /* colorAttachment.flags */ 0,
            /* colorAttachment.format */ _renderer.getRenderWindow()->getSwapchain().getFormat().format,
            /* colorAttachment.samples */ nbSamples,
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
            /* depthAttachment.samples */ nbSamples,
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

} // Render
} // Vulkan
} // Graphics
} // lug
