#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/View.hpp>
#include <lug/Graphics/Vulkan/API/DescriptorPool.hpp>
#include <lug/Graphics/Vulkan/API/Device.hpp>
#include <lug/Graphics/Vulkan/API/Queue.hpp>
#include <lug/Graphics/Vulkan/API/Semaphore.hpp>
#include <lug/Graphics/Vulkan/Render/Technique/Technique.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Renderer;

namespace Render {

class LUG_GRAPHICS_API View final : public ::lug::Graphics::Render::View {
public:
    // TODO: Take the number of images and the list of images
    View(const Renderer& renderer, const ::lug::Graphics::Render::Target* renderTarget);

    View(const View&) = delete;
    View(View&&) = delete;

    View& operator=(const View&) = delete;
    View& operator=(View&&) = delete;

    ~View() = default;

    bool init(View::InitInfo& initInfo,
                const API::Device* device,
                API::Queue* presentQueue,
                API::DescriptorPool* descriptorPool,
                const std::vector<std::unique_ptr<API::ImageView>>& imageViews);

    bool render(const API::Semaphore& imageReadySemaphore, uint32_t currentImageIndex);
    void destroy() override final;
    bool endFrame() override final;

    Technique::Technique* getRenderTechnique();
    const API::Semaphore& getDrawCompleteSemaphore(uint32_t currentImageIndex) const;

    VkSurfaceFormatKHR getFormat() const;

    // TODO: Add a method to change the index of the good image to use (change by the render window)
    // TODO: Add the semaphores for the images ready in that class too

private:
    const Renderer& _renderer;
    std::unique_ptr<Technique::Technique> _renderTechnique{nullptr};

    std::vector<API::Semaphore> _drawCompleteSemaphores;
    API::Queue* _presentQueue;
};

#include <lug/Graphics/Vulkan/Render/View.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
