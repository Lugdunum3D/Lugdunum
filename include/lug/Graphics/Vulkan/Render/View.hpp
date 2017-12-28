#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/View.hpp>
#include <lug/Graphics/Vulkan/API/Semaphore.hpp>
#include <lug/Graphics/Vulkan/Render/Queue.hpp>
#include <lug/Graphics/Vulkan/Render/Technique/Technique.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

namespace API {
class DescriptorPool;
class Queue;
} // API

class Renderer;

namespace Render {

class LUG_GRAPHICS_API View final : public ::lug::Graphics::Render::View {
public:
    // TODO: Take the number of images and the list of images
    View(Renderer& renderer, const ::lug::Graphics::Render::Target* renderTarget);

    View(const View&) = delete;
    View(View&&) = delete;

    View& operator=(const View&) = delete;
    View& operator=(View&&) = delete;

    ~View() = default;

    bool init(View::InitInfo& initInfo,
                const API::Queue* presentQueue,
                const std::vector<API::ImageView>& sceneImageViews,
                const std::vector<API::ImageView>& glowImageViews);

    bool render(const API::Semaphore& imageReadySemaphore, uint32_t currentImageIndex);
    void destroy() override final;
    bool endFrame() override final;

    Technique::Technique* getRenderTechnique();
    const API::Semaphore& getDrawCompleteSemaphore(uint32_t currentImageIndex) const;

    VkSurfaceFormatKHR getFormat() const;

    // TODO: Add a method to change the index of the good image to use (change by the render window)
    // TODO: Add the semaphores for the images ready in that class too

private:
    Renderer& _renderer;
    std::unique_ptr<Technique::Technique> _renderTechnique{nullptr};

    std::vector<API::Semaphore> _drawCompleteSemaphores;
    const API::Queue* _presentQueue;

    Render::Queue _renderQueue;
};

#include <lug/Graphics/Vulkan/Render/View.inl>

} // Render
} // Vulkan
} // Graphics
} // lug
