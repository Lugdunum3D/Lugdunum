#include <lug/Graphics/Scene/MeshInstance.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

MeshInstance::MeshInstance(const std::string& name, Render::Mesh* mesh, ModelInstance* modelInstance) :
    MovableObject(name, MovableObject::Type::Mesh), _mesh(mesh), _modelInstance(modelInstance) {}

MeshInstance::~MeshInstance() {
    // TODO: Remove this, the MeshInstance should not delete the mesh, it's not the owner !
   /* if (_mesh) {
        delete _mesh;
    }*/
}

void MeshInstance::needUpdate() {
    // Do nothing here (called when the parent become dirty)
}

} // Scene
} // Graphics
} // lug
