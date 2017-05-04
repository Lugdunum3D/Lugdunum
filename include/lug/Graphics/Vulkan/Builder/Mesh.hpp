#pragma once

#include <lug/Graphics/Builder/Mesh.hpp>
#include <lug/Graphics/Vulkan/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Builder {

class LUG_GRAPHICS_API Mesh final : public lug::Graphics::Builder::Mesh {
public:
    explicit Mesh(lug::Graphics::Renderer& renderer);

    Mesh(const Mesh&) = default;
    Mesh(Mesh&&) = default;

    Mesh& operator=(const Mesh&) = default;
    Mesh& operator=(Mesh&&) = default;

    ~Mesh() = default;

    Resource::SharedPtr<lug::Graphics::Render::Mesh> build() override final;
};

} // Builder
} // Vulkan
} // Graphics
} // lug
