#include <lug/Graphics/MeshInstance.hpp>

namespace lug {
namespace Graphics {

MeshInstance::MeshInstance(const std::string& name, Mesh* mesh) : MovableObject(name), _mesh(mesh) {}

void MeshInstance::needUpdate() {
    // Do nothing here (called when the parent become dirty)
}

} // Graphics
} // lug
