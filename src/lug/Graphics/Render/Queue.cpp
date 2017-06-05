#include <lug/Graphics/Render/Queue.hpp>
#include <cstring>
#include <lug/Graphics/Render/Light.hpp>
#include <lug/Graphics/Scene/MeshInstance.hpp>
#include <lug/Graphics/Scene/MovableObject.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Render {

void Queue::addMovableObject(Scene::MovableObject* object) {
    if (!object) {
        return;
    }

    /*if (object->getType() == Scene::MovableObject::Type::Light) {
        _lights[_lightsNb] = static_cast<Render::Light*>(object);
        ++_lightsNb;
        return;
    }*/ if (object->getType() == Scene::MovableObject::Type::Mesh) {
        _meshs[_meshsNb] = static_cast<Scene::MeshInstance*>(object);
        ++_meshsNb;
        return;
    } else if (object->getType() != Scene::MovableObject::Type::Camera) {
        LUG_LOG.warn("Queue::addMovableObject: Unknow object type");
    }
}

void Queue::clear() {
    std::memset(_meshs.data(), 0, _meshsNb * sizeof(void*));
    _meshsNb = 0;

    std::memset(_lights.data(), 0, _lightsNb * sizeof(void*));
    _lightsNb = 0;
}

void Queue::removeDirtyProperty() {
    for (std::size_t i = 0; i < _meshsNb; ++i) {
        _meshs[i]->isDirty(false);
    }
}

} // Render
} // Graphics
} // lug
