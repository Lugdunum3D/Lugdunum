#include <cstring>
#include <lug/Graphics/Light.hpp>
#include <lug/Graphics/MeshInstance.hpp>
#include <lug/Graphics/ModelInstance.hpp>
#include <lug/Graphics/MovableObject.hpp>
#include <lug/Graphics/RenderQueue.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {

void RenderQueue::addMovableObject(MovableObject* object) {
    if (!object) {
        return;
    }

    if (object->getType() == MovableObject::Type::LIGHT) {
        _lights[_lightsNb] = static_cast<Light*>(object);
        ++_lightsNb;
        return;
    }
    else if (object->getType() == MovableObject::Type::MESH) {
        _meshs[_meshsNb] = static_cast<MeshInstance*>(object);
        ++_meshsNb;
        return;
    }
    else if (object->getType() == MovableObject::Type::MODEL) {
        ModelInstance* modelInstance = static_cast<ModelInstance*>(object);
        auto& meshsInstances = modelInstance->getMeshsInstances();
        for (auto& meshInstance: meshsInstances) {
            addMovableObject(meshInstance.get());
        }
    }
    else if (object->getType() != MovableObject::Type::CAMERA) {
        LUG_LOG.warn("RenderQueue::addMovableObject: Unknow object type");
    }
}

void RenderQueue::clear() {
    std::memset(_meshs.data(), 0, _meshsNb * sizeof(void*));
    _meshsNb = 0;

    std::memset(_lights.data(), 0, _lightsNb * sizeof(void*));
    _lightsNb = 0;
}

void RenderQueue::removeDirtyProperty() {
    for (std::size_t i = 0; i < _meshsNb; ++i) {
        _meshs[i]->isDirty(false);
    }

    for (std::size_t i = 0; i < _lightsNb; ++i) {
        _lights[i]->isDirty(false);
    }
}

} // Graphics
} // lug
