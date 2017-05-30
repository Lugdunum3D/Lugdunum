#pragma once

#include <lug/Graphics/Builder/Material.hpp>
#include <lug/Graphics/Vulkan/Render/Material.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Builder {

class LUG_GRAPHICS_API Material final : public lug::Graphics::Builder::Material {
public:
    explicit Material(lug::Graphics::Renderer& renderer);

    Material(const Material&) = default;
    Material(Material&&) = default;

    Material& operator=(const Material&) = default;
    Material& operator=(Material&&) = default;

    ~Material() = default;

    Resource::SharedPtr<lug::Graphics::Render::Material> build() override final;
};

} // Builder
} // Vulkan
} // Graphics
} // lug
