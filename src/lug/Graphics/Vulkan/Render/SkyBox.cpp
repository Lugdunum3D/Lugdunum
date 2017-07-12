#include <lug/Graphics/Vulkan/Render/SkyBox.hpp>

#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

API::GraphicsPipeline SkyBox::_pipeline;
lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Mesh> SkyBox::_mesh;
uint32_t SkyBox::_skyBoxCount{0};

SkyBox::SkyBox(const std::string& name) : ::lug::Graphics::Render::SkyBox(name) {}

SkyBox::~SkyBox() {
    destroy();
}

void SkyBox::destroy() {
    --_skyBoxCount;
    if (_skyBoxCount == 0) {
        SkyBox::_pipeline.destroy();
        // TODO(nokitoo): destroy SkyBox::_mesh (Can't delete resources in resource manager yet)
    }
}

} // Render
} // Vulkan
} // Graphics
} // lug
