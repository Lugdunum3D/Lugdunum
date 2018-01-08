#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/API/ImageView.hpp>
#include <lug/Graphics/Vulkan/Render/Queue.hpp>

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
    Technique(Renderer& renderer, View& renderView);

    Technique(const Technique&) = delete;
    Technique(Technique&&) = delete;

    Technique& operator=(const Technique&) = delete;
    Technique& operator=(Technique&&) = delete;

    virtual ~Technique() = default;

    virtual bool render(
        const Render::Queue& renderQueue,
        const API::Semaphore& imageReadySemaphore,
        const API::Semaphore& drawCompleteSemaphore,
        uint32_t currentImageIndex
    ) = 0;

    virtual bool init(const std::vector<API::ImageView>& imageViews) = 0;
    virtual void destroy() = 0;

    virtual bool setSwapchainImageViews(const std::vector<API::ImageView>& imageViews) = 0;
    virtual bool initFrameDatas(const std::vector<API::ImageView>& imageViews) = 0;

protected:
    Renderer& _renderer;
    View& _renderView;
};

} // Technique
} // Render
} // Vulkan
} // Graphics
} // lug
