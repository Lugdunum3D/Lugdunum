#include <lug/Graphics/Render/Material.hpp>

namespace lug {
namespace Graphics {
namespace Render {

Material::Material(const std::string& name): Resource(Resource::Type::Material), _name(name) {
    (void)_baseColorTexture;
    (void)_metallicRoughnessTexture;
    (void)_normalTexture;
    (void)_occlusionTexture;
    (void)_emissiveTexture;
    (void)_constants;
}


} // Render
} // Graphics
} // lug
