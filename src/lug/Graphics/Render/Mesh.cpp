#include <lug/Graphics/Render/Mesh.hpp>

namespace lug {
namespace Graphics {
namespace Render {

Mesh::Mesh(const std::string& name) : Resource(Resource::Type::Mesh), _name(name) {}

Mesh::~Mesh() {
    for (auto& primitiveSet : _primitiveSets) {
        for (auto& attribute : primitiveSet.attributes) {
            delete[] attribute.buffer.data;
            attribute.buffer.data = nullptr;
        }
    }
}

} // Render
} // Graphics
} // lug
