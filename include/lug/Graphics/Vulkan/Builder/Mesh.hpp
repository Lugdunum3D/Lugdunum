#pragma once

#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {

namespace Builder {
class Mesh;
} // Builder

namespace Vulkan {
namespace Builder {
namespace Mesh {

Resource::SharedPtr<lug::Graphics::Render::Mesh> LUG_GRAPHICS_API build(const ::lug::Graphics::Builder::Mesh& builder);

} // Mesh
} // Builder
} // Vulkan
} // Graphics
} // lug
