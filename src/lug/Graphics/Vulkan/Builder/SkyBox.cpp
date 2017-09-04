#include <lug/Graphics/Vulkan/Builder/SkyBox.hpp>

#include <lug/Graphics/Builder/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/Builder/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/RenderPass.hpp>
#include <lug/Graphics/Builder/SkyBox.hpp>
#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/Render/SkyBox.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Builder {
namespace SkyBox {

static bool initSkyBoxPipeline(Renderer& renderer, API::GraphicsPipeline& skyBoxPipeline) {
    API::Builder::GraphicsPipeline graphicsPipelineBuilder(renderer.getDevice());

    // Set shaders
    {
        if (!graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_VERTEX_BIT, "main", renderer.getInfo().shadersRoot + "skybox.vert.spv")
            || !graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_FRAGMENT_BIT, "main", renderer.getInfo().shadersRoot + "skybox.frag.spv")) {
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
        VK_DYNAMIC_STATE_SCISSOR
    });

    // Set descriptorSetLayouts
    std::vector<Vulkan::API::DescriptorSetLayout> descriptorSetLayouts(2);
    {
        // Bindings set 0 : Camera uniform buffer (V)
        {
            API::Builder::DescriptorSetLayout descriptorSetLayoutBuilder(renderer.getDevice());

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
                LUG_LOG.error("initPipeline: Can't create pipeline descriptor sets layout 0: {}", result);
                return false;
            }
        }

        // Bindings set 1 : Skybox cube sampler (F)
        {
            API::Builder::DescriptorSetLayout descriptorSetLayoutBuilder(renderer.getDevice());

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
                LUG_LOG.error("initPipeline: Can't create pipeline descriptor sets layout 1: {}", result);
                return false;
            }
        }
    }

    // Set pipeline layout
    {
        API::PipelineLayout pipelineLayout{};
        API::Builder::PipelineLayout pipelineLayoutBuilder(renderer.getDevice());

        pipelineLayoutBuilder.setDescriptorSetLayouts(std::move(descriptorSetLayouts));

        VkResult result{VK_SUCCESS};
        if (!pipelineLayoutBuilder.build(pipelineLayout, &result)) {
            LUG_LOG.error("initPipeline: Can't create pipeline layout: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setPipelineLayout(std::move(pipelineLayout));
    }

    // Set render pass
    {
        API::Builder::RenderPass renderPassBuilder(renderer.getDevice());

        const VkAttachmentDescription colorAttachment{
            /* colorAttachment.flags */ 0,
            /* colorAttachment.format */ renderer.getRenderWindow()->getSwapchain().getFormat().format,
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
            renderer.getDevice(),
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
            LUG_LOG.error("initPipeline: Can't create render pass: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setRenderPass(std::move(renderPass), 0);
    }

    VkResult result{VK_SUCCESS};
    if (!graphicsPipelineBuilder.build(skyBoxPipeline, &result)) {
        LUG_LOG.error("initPipeline: Can't create pipeline: {}", result);
        return false;
    }

    return true;
}

static bool initIrradianceMapPipeline(Renderer& renderer, API::GraphicsPipeline& irradianceMapPipeline) {
    API::Builder::GraphicsPipeline graphicsPipelineBuilder(renderer.getDevice());

    // Set shaders
    {
        if (!graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_VERTEX_BIT, "main", renderer.getInfo().shadersRoot + "irradiance.vert.spv")
            || !graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_FRAGMENT_BIT, "main", renderer.getInfo().shadersRoot + "irradiance.frag.spv")) {
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
        VK_DYNAMIC_STATE_SCISSOR
    });

    // Set descriptorSetLayouts
    std::vector<Vulkan::API::DescriptorSetLayout> descriptorSetLayouts(1);
    {
        // Bindings set 0 : Skybox cube sampler (F)
        {
            API::Builder::DescriptorSetLayout descriptorSetLayoutBuilder(renderer.getDevice());

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

    // MVP matrix
    const VkPushConstantRange pushConstant{
        /* pushConstant.stageFlags */ VK_SHADER_STAGE_VERTEX_BIT,
        /* pushConstant.offset */ 0,
        /* pushConstant.size */ sizeof(Math::Mat4x4f)
    };

    // Set pipeline layout
    {
        API::PipelineLayout pipelineLayout{};
        API::Builder::PipelineLayout pipelineLayoutBuilder(renderer.getDevice());

        pipelineLayoutBuilder.setDescriptorSetLayouts(std::move(descriptorSetLayouts));
        pipelineLayoutBuilder.setPushConstants({pushConstant});

        VkResult result{VK_SUCCESS};
        if (!pipelineLayoutBuilder.build(pipelineLayout, &result)) {
            LUG_LOG.error("initPipeline: Can't create pipeline layout: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setPipelineLayout(std::move(pipelineLayout));
    }

    // Set render pass
    {
        API::Builder::RenderPass renderPassBuilder(renderer.getDevice());

        const VkAttachmentDescription colorAttachment{
            /* colorAttachment.flags */ 0,
            /* colorAttachment.format */ VK_FORMAT_R32G32B32A32_SFLOAT,
            /* colorAttachment.samples */ VK_SAMPLE_COUNT_1_BIT,
            /* colorAttachment.loadOp */ VK_ATTACHMENT_LOAD_OP_CLEAR,
            /* colorAttachment.storeOp */ VK_ATTACHMENT_STORE_OP_STORE,
            /* colorAttachment.stencilLoadOp */ VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            /* colorAttachment.stencilStoreOp */ VK_ATTACHMENT_STORE_OP_DONT_CARE,
            /* colorAttachment.initialLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            /* colorAttachment.finalLayout */ VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
        };

        auto colorAttachmentIndex = renderPassBuilder.addAttachment(colorAttachment);

        const VkFormat depthFormat = API::Image::findSupportedFormat(
            renderer.getDevice(),
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
            LUG_LOG.error("initPipeline: Can't create render pass: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setRenderPass(std::move(renderPass), 0);
    }

    VkResult result{VK_SUCCESS};
    if (!graphicsPipelineBuilder.build(irradianceMapPipeline, &result)) {
        LUG_LOG.error("initPipeline: Can't create pipeline: {}", result);
        return false;
    }

    return true;
}


static bool initMesh(Renderer& renderer, lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Mesh>& skyBoxMesh) {
    const std::vector<lug::Math::Vec3f> positions = {
        // Back
        {-1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},
        {-1.0f, 1.0f, -1.0f},
        {1.0f, 1.0f, -1.0f},

        // Front
        {-1.0f, -1.0f, 1.0f},
        {1.0f, -1.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},

        // Left
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, 1.0f},
        {-1.0f, 1.0f, -1.0f},
        {-1.0f, 1.0f, 1.0f},

        // Right
        {1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, 1.0f},
        {1.0f, 1.0f, -1.0f},
        {1.0f, 1.0f, 1.0f},

        // Bottom
        {-1.0f, -1.0f, -1.0f},
        {-1.0f, -1.0f, 1.0f},
        {1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, 1.0f},

        // Top
        {-1.0f, 1.0f, -1.0f},
        {-1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, -1.0f},
        {1.0f, 1.0f, 1.0}
    };

    const std::vector<uint16_t> indices = {
        // Back
        0, 2, 1,
        1, 2, 3,

        // Front
        6, 4, 5,
        7, 6, 5,

        // Left
        10, 8, 9,
        11, 10, 9,

        // Right
        14, 13, 12,
        15, 13, 14,

        // Bottom
        17, 16, 19,
        19, 16, 18,

        // Top
        23, 20, 21,
        22, 20, 23
    };

    // Build the mesh
    {
        lug::Graphics::Builder::Mesh meshBuilder(renderer);
        meshBuilder.setName("skybox");

        lug::Graphics::Builder::Mesh::PrimitiveSet* primitiveSet = meshBuilder.addPrimitiveSet();

        primitiveSet->setMode(lug::Graphics::Render::Mesh::PrimitiveSet::Mode::Triangles);

        primitiveSet->addAttributeBuffer(
            indices.data(),
            sizeof(uint16_t),
            static_cast<uint32_t>(indices.size()),
            lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Indice
        );

        primitiveSet->addAttributeBuffer(
            positions.data(),
            sizeof(lug::Math::Vec3f),
            static_cast<uint32_t>(positions.size()),
            lug::Graphics::Render::Mesh::PrimitiveSet::Attribute::Type::Position
        );

        skyBoxMesh = meshBuilder.build();

        if (!skyBoxMesh) {
            LUG_LOG.error("initMesh: Can't create the skyBox mesh");
            return false;
        }
    }

    return true;
}

Resource::SharedPtr<::lug::Graphics::Render::SkyBox> build(const ::lug::Graphics::Builder::SkyBox& builder) {
    // Constructor of SkyBox is private, we can't use std::make_unique
    std::unique_ptr<Resource> resource{new Vulkan::Render::SkyBox(builder._name)};
    Vulkan::Render::SkyBox* skyBox = static_cast<Vulkan::Render::SkyBox*>(resource.get());

    Vulkan::Renderer& renderer = static_cast<Vulkan::Renderer&>(builder._renderer);

    lug::Graphics::Builder::Texture textureBuilder(builder._renderer);

    textureBuilder.setType(lug::Graphics::Builder::Texture::Type::CubeMap);
    textureBuilder.setMagFilter(builder._magFilter);
    textureBuilder.setMinFilter(builder._minFilter);
    textureBuilder.setMipMapFilter(builder._mipMapFilter);
    textureBuilder.setWrapS(builder._wrapS);
    textureBuilder.setWrapT(builder._wrapT);
    textureBuilder.addLayer(builder._faces[static_cast<uint8_t>(lug::Graphics::Builder::SkyBox::Face::PositiveX)]);
    textureBuilder.addLayer(builder._faces[static_cast<uint8_t>(lug::Graphics::Builder::SkyBox::Face::NegativeX)]);
    textureBuilder.addLayer(builder._faces[static_cast<uint8_t>(lug::Graphics::Builder::SkyBox::Face::PositiveY)]);
    textureBuilder.addLayer(builder._faces[static_cast<uint8_t>(lug::Graphics::Builder::SkyBox::Face::NegativeY)]);
    textureBuilder.addLayer(builder._faces[static_cast<uint8_t>(lug::Graphics::Builder::SkyBox::Face::PositiveZ)]);
    textureBuilder.addLayer(builder._faces[static_cast<uint8_t>(lug::Graphics::Builder::SkyBox::Face::NegativeZ)]);

    skyBox->_texture = textureBuilder.build();
    if (!skyBox->_texture) {
        LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::build Can't create skyBox texture");
        return nullptr;
    }

    // Init the skyBox pipeline and mesh only one time
    ++Render::SkyBox::_skyBoxCount;
    if (Render::SkyBox::_skyBoxCount == 1) {
        if (!initSkyBoxPipeline(renderer, Render::SkyBox::_pipeline) ||
            !initIrradianceMapPipeline(renderer, Render::SkyBox::_irradianceMapPipeline) ||
            !initMesh(renderer, Render::SkyBox::_mesh)) {
            LUG_LOG.error("Resource::SharedPtr<::lug::Graphics::Render::SkyBox>::build Can't init skybox pipeline/mesh resources");
            return nullptr;
        }
    }

    return builder._renderer.getResourceManager()->add<::lug::Graphics::Render::SkyBox>(std::move(resource));
}

} // SkyBox
} // Builder
} // Vulkan
} // Graphics
} // lug
