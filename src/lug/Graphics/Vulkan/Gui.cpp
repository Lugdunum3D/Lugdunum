
#include <math.h>
#include <lug/Graphics/Vulkan/Gui.hpp>

#include <imgui.h>
#include <imgui_freetype.h>

#include <lug/Graphics/Vulkan/API/Builder/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/Builder/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/RenderPass.hpp>
#include <lug/Graphics/Vulkan/API/Builder/Sampler.hpp>
#include <lug/Graphics/Vulkan/Render/Window.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

Gui::Gui(lug::Graphics::Vulkan::Renderer& renderer, lug::Graphics::Vulkan::Render::Window& window) : _renderer(renderer), _window(window) {
}

Gui::~Gui() {
    destroy();
}

void Gui::destroy() {
    _fontImage.destroy();
    _fontImageView.destroy();
    _fontDeviceMemory.destroy();
    _fontSampler.destroy();

    _descriptorPool.destroy();

    _pipeline.destroy();

    for (const auto& frameData: _framesData) {
        if (frameData.vertexMemoryPtr) {
            frameData.vertexMemory.unmap();
        }

        if (frameData.indexMemoryPtr) {
            frameData.indexMemory.unmap();
        }
    }

    _framesData.clear();

    _graphicQueueCommandPool.destroy();
    _transferQueueCommandPool.destroy();

    _graphicQueue = nullptr;
    _transferQueue = nullptr;
}

bool Gui::init(const std::vector<API::ImageView>& imageViews) {
    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->AddFontDefault();

    initKeyMapping();

    io.DisplaySize = ImVec2(_window.getWidth(), _window.getHeight());
    io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

    API::Device& device = _renderer.getDevice();

    // Get graphic queue family and retrieve the first queue
    {
        _graphicQueue = device.getQueue("queue_graphics");
        if (!_graphicQueue) {
            LUG_LOG.error("Gui::init: Can't find graphic queue");
            return false;
        }

    }

    // Create command pool of graphic queue
    {
        VkResult result{VK_SUCCESS};
        API::Builder::CommandPool commandPoolBuilder(device, *_graphicQueue->getQueueFamily());
        if (!commandPoolBuilder.build(_graphicQueueCommandPool, &result)) {
            LUG_LOG.error("Gui::init: Can't create a command pool: {}", result);
            return false;
        }
    }

    API::Builder::CommandBuffer commandBufferBuilder(_renderer.getDevice(), _graphicQueueCommandPool);
    commandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    // Create command buffers (as a temporary vector first)
    std::vector<Vulkan::API::CommandBuffer> commandBuffers(imageViews.size());

    // Command buffers
    {
        VkResult result{VK_SUCCESS};
        if (!commandBufferBuilder.build(commandBuffers, &result)) {
            LUG_LOG.error("Gui::init: Can't create the command buffer: {}", result);
            return false;
        }
    }

    // Initialize FrameData's
    _framesData.resize(imageViews.size());
    for (uint32_t i = 0; i < imageViews.size(); ++i) {
        _framesData[i].commandBuffer = std::move(commandBuffers[i]);
    }
    commandBuffers.clear(); // clear the array

    return initFontsTexture() && initPipeline() && initFramebuffers(imageViews);
}

void Gui::initKeyMapping() {
    ImGuiIO& io = ImGui::GetIO();

    // Keyboard mapping for specific shortcuts used by ImGui
    io.KeyMap[ImGuiKey_Tab] = static_cast<int>(lug::Window::Keyboard::Key::Tab);
    io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(lug::Window::Keyboard::Key::Left);
    io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(lug::Window::Keyboard::Key::Right);
    io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(lug::Window::Keyboard::Key::Up);
    io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(lug::Window::Keyboard::Key::Down);
    io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(lug::Window::Keyboard::Key::PageUp);
    io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(lug::Window::Keyboard::Key::PageDown);
    io.KeyMap[ImGuiKey_Home] = static_cast<int>(lug::Window::Keyboard::Key::Home);
    io.KeyMap[ImGuiKey_End] = static_cast<int>(lug::Window::Keyboard::Key::End);
    io.KeyMap[ImGuiKey_Delete] = static_cast<int>(lug::Window::Keyboard::Key::Delete);
    io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(lug::Window::Keyboard::Key::BackSpace);
    io.KeyMap[ImGuiKey_Enter] = static_cast<int>(lug::Window::Keyboard::Key::Return);
    io.KeyMap[ImGuiKey_Escape] = static_cast<int>(lug::Window::Keyboard::Key::Escape);
    io.KeyMap[ImGuiKey_A] = static_cast<int>(lug::Window::Keyboard::Key::A);
    io.KeyMap[ImGuiKey_C] = static_cast<int>(lug::Window::Keyboard::Key::C);
    io.KeyMap[ImGuiKey_V] = static_cast<int>(lug::Window::Keyboard::Key::V);
    io.KeyMap[ImGuiKey_X] = static_cast<int>(lug::Window::Keyboard::Key::X);
    io.KeyMap[ImGuiKey_Y] = static_cast<int>(lug::Window::Keyboard::Key::Y);
    io.KeyMap[ImGuiKey_Z] = static_cast<int>(lug::Window::Keyboard::Key::Z);
}

