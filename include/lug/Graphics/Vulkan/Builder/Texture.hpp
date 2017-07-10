#pragma once

#include <lug/Graphics/Render/Texture.hpp>
#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {

namespace Builder {
class Texture;
} // Builder

namespace Vulkan {
namespace Builder {
namespace Texture {

Resource::SharedPtr<lug::Graphics::Render::Texture> build(const ::lug::Graphics::Builder::Texture& builder);

} // Texture
} // Builder
} // Vulkan
} // Graphics
} // lug
