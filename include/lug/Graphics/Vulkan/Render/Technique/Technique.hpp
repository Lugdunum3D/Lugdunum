#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/ImageView.hpp>

namespace lug {
namespace Graphics {

namespace Render {
class Queue;
} // Render

namespace Vulkan {

namespace API {
class DescriptorPool;
class Semaphore;
} // API

class Renderer;

namespace Render {

class View;

namespace Technique {

class LUG_GRAPHICS_API Technique {
public:
    Technique(const Renderer& renderer, const View& renderView);

    Technique(const Technique&) = delete;
    Technique(Technique&&) = delete;

    Technique& operator=(const Technique&) = delete;
    Technique& operator=(Technique&&) = delete;

    virtual ~Technique() = default;

    virtual bool render(const ::lug::Graphics::Render::Queue& renderQueue, const API::Semaphore& imageReadySemaphore, const API::Semaphore& drawCompleteSemaphore, uint32_t currentImageIndex) = 0;
    virtual bool init(API::DescriptorPool* descriptorPool, const std::vector<API::ImageView>& imageViews) = 0;
    virtual void destroy() = 0;

    virtual bool initDepthBuffers(const std::vector<API::ImageView>& imageViews) = 0;
    virtual bool initFramebuffers(const std::vector<API::ImageView>& imageViews) = 0;

protected:
    const Renderer& _renderer;
    const View& _renderView;
};

} // Technique
} // Render
} // Vulkan
} // Graphics
} // lug