bool Gui::initFontsTexture() {
    ImGuiIO& io = ImGui::GetIO();

    // Create font texture
    unsigned char* fontData = nullptr;
    uint32_t texWidth = 0;
    uint32_t texHeight = 0;

    {
        int tempWidth;
        int tempHeight;
        // See ImGuiFreeType::RasterizationFlags
        unsigned int flags = ImGuiFreeType::NoHinting;
        ImGuiFreeType::BuildFontAtlas(io.Fonts, flags);
        io.Fonts->GetTexDataAsRGBA32(&fontData, &tempWidth, &tempHeight);
        texWidth = static_cast<uint32_t>(tempWidth);
        texHeight = static_cast<uint32_t>(tempHeight);
    }

    const size_t uploadSize = texWidth * texHeight * 4 * sizeof(char);

    API::Device &device = _renderer.getDevice();

    // Get transfer queue family and retrieve the first queue
    {
        _transferQueue = device.getQueue("queue_transfer");
        if (!_transferQueue) {
            LUG_LOG.error("Gui::initFontsTexture: Can't find transfer queue");
            return false;
        }
    }

    // Create command pool of transfer queue
    {
        VkResult result{ VK_SUCCESS };
        API::Builder::CommandPool commandPoolBuilder(device, *_transferQueue->getQueueFamily());
        if (!commandPoolBuilder.build(_transferQueueCommandPool, &result)) {
            LUG_LOG.error("Gui::init: Can't create a command pool: {}", result);
            return false;
        }
    }

    // Create FontsTexture image
    {
        API::Builder::Image imageBuilder(device);

        imageBuilder.setUsage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
        imageBuilder.setPreferedFormats({ VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT });
        imageBuilder.setFeatureFlags(VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
        imageBuilder.setQueueFamilyIndices({ _transferQueue->getQueueFamily()->getIdx() });
        imageBuilder.setTiling(VK_IMAGE_TILING_OPTIMAL);

        API::Builder::DeviceMemory deviceMemoryBuilder(device);
        deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        VkExtent3D extent{
            /* extent.width */ texWidth,
            /* extent.height */ texHeight,
            /* extent.depth */ 1
        };

        imageBuilder.setExtent(extent);

        {
            VkResult result{VK_SUCCESS};
            if (!imageBuilder.build(_fontImage, &result)) {
                LUG_LOG.error("Gui::initFontsTexture: Can't create depth buffer image: {}", result);
                return false;
            }
            result = VK_SUCCESS;
            if (!deviceMemoryBuilder.addImage(_fontImage)) {
                LUG_LOG.error("Gui::initFontsTexture: Can't add image to device memory");
                return false;
            }

            result = VK_SUCCESS;
            if (!deviceMemoryBuilder.build(_fontDeviceMemory, &result)) {
                LUG_LOG.error("Gui::initFontsTexture: Can't create font buffer device memory: {}", result);
                return false;
            }
        }
    }

    // Create FontsTexture image view
    {
        API::Builder::ImageView imageViewBuilder(device, _fontImage);

        imageViewBuilder.setFormat(_fontImage.getFormat());
        imageViewBuilder.setAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT);

        VkResult result{VK_SUCCESS};
        if (!imageViewBuilder.build(_fontImageView, &result)) {
            LUG_LOG.error("Gui::initFontsTexture: Can't create image view: {}", result);
            return false;
        }

        // Set font id to font image view
        io.Fonts->TexID = (void *)(intptr_t)static_cast<VkImageView>(_fontImageView);
    }

    // Create staging buffers for font data upload
    {
        API::Buffer stagingBuffer;
        API::DeviceMemory stagingBufferMemory;
        std::set<uint32_t> queueFamilyIndices = { _transferQueue->getQueueFamily()->getIdx() };

        // Create staging buffer
        {
            API::Builder::Buffer bufferBuilder(device);
            bufferBuilder.setQueueFamilyIndices(queueFamilyIndices);
            bufferBuilder.setSize(uploadSize);
            bufferBuilder.setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
            bufferBuilder.setExclusive(VK_SHARING_MODE_EXCLUSIVE);

            VkResult result{VK_SUCCESS};
            if (!bufferBuilder.build(stagingBuffer, &result)) {
                LUG_LOG.error("Gui::initFontsTexture: Can't create staging buffer: {}", result);
                return false;
            }
        }

        // Create staging buffer memory
        {
            API::Builder::DeviceMemory deviceMemoryBuilder(device);
            deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            deviceMemoryBuilder.addBuffer(stagingBuffer);

            VkResult result{VK_SUCCESS};
            if (!deviceMemoryBuilder.build(stagingBufferMemory, &result)) {
                LUG_LOG.error("Gui::initFontsTexture: Can't create staging buffer device memory: {}", result);
                return false;
            }
        }

        stagingBuffer.updateData(fontData, (uint32_t)uploadSize);

        // Copy buffer data to font image
        {
            VkResult result{VK_SUCCESS};
            API::CommandBuffer commandBuffer;

            API::Builder::CommandBuffer commandBufferBuilder(_renderer.getDevice(), _transferQueueCommandPool);
            commandBufferBuilder.setLevel(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

            if (!commandBufferBuilder.build(commandBuffer, &result)) {
                LUG_LOG.error("Gui::init: Can't create the command buffer: {}", result);
                return false;
            }

            // Create fence
            API::Fence fence;
            {
                API::Builder::Fence fenceBuilder(device);

                if (!fenceBuilder.build(fence, &result)) {
                    LUG_LOG.error("Gui::initFontsTexture: Can't create swapchain fence: {}", result);
                    return false;
                }
            }

            commandBuffer.begin();

            // Prepare for transfer
            {
                API::CommandBuffer::CmdPipelineBarrier pipelineBarrier;
                pipelineBarrier.imageMemoryBarriers.resize(1);
                pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = 0;
                pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].image = &_fontImage;

                commandBuffer.pipelineBarrier(pipelineBarrier);
            }

            // Copy
            VkBufferImageCopy bufferCopyRegion{
                /* bufferCopyRegion.bufferOffset */ 0,
                /* bufferCopyRegion.bufferRowLength */ 0,
                /* bufferCopyRegion.bufferImageHeight */ 0,
                {
                    /* bufferCopyRegion.imageSubresource.aspectMask */ VK_IMAGE_ASPECT_COLOR_BIT,
                    /* bufferCopyRegion.imageSubresource.mipLevel */ 0,
                    /* bufferCopyRegion.imageSubresource.baseArrayLayer */ 0,
                    /* bufferCopyRegion.imageSubresource.layerCount */ 1
                },
                {
                    /* bufferCopyRegion.imageOffset.x */ 0,
                    /* bufferCopyRegion.imageOffset.y */ 0,
                    /* bufferCopyRegion.imageOffset.z */ 0,
                },
                {
                    /* bufferCopyRegion.imageExtent.width */ texWidth,
                    /* bufferCopyRegion.imageExtent.height */ texHeight,
                    /* bufferCopyRegion.imageExtent.depth */ 1
                }
            };

//            // TODO write this function
//            commandBuffer.copyBufferToImage();

            vkCmdCopyBufferToImage(
                static_cast<VkCommandBuffer>(commandBuffer),
                static_cast<VkBuffer>(stagingBuffer),
                static_cast<VkImage>(_fontImage),
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                1,
                &bufferCopyRegion
            );

            // Prepare for shader read
            {
                API::CommandBuffer::CmdPipelineBarrier pipelineBarrier;
                pipelineBarrier.imageMemoryBarriers.resize(1);
                pipelineBarrier.imageMemoryBarriers[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                pipelineBarrier.imageMemoryBarriers[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                pipelineBarrier.imageMemoryBarriers[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                pipelineBarrier.imageMemoryBarriers[0].image = &_fontImage;

                commandBuffer.pipelineBarrier(pipelineBarrier);
            }

            if (commandBuffer.end() == false) {
                LUG_LOG.error("Gui::initFontsTexture: Failed to end commandBuffer");
                return false;
            }

            if (_transferQueue->submit(commandBuffer, {}, {}, {}, static_cast<VkFence>(fence)) == false) {
                LUG_LOG.error("Gui::initFontsTexture: Can't submit commandBuffer");
                return false;
            }

            // TODO(saveman71): set a define for the fence timeout
            if (!fence.wait()) {
                LUG_LOG.error("Gui::initFontsTexture: Can't vkWaitForFences");
                return false;
            }

            // Properly destroy everything in the right order
            fence.destroy();
            stagingBufferMemory.destroy();
            stagingBuffer.destroy();
            commandBuffer.destroy();
        }
    }

    // Font texture Sampler
    {
        API::Builder::Sampler samplerBuilder(device);

        samplerBuilder.setAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        samplerBuilder.setAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        samplerBuilder.setAddressModeW(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
        samplerBuilder.setBorderColor(VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE);

        VkResult result{VK_SUCCESS};
        if (!samplerBuilder.build(_fontSampler, &result)) {
            LUG_LOG.error("Gui::initFontsTexture: Can't create image view: {}", result);
            return false;
        }
    }

    if (initFrameData() == false) {
        LUG_LOG.error("Gui::initFontsTexture: Failed to init frame data");
        return false;
    }


    return true;
}

bool Gui::initPipeline() {
    API::Device &device = _renderer.getDevice();

    API::Builder::GraphicsPipeline graphicsPipelineBuilder(_renderer.getDevice());

    // Set shaders state
    if (!graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_VERTEX_BIT, "main", _renderer.getInfo().shadersRoot + "gui.vert.spv")
        || !graphicsPipelineBuilder.setShaderFromFile(VK_SHADER_STAGE_FRAGMENT_BIT, "main", _renderer.getInfo().shadersRoot + "gui.frag.spv")) {
        LUG_LOG.error("ForwardRenderer: Can't create pipeline's shaders.");
        return false;
    }

    // Set vertex input state
    auto vertexBinding = graphicsPipelineBuilder.addInputBinding(sizeof(ImDrawVert), VK_VERTEX_INPUT_RATE_VERTEX);

    vertexBinding.addAttributes(VK_FORMAT_R32G32B32_SFLOAT, offsetof(ImDrawVert, pos)); // Position
    vertexBinding.addAttributes(VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv)); // UV
    vertexBinding.addAttributes(VK_FORMAT_R8G8B8A8_UNORM, offsetof(ImDrawVert, col)); // Color

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
        /* scissor.offset */{ 0, 0 },
        /* scissor.extent */{ 0, 0 }
    };

    auto viewportState = graphicsPipelineBuilder.getViewportState();
    viewportState.addViewport(viewport);
    viewportState.addScissor(scissor);

    // Set rasterization state
    auto rasterizationState = graphicsPipelineBuilder.getRasterizationState();
    rasterizationState.setCullMode(VK_CULL_MODE_NONE);

    // Set color blend state
    const VkPipelineColorBlendAttachmentState colorBlendAttachment{
        /* colorBlendAttachment.blendEnable */ VK_TRUE,
        /* colorBlendAttachment.srcColorBlendFactor */ VK_BLEND_FACTOR_SRC_ALPHA,
        /* colorBlendAttachment.dstColorBlendFactor */ VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        /* colorBlendAttachment.colorBlendOp */ VK_BLEND_OP_ADD,
        /* colorBlendAttachment.srcAlphaBlendFactor */ VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        /* colorBlendAttachment.dstAlphaBlendFactor */ VK_BLEND_FACTOR_ZERO,
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

    std::vector<Vulkan::API::DescriptorSetLayout> descriptorSetLayouts;
    {
        // creating descriptor pool
        {
            API::Builder::DescriptorPool descriptorPoolBuilder(device);

            // Use VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT to individually free descritors sets
            descriptorPoolBuilder.setFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);

            // Only ForwardRenderTechnique has 1 descriptor sets (for lights) and 1 (for the camera)
            descriptorPoolBuilder.setMaxSets(1);

            VkDescriptorPoolSize poolSize{
                // Dynamic uniform buffers descriptors (1 for camera and 1 for lights in ForwardRenderTechnique)
                poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                poolSize.descriptorCount = 1
            };
            descriptorPoolBuilder.setPoolSizes({ poolSize });

            VkResult result{VK_SUCCESS};
            if (!descriptorPoolBuilder.build(_descriptorPool, &result)) {
                LUG_LOG.error("Gui::initPipeline: Can't create the descriptor pool: {}", result);
                return false;
            }
        }

        // descriptorSetLayout
        {
            API::Builder::DescriptorSetLayout descriptorSetLayoutBuilder(device);

            // Camera uniform buffer
            const VkDescriptorSetLayoutBinding binding{
                /* binding.binding */ 0,
                /* binding.descriptorType */ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                /* binding.descriptorCount */ 1,
                /* binding.stageFlags */ VK_SHADER_STAGE_FRAGMENT_BIT,
                /* binding.pImmutableSamplers */ nullptr
            };

            descriptorSetLayoutBuilder.setBindings({ binding });

            // create descriptor set
            VkResult result{VK_SUCCESS};
            descriptorSetLayouts.resize(1);
            if (!descriptorSetLayoutBuilder.build(descriptorSetLayouts[0], &result)) {
                LUG_LOG.error("Gui::initPipeline: Can't create pipeline descriptor: {}", result);
                return false;
            }
        }

        // Create and update descriptor set
        {
            API::Builder::DescriptorSet descriptorSetBuilder(device, _descriptorPool);
            descriptorSetBuilder.setDescriptorSetLayouts({ static_cast<VkDescriptorSetLayout>(descriptorSetLayouts[0]) });

            VkResult result{VK_SUCCESS};
            if (!descriptorSetBuilder.build(_descriptorSet, &result)) {
                LUG_LOG.error("Gui::initPipeline: Can't create descriptor set: {}", result);
                return false;
            }
        }

        VkPushConstantRange pushConstant{
            /*pushConstant.stageFlags*/ VK_SHADER_STAGE_VERTEX_BIT,
            /*pushConstant.offset*/ 0,
            /*pushConstant.size*/ sizeof(PushConstBlock)
        };

        API::Builder::PipelineLayout pipelineLayoutBuilder(device);

        pipelineLayoutBuilder.setPushConstants({ pushConstant });
        pipelineLayoutBuilder.setDescriptorSetLayouts(std::move(descriptorSetLayouts));

        VkResult result{VK_SUCCESS};
        if (!pipelineLayoutBuilder.build(_pipelineLayout, &result)) {
            LUG_LOG.error("Gui::initPipeline: Can't create pipeline layout: {}", result);
            return false;
        }
        graphicsPipelineBuilder.setPipelineLayout(std::move(_pipelineLayout));
    }

    // Set render pass
    {
        API::Builder::RenderPass renderPassBuilder(_renderer.getDevice());

        auto colorFormat = _window.getSwapchain().getFormat().format;

        const VkAttachmentDescription colorAttachment{
            /* colorAttachment.flags */ 0,
            /* colorAttachment.format */ colorFormat,
            /* colorAttachment.samples */ VK_SAMPLE_COUNT_1_BIT,
            /* colorAttachment.loadOp */ VK_ATTACHMENT_LOAD_OP_LOAD,
            /* colorAttachment.storeOp */ VK_ATTACHMENT_STORE_OP_STORE,
            /* colorAttachment.stencilLoadOp */ VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            /* colorAttachment.stencilStoreOp */ VK_ATTACHMENT_STORE_OP_DONT_CARE,
            /* colorAttachment.initialLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            /* colorAttachment.finalLayout */ VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        };

        auto colorAttachmentIndex = renderPassBuilder.addAttachment(colorAttachment);

        const API::Builder::RenderPass::SubpassDescription subpassDescription{
            /* subpassDescription.pipelineBindPoint */ VK_PIPELINE_BIND_POINT_GRAPHICS,
            /* subpassDescription.inputAttachments */{},
            /* subpassDescription.colorAttachments */{ { colorAttachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
            /* subpassDescription.resolveAttachments */{},
            /* subpassDescription.depthStencilAttachment */{},
            /* subpassDescription.preserveAttachments */{},
        };

        renderPassBuilder.addSubpass(subpassDescription);

        VkResult result{VK_SUCCESS};
        API::RenderPass renderPass;
        if (!renderPassBuilder.build(renderPass, &result)) {
            LUG_LOG.error("Gui::initPipeline: Can't create render pass: {}", result);
            return false;
        }

        graphicsPipelineBuilder.setRenderPass(std::move(renderPass), 0);
    }

    VkResult result{VK_SUCCESS};
    if (!graphicsPipelineBuilder.build(_pipeline, &result)) {
        LUG_LOG.error("Gui::initPipeline: Can't create pipeline: {}", result);
        return false;
    }

    return true;
}

bool Gui::initFramebuffers(const std::vector<API::ImageView>& imageViews) {
    const API::RenderPass* renderPass = _pipeline.getRenderPass();

    for (size_t i = 0; i < imageViews.size(); ++i) {
        // Create depth buffer image view
        API::Builder::Framebuffer framebufferBuilder(_renderer.getDevice());

        framebufferBuilder.setRenderPass(renderPass);
        framebufferBuilder.addAttachment(&imageViews[i]);
        framebufferBuilder.setWidth(imageViews[i].getImage()->getExtent().width);
        framebufferBuilder.setHeight(imageViews[i].getImage()->getExtent().height);

        VkResult result{VK_SUCCESS};
        if (!framebufferBuilder.build(_framesData[i].framebuffer, &result)) {
            LUG_LOG.error("Gui::initFramebuffers: Can't create framebuffer: {}", result);
            return false;
        }
    }

    return true;
}

void Gui::beginFrame(const lug::System::Time& elapsedTime) {
    ImGuiIO& io = ImGui::GetIO();

    io.DeltaTime = elapsedTime.getSeconds();

    io.DisplaySize = ImVec2(_window.getWidth(), _window.getHeight());
#if defined(LUG_SYSTEM_ANDROID)
io.MousePos = ImVec2(_window._touchScreenState.coordinates[0].x(), _window._touchScreenState.coordinates[0].y());
io.MouseDown[0] = _window._touchScreenState.tap;
#else
    const auto mousePos = _window.getMousePos();
    io.MousePos = ImVec2(static_cast<float>(mousePos.x()), static_cast<float>(mousePos.y()));

    #endif
    ImGui::NewFrame();
}

bool Gui::endFrame(const std::vector<VkSemaphore>& waitSemaphores, uint32_t currentImageIndex) {
    ImGui::Render();
    FrameData& frameData = _framesData[currentImageIndex];

    if (!frameData.fence.wait()) {
        return false;
    }
    frameData.fence.reset();
    if (updateBuffers(currentImageIndex) == false) {
        LUG_LOG.error("Gui::endFrame: Failed to update buffers");
        return false;
    }

    frameData.commandBuffer.reset();
    frameData.commandBuffer.begin();

    ImGuiIO& io = ImGui::GetIO();

    io.MouseWheel = 0.f;

    // Init viewport
    {
        const VkViewport vkViewport{
            /* vkViewport.x */ 0,
            /* vkViewport.y */ 0,
            /* vkViewport.width */ io.DisplaySize.x,
            /* vkViewport.height */ io.DisplaySize.y,
            /* vkViewport.minDepth */ 0.0f,
            /* vkViewport.maxDepth */ 1.0f,
        };

        frameData.commandBuffer.setViewport({vkViewport});
    }

    const API::RenderPass* renderPass = _pipeline.getRenderPass();

    API::CommandBuffer::CmdBeginRenderPass beginRenderPass{
        /* beginRenderPass.framebuffer */ frameData.framebuffer,
        /* beginRenderPass.renderArea */{},
        /* beginRenderPass.clearValues */{}
    };

    beginRenderPass.renderArea.offset = { 0, 0 };
    beginRenderPass.renderArea.extent = { _window.getWidth(), _window.getHeight() };

    frameData.commandBuffer.beginRenderPass(*renderPass, beginRenderPass);

    const API::CommandBuffer::CmdBindDescriptors cmdDescriptorSet{
        /* cameraBind.pipelineLayout */ *_pipeline.getLayout(),
        /* lightBind.pipelineBindPoint */ VK_PIPELINE_BIND_POINT_GRAPHICS,
        /* lightBind.firstSet */ 0,
        /* lightBind.descriptorSets */{ &_descriptorSet },
        /* lightBind.dynamicOffsets */{ },
    };

    frameData.commandBuffer.bindDescriptorSets(cmdDescriptorSet);
    frameData.commandBuffer.bindPipeline(_pipeline);

    if (static_cast<VkBuffer>(frameData.vertexBuffer) != VK_NULL_HANDLE) {
        frameData.commandBuffer.bindVertexBuffers({&frameData.vertexBuffer}, {0});
    }

    if (static_cast<VkBuffer>(frameData.indexBuffer) != VK_NULL_HANDLE) {
        frameData.commandBuffer.bindIndexBuffer(frameData.indexBuffer, VK_INDEX_TYPE_UINT16);
    }

    // UI scale and translate via push constants
    const PushConstBlock pushConstants{
        /* pushConstants.scale */ {2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y},
        /* pushConstants.translate */ {-1.0f, -1.0f}
    };
    const API::CommandBuffer::CmdPushConstants cmdPushConstants{
        /* cmdPushConstants.layout */ static_cast<VkPipelineLayout>(*_pipeline.getLayout()),
        /* cmdPushConstants.stageFlags */ VK_SHADER_STAGE_VERTEX_BIT,
        /* cmdPushConstants.offset */ 0,
        /* cmdPushConstants.size */ sizeof(pushConstants),
        /* cmdPushConstants.values */ &pushConstants
    };
    frameData.commandBuffer.pushConstants(cmdPushConstants);

    // Render commands
    ImDrawData* imDrawData = ImGui::GetDrawData();

    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;

    for (int32_t i = 0; i < imDrawData->CmdListsCount; i++) {
        const ImDrawList* cmd_list = imDrawData->CmdLists[i];
        for (int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[j];

            const VkRect2D scissor{
                /* scissor.offset */ {
                    std::max(static_cast<int32_t>(pcmd->ClipRect.x), 0),
                    std::max(static_cast<int32_t>(pcmd->ClipRect.y), 0)},
                /* scissor.extent */ {
                    static_cast<uint32_t>(pcmd->ClipRect.z - pcmd->ClipRect.x),
                    static_cast<uint32_t>(pcmd->ClipRect.w - pcmd->ClipRect.y)
                }
            };
            frameData.commandBuffer.setScissor({scissor});

            // Update texture descriptor
            // pcmd->TextureId can be io.Fonts->TexID or
            // a texture id passed in ImGui images functions (eg. ImGui::Image())
            VkDescriptorImageInfo descriptorImageInfo;
            descriptorImageInfo.sampler = static_cast<VkSampler>(_fontSampler);
            descriptorImageInfo.imageView = (VkImageView)(intptr_t)pcmd->TextureId;
            descriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            _descriptorSet.updateImages(0, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, {descriptorImageInfo});

            const API::CommandBuffer::CmdDrawIndexed cmdDrawIndexed {
                /* cmdDrawIndexed.indexCount */ pcmd->ElemCount,
                /* cmdDrawIndexed.instanceCount */ 1,
                /* cmdDrawIndexed.firstIndex */ indexCount,
                /* cmdDrawIndexed.vertexOffset */ vertexCount,
                /* cmdDrawIndexed.firstInstance */ 0
            };
            frameData.commandBuffer.drawIndexed(cmdDrawIndexed);

            indexCount += pcmd->ElemCount;
        }
        vertexCount += cmd_list->VtxBuffer.Size;
    }

    frameData.commandBuffer.endRenderPass();
    frameData.commandBuffer.end();

    std::vector<VkPipelineStageFlags> waitDstStageMasks(waitSemaphores.size(), VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT);
    if (_graphicQueue->submit(frameData.commandBuffer, { static_cast<VkSemaphore>(frameData.semaphore) }, waitSemaphores, waitDstStageMasks, static_cast<VkFence>(frameData.fence)) == false) {
        LUG_LOG.error("GUI: Can't submit commandBuffer");
        return false;
    }

    return true;
}

void Gui::processEvent(const lug::Window::Event event) {
   
    ImGuiIO& io = ImGui::GetIO();
    switch (event.type) {
        case lug::Window::Event::Type::KeyPressed:
        case lug::Window::Event::Type::KeyReleased:
            io.KeysDown[static_cast<int>(event.key.code)] = (event.type == lug::Window::Event::Type::KeyPressed) ? true : false;

            io.KeyCtrl = static_cast<int>(event.key.ctrl);
            io.KeyShift = static_cast<int>(event.key.shift);
            io.KeyAlt = static_cast<int>(event.key.alt);
            io.KeySuper = static_cast<int>(event.key.system);
            break;
        case lug::Window::Event::Type::CharEntered:
            if (isprint(event.character.val)) {
                io.AddInputCharacter(static_cast<unsigned short>(event.character.val));
            }
            break;
        case lug::Window::Event::Type::MouseWheel:
            io.MouseWheel += static_cast<float>(event.mouse.scrollOffset.xOffset);
            break;
         case lug::Window::Event::Type::TouchScreenChange:
        
            if (event.touchScreen.drag) {

                float draggedDistance = sqrtf(((event.touchScreen.coordinates[0].x()  - io.MousePosPrev.x ) * (event.touchScreen.coordinates[0].x()  - io.MousePosPrev.x))
                + ((event.touchScreen.coordinates[0].y()  - io.MousePosPrev.y) * (event.touchScreen.coordinates[0].y()  - io.MousePosPrev.y)));
                LUG_LOG.info("draggedDistance {}, displaysize {}", draggedDistance, io.DisplaySize.y);
                if ((static_cast<float>(io.MousePosPrev.y) - event.touchScreen.coordinates[0].y()) > 0) {
                    io.MouseWheel -= draggedDistance / (io.DisplaySize.y - static_cast<float>(io.MousePosPrev.y));
                } else {
                    io.MouseWheel += draggedDistance / (io.DisplaySize.y - static_cast<float>(io.MousePosPrev.y));
                }
                LUG_LOG.info("io.MouseWheel{}", io.MouseWheel);
            }
            break;
        case lug::Window::Event::Type::ButtonPressed:
        case lug::Window::Event::Type::ButtonReleased:
            switch (event.mouse.code) {
            case lug::Window::Mouse::Button::Left:
                io.MouseDown[0] = (event.type == lug::Window::Event::Type::ButtonPressed) ? true : false;
                break;
            case lug::Window::Mouse::Button::Right:
                io.MouseDown[1] = (event.type == lug::Window::Event::Type::ButtonPressed) ? true : false;
                break;
            case lug::Window::Mouse::Button::Middle:
                io.MouseDown[2] = (event.type == lug::Window::Event::Type::ButtonPressed) ? true : false;
                break;
            case lug::Window::Mouse::Button::XButton1:
                io.MouseDown[3] = (event.type == lug::Window::Event::Type::ButtonPressed) ? true : false;
                break;
            case lug::Window::Mouse::Button::XButton2:
                io.MouseDown[4] = (event.type == lug::Window::Event::Type::ButtonPressed) ? true : false;
                break;
            default:
                break;
            }
        default:
        io.MouseWheel = 0;
            break;
    }
}

const Vulkan::API::Semaphore& Gui::getSemaphore(uint32_t currentImageIndex) const {
    return _framesData[currentImageIndex].semaphore;
}

bool Gui::updateBuffers(uint32_t currentImageIndex) {
    ImDrawData* imDrawData = ImGui::GetDrawData();
    if (!imDrawData) {
        LUG_LOG.error("Gui::updateBuffers: Failed to retrieve draw data from imgui");
        return false;
    }

    if (imDrawData->TotalVtxCount == 0 || imDrawData->TotalIdxCount == 0) {
        return true;
    }

    API::Device &device = _renderer.getDevice();
    FrameData& frameData = _framesData[currentImageIndex];

    // Note: Alignment is done inside buffer creation
    const VkDeviceSize vertexBufferSize = imDrawData->TotalVtxCount * sizeof(ImDrawVert);
    const VkDeviceSize indexBufferSize = imDrawData->TotalIdxCount * sizeof(ImDrawIdx);

    // Update buffers only if vertex or index count has been changed compared to current buffer size
    {
        // Vertex buffer
        if ((static_cast<VkBuffer>(frameData.vertexBuffer) == VK_NULL_HANDLE) || (vertexBufferSize > frameData.vertexBuffer.getRequirements().size)) {
            {
                // Create Vertex buffer
                {
                    API::Builder::Buffer bufferBuilder(device);
                    bufferBuilder.setQueueFamilyIndices({ _transferQueue->getQueueFamily()->getIdx() });
                    bufferBuilder.setSize(vertexBufferSize);
                    bufferBuilder.setUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

                    VkResult result{VK_SUCCESS};
                    if (!bufferBuilder.build(frameData.vertexBuffer, &result)) {
                        LUG_LOG.error("Gui::updateBuffers: Can't create staging buffer: {}", result);
                        return false;
                    }
                }

                if (frameData.vertexMemoryPtr) {
                    frameData.vertexMemory.unmap();
                }

                API::Builder::DeviceMemory deviceMemoryBuilder(device);
                deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
                deviceMemoryBuilder.addBuffer(frameData.vertexBuffer);

                VkResult result{VK_SUCCESS};
                if (!deviceMemoryBuilder.build(frameData.vertexMemory, &result)) {
                    LUG_LOG.error("Gui::updateBuffers: Can't create staging buffer device memory: {}", result);
                    return false;
                }

                frameData.vertexMemoryPtr = frameData.vertexMemory.mapBuffer(frameData.vertexBuffer);
            }
        }

        // IndexBuffer
        if ((static_cast<VkBuffer>(frameData.indexBuffer) == VK_NULL_HANDLE) || (indexBufferSize > frameData.indexBuffer.getRequirements().size)) {
            {
                // Create Index buffer
                {
                    API::Builder::Buffer bufferBuilder(device);
                    bufferBuilder.setQueueFamilyIndices({ _transferQueue->getQueueFamily()->getIdx() });
                    bufferBuilder.setSize(indexBufferSize);
                    bufferBuilder.setUsage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

                    VkResult result{VK_SUCCESS};
                    if (!bufferBuilder.build(frameData.indexBuffer, &result)) {
                        LUG_LOG.error("Gui::updateBuffers: Can't create staging buffer: {}", result);
                        return false;
                    }
                }

                if (frameData.indexMemoryPtr) {
                    frameData.indexMemory.unmap();
                }

                API::Builder::DeviceMemory deviceMemoryBuilder(device);
                deviceMemoryBuilder.setMemoryFlags(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
                deviceMemoryBuilder.addBuffer(frameData.indexBuffer);

                VkResult result{VK_SUCCESS};
                if (!deviceMemoryBuilder.build(frameData.indexMemory, &result)) {
                    LUG_LOG.error("Gui::updateBuffers: Can't create staging buffer device memory: {}", result);
                    return false;
                }

                frameData.indexMemoryPtr = frameData.indexMemory.mapBuffer(frameData.indexBuffer);
            }
        }
    }

    // Upload data
    {
        ImDrawVert* vertexMemoryPtr = static_cast<ImDrawVert*>(frameData.vertexMemoryPtr);
        ImDrawIdx* indexMemoryPtr = static_cast<ImDrawIdx*>(frameData.indexMemoryPtr);

        for (int n = 0; n < imDrawData->CmdListsCount; n++) {
            const ImDrawList* cmd_list = imDrawData->CmdLists[n];
            memcpy(vertexMemoryPtr, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
            memcpy(indexMemoryPtr, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
            vertexMemoryPtr += cmd_list->VtxBuffer.Size;
            indexMemoryPtr += cmd_list->IdxBuffer.Size;
        }
    }

    return true;
}

bool Gui::initFrameData()
{
    API::Device& device = _renderer.getDevice();

    for (auto& frameData : _framesData) {
        // Build semaphore
        {
            API::Builder::Semaphore semaphoreBuilder(device);

            VkResult result{ VK_SUCCESS };
            if (!semaphoreBuilder.build(frameData.semaphore, &result)) {
                LUG_LOG.error("Gui::initFontsTexture: Can't create semaphore: {}", result);
                return false;
            }
        }

        // Build fence
        {
            API::Builder::Fence fenceBuilder(device);
            fenceBuilder.setFlags(VK_FENCE_CREATE_SIGNALED_BIT); // Signaled state

            VkResult result{ VK_SUCCESS };
            if (!fenceBuilder.build(frameData.fence, &result)) {
                LUG_LOG.error("Gui::initFontsTexture: Can't create fence: {}", result);
                return false;
            }
        }
    }

    return true;
}

} // Vulkan
} // Graphics
} // lug
