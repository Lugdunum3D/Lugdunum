#pragma once

#include <array>
#include <memory>
#include <vector>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/DeviceMemory.hpp>
#include <lug/Graphics/Vulkan/API/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/Graphics/Vulkan/API/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/Sampler.hpp>
#include <lug/Graphics/Vulkan/API/Semaphore.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Renderer;

namespace Render {

namespace DescriptorSetPool {
    class BloomSampler;
    class DescriptorSet;
};

class Window;

class LUG_GRAPHICS_API BloomPass {
private:
    struct BlurPass {
        // We need 2 passes to blur : one vertical blur, one horizontal blur
        // Each pass has different color attachment
        std::array<API::Framebuffer, 2> framebuffers;
        std::array<API::Sampler, 2> samplers;
        std::array<API::Image, 2> images;
        std::array<API::ImageView, 2> imagesViews;
    };

    struct BlendPass {
        API::Framebuffer framebuffer;
        API::Image image;
        API::ImageView imageView;
        API::Sampler sampler;
    };

    struct FrameData {
        API::Semaphore glowCopyFinishedSemaphore{};
        API::Semaphore blurFinishedSemaphore{};
        API::Semaphore bloomFinishedSemaphores{};

        std::vector<API::CommandBuffer> transferCmdBuffers;
        API::CommandBuffer graphicsCmdBuffer;

        std::vector<BlurPass> blurPasses;
        BlendPass blendPass;
        API::Fence fence;

        API::Framebuffer hdrFramebuffer;

        bool freeDescriptorSets{false};

        std::vector<const Render::DescriptorSetPool::DescriptorSet*> texturesDescriptorSets;
    };

public:
    BloomPass(lug::Graphics::Vulkan::Renderer& renderer, lug::Graphics::Vulkan::Render::Window& window);

    BloomPass(const BloomPass&) = delete;
    BloomPass(BloomPass&&) = delete;

    BloomPass& operator=(const BloomPass&) = delete;
    BloomPass& operator=(BloomPass&&) = delete;

    ~BloomPass();

    bool init();

    void destroy();

    bool endFrame(const std::vector<VkSemaphore>& waitSemaphores, uint32_t currentImageIndex);

    const Vulkan::API::Semaphore& getSemaphore(uint32_t currentImageIndex) const;

    bool initBlurPass();
    bool buildEndCommandBuffer();

private:
    bool renderBlurPass(uint32_t currentImageIndex);
    bool initHdrPipeline();
    bool initBlendPipeline();
    bool initBlurPipeline(API::GraphicsPipeline& pipeline, int blurDirection);
    bool initPipelines();

private:
    lug::Graphics::Vulkan::Renderer& _renderer;
    lug::Graphics::Vulkan::Render::Window& _window;

    const API::Queue* _transferQueue{nullptr};
    const API::Queue* _graphicsQueue{nullptr};

    std::vector<FrameData> _framesData;

    API::CommandPool _graphicsCommandPool;
    API::CommandPool _transferQueueCommandPool;

    API::DeviceMemory _offscreenImagesMemory;

    API::GraphicsPipeline _horizontalPipeline;
    API::GraphicsPipeline _verticalPipeline;
    API::GraphicsPipeline _blendPipeline;
    API::GraphicsPipeline _hdrPipeline;

    std::unique_ptr<Render::DescriptorSetPool::BloomSampler> _texturesDescriptorSetPool;
};

} // Render
} // Vulkan
} // Graphics
} // lug
