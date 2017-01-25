#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/RenderTechnique.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class LUG_GRAPHICS_API ForwardRenderTechnique final : public RenderTechnique {
public:
    ForwardRenderTechnique(const RenderView* renderView);

    ForwardRenderTechnique(const ForwardRenderTechnique&) = delete;
    ForwardRenderTechnique(ForwardRenderTechnique&&) = delete;

    ForwardRenderTechnique& operator=(const ForwardRenderTechnique&) = delete;
    ForwardRenderTechnique& operator=(ForwardRenderTechnique&&) = delete;

    ~ForwardRenderTechnique() = default;

    void render(const RenderQueue& renderQueue) override final;
};

} // Vulkan
} // Graphics
} // lug
