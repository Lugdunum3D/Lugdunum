#include <lug/Graphics/MeshInstance.hpp>

namespace lug {
namespace Graphics {

MeshInstance::MeshInstance(const std::string& name, Mesh* mesh) : MovableObject(name), _mesh(mesh) {}

MeshInstance::~MeshInstance() {
    // TODO: Remove this, the MeshInstance should not delete the mesh, it's not the owner !
   /* if (_mesh) {
        delete _mesh;
    }*/
}

void MeshInstance::needUpdate() {
    // Do nothing here (called when the parent become dirty)
}

} // Graphics
} // lug
