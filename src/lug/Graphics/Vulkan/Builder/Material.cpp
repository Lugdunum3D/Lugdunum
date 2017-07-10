#include <lug/Graphics/Vulkan/Builder/Material.hpp>

#include <lug/Graphics/Builder/Material.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/Render/Material.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Builder {
namespace Material {

Resource::SharedPtr<::lug::Graphics::Render::Material> build(const ::lug::Graphics::Builder::Material& builder) {
    // Constructor of Material is private, we can't use std::make_unique
    std::unique_ptr<Resource> resource{new Vulkan::Render::Material(builder._name)};
    Vulkan::Render::Material* material = static_cast<Vulkan::Render::Material*>(resource.get());

    material->_name = builder._name;
    material->_constants = builder._constants;
    material->_baseColorTexture = builder._baseColorTexture;
    material->_metallicRoughnessTexture = builder._metallicRoughnessTexture;
    material->_normalTexture = builder._normalTexture;
    material->_occlusionTexture = builder._occlusionTexture;
    material->_emissiveTexture = builder._emissiveTexture;

    material->_pipelineIdMaterialPart.baseColorInfo = material->_baseColorTexture.texture ? material->_baseColorTexture.texCoord : 0b11;
    material->_pipelineIdMaterialPart.metallicRoughnessInfo = material->_metallicRoughnessTexture.texture ? material->_metallicRoughnessTexture.texCoord : 0b11;
    material->_pipelineIdMaterialPart.normalInfo = material->_normalTexture.texture ? material->_normalTexture.texCoord : 0b11;
    material->_pipelineIdMaterialPart.occlusionInfo = material->_occlusionTexture.texture ? material->_occlusionTexture.texCoord : 0b11;
    material->_pipelineIdMaterialPart.emissiveInfo = material->_emissiveTexture.texture ? material->_emissiveTexture.texCoord : 0b11;

    return builder._renderer.getResourceManager()->add<::lug::Graphics::Render::Material>(std::move(resource));
}

} // Material
} // Builder
} // Vulkan
} // Graphics
} // lug
