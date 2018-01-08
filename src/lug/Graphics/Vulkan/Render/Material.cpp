#include <lug/Graphics/Vulkan/Render/Material.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Material::Material(const std::string& name) : ::lug::Graphics::Render::Material(name) {

}

Material::~Material() {
    destroy();
}

void Material::destroy() {

}

Pipeline::Id::Model::MaterialPart Material::getPipelineId() {
    return _pipelineIdMaterialPart;
}

} // Render
} // Vulkan
} // Graphics
} // lug
