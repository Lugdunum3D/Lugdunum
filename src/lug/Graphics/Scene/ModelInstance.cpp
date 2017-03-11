#include <lug/Graphics/Scene/ModelInstance.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

ModelInstance::ModelInstance(const std::string& name, Render::Model* model) : MovableObject(name, MovableObject::Type::Model), _model(model)
{
    auto& meshs = _model->getMeshs();
    for (const auto& mesh : meshs) {
        _meshsInstances.push_back(std::make_unique<MeshInstance>(mesh->getName(), mesh.get(), this));
    }
}

void ModelInstance::needUpdate() {
    // Do nothing here (called when the parent become dirty)
}

} // Scene
} // Graphics
} // lug
