#pragma once

#include <lug/Graphics/Render/Material.hpp>
#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {

namespace Builder {
class Material;
} // Builder

namespace Vulkan {
namespace Builder {
namespace Material {

Resource::SharedPtr<lug::Graphics::Render::Material> build(const ::lug::Graphics::Builder::Material& builder);

} // Material
} // Builder
} // Vulkan
} // Graphics
} // lug
