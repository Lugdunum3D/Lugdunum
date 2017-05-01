#include <lug/Graphics/Render/Material.hpp>

namespace lug {
namespace Graphics {
namespace Render {

Material::Material(const std::string& name): _name(name) {
    (void)_baseColorTexture;
    (void)_metallicRoughnessTexture;
    (void)_normalTexture;
    (void)_occlusionTexture;
    (void)_emissiveTexture;
    (void)_factors;
}


} // Render
} // Graphics
} // lug
