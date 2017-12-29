#pragma once

#include <vector>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/CommandBuffer.hpp>
#include <lug/Graphics/Vulkan/API/CommandPool.hpp>
#include <lug/Graphics/Vulkan/API/Image.hpp>
#include <lug/Graphics/Vulkan/API/ImageView.hpp>
#include <lug/Graphics/Vulkan/API/Semaphore.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Renderer;

namespace Render {

class Window;

class LUG_GRAPHICS_API BloomPass final {
private:
    struct FrameData {
        API::Semaphore bloomFinishedSemaphores{};
        API::CommandBuffer cmdBuffer;
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

private:
    bool buildEndCommandBuffer();

private:
    lug::Graphics::Vulkan::Renderer& _renderer;
    lug::Graphics::Vulkan::Render::Window& _window;

    const API::Queue* _transferQueue{nullptr};

    std::vector<FrameData> _framesData;

    API::CommandPool _commandPool;
};

} // Render
} // Vulkan
} // Graphics
} // lug
