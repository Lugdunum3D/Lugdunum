#include <lug/Graphics/ModelInstance.hpp>

namespace lug {
namespace Graphics {

ModelInstance::ModelInstance(const std::string& name, Model* model) : MovableObject(name, MovableObject::Type::MODEL), _model(model)
{
    auto& meshs = _model->getMeshs();
    for (const auto& mesh : meshs) {
        _meshsInstances.push_back(std::make_unique<MeshInstance>(mesh->getName(), mesh.get(), this));
    }
}

void ModelInstance::needUpdate() {
    // Do nothing here (called when the parent become dirty)
}

} // Graphics
} // lug
