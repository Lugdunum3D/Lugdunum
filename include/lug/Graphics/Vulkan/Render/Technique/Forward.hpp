#pragma once

#include <unordered_map>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/Fence.hpp>
#include <lug/Graphics/Vulkan/API/Framebuffer.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/Graphics/Vulkan/API/ImageView.hpp>
#include <lug/Graphics/Vulkan/Render/BufferPool/Camera.hpp>
#include <lug/Graphics/Vulkan/Render/BufferPool/Light.hpp>
#include <lug/Graphics/Vulkan/Render/BufferPool/Material.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/Camera.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/Light.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/Material.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/MaterialTextures.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/SkyBox.hpp>
#include <lug/Graphics/Vulkan/Render/Technique/Technique.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

namespace API {
class Queue;
} // API

namespace Render {
namespace Technique {

class LUG_GRAPHICS_API Forward final : public Technique {
private:
    struct DepthBuffer {
        API::Image image;
        API::ImageView imageView;
    };

    struct FrameData {
        DepthBuffer depthBuffer;
        API::Framebuffer framebuffer;

        API::Fence renderFence;
        API::CommandBuffer renderCmdBuffer;

        API::Fence transferFence;
        API::CommandBuffer transferCmdBuffer;
        API::Semaphore transferSemaphore;

        const BufferPool::SubBuffer* cameraBuffer{nullptr};
        std::vector<const BufferPool::SubBuffer*> lightBuffers;
        std::vector<const BufferPool::SubBuffer*> materialBuffers;

        const DescriptorSetPool::DescriptorSet* cameraDescriptorSet{nullptr};
        const DescriptorSetPool::DescriptorSet* skyBoxDescriptorSet{nullptr};
        std::vector<const DescriptorSetPool::DescriptorSet*> lightDescriptorSets;
        std::vector<const DescriptorSetPool::DescriptorSet*> materialDescriptorSets;
        std::vector<const DescriptorSetPool::DescriptorSet*> materialTexturesDescriptorSets;
    };

public:
    Forward(Renderer& renderer, const View& renderView);

    Forward(const Forward&) = delete;
    Forward(Forward&&) = delete;

    Forward& operator=(const Forward&) = delete;
    Forward& operator=(Forward&&) = delete;

    ~Forward();

    bool render(
        const Render::Queue& renderQueue,
        const API::Semaphore& imageReadySemaphore,
        const API::Semaphore& drawCompleteSemaphore,
        uint32_t currentImageIndex
    ) override final;

    bool init(const std::vector<API::ImageView>& imageViews) override final;
    void destroy() override final;

    bool initDepthBuffers(const std::vector<API::ImageView>& imageViews) override final;
    bool initFramebuffers(const std::vector<API::ImageView>& imageViews) override final;

private:
    API::DeviceMemory _depthBufferMemory;

    std::vector<FrameData> _framesData;

    const API::Queue* _graphicsQueue{nullptr};
    API::CommandPool _graphicsCommandPool;

    const API::Queue* _transferQueue{nullptr};
    API::CommandPool _transferCommandPool;

private:
    // TODO: Use shared_ptr in the instance and static weak_ptr to avoid problem when we delete one forward renderer and not the others
    static std::unique_ptr<BufferPool::Camera> _cameraBufferPool;
    static std::unique_ptr<BufferPool::Light> _lightBufferPool;
    static std::unique_ptr<BufferPool::Material> _materialBufferPool;

    static std::unique_ptr<DescriptorSetPool::Camera> _cameraDescriptorSetPool;
    static std::unique_ptr<DescriptorSetPool::Light> _lightDescriptorSetPool;
    static std::unique_ptr<DescriptorSetPool::Material> _materialDescriptorSetPool;
    static std::unique_ptr<DescriptorSetPool::MaterialTextures> _materialTexturesDescriptorSetPool;
    static std::unique_ptr<DescriptorSetPool::SkyBox> _skyBoxDescriptorSetPool;
};

} // Technique
} // Render
} // Vulkan
} // Graphics
} // lug
