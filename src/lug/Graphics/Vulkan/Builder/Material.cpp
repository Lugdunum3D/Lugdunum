#include <lug/Graphics/Vulkan/Builder/Material.hpp>

#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Graphics.hpp>
#include <lug/Graphics/Render/Material.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Builder {

Resource::SharedPtr<::lug::Graphics::Render::Material> Material::build() {
    // Constructor of Material is private, we can't use std::make_unique
    std::unique_ptr<Resource> resource{new Vulkan::Render::Material(_name)};
    Vulkan::Render::Material* material = static_cast<Vulkan::Render::Material*>(resource.get());

    material->_name = _name;
    material->_factors = _factors;
    material->_baseColorTexture = _baseColorTexture;
    material->_metallicRoughnessTexture = _metallicRoughnessTexture;
    material->_normalTexture = _normalTexture;
    material->_occlusionTexture = _occlusionTexture;
    material->_emissiveTexture = _emissiveTexture;

    // TODO(Nokitoo): load vulkan textures

    return _renderer.getResourceManager()->add<::lug::Graphics::Render::Material>(std::move(resource));
}

} // Builder
} // Vulkan
} // Graphics
} // lug
