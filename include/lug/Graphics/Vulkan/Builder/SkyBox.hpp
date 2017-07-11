#pragma once

#include <lug/Graphics/Render/SkyBox.hpp>
#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {

namespace Builder {
class SkyBox;
} // Builder

namespace Vulkan {
namespace Builder {
namespace SkyBox {

Resource::SharedPtr<lug::Graphics::Render::SkyBox> build(const ::lug::Graphics::Builder::SkyBox& builder);

} // SkyBox
} // Builder
} // Vulkan
} // Graphics
} // lug
