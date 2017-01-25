#pragma once

#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {

class RenderQueue;

namespace Vulkan {

class RenderView;

class LUG_GRAPHICS_API RenderTechnique {
public:
    RenderTechnique(const RenderView* renderView);

    RenderTechnique(const RenderTechnique&) = delete;
    RenderTechnique(RenderTechnique&&) = delete;

    RenderTechnique& operator=(const RenderTechnique&) = delete;
    RenderTechnique& operator=(RenderTechnique&&) = delete;

    virtual ~RenderTechnique() = default;

    virtual void render(const RenderQueue& renderQueue) = 0;

protected:
    const RenderView* _renderView;
};

} // Vulkan
} // Graphics
} // lug
