#include <lug/Graphics/Render/Queue.hpp>
#include <cstring>
#include <lug/Graphics/Render/Light.hpp>
#include <lug/System/Logger/Logger.hpp>

namespace lug {
namespace Graphics {
namespace Render {

void Queue::addMeshInstance(Scene::Node& node) {
    _meshs[_meshsNb] = &node;
    ++_meshsNb;
}

void Queue::addLight(Scene::Node& node) {
    _lights[_lightsNb] = &node;
    ++_lightsNb;
}

void Queue::clear() {
    std::memset(_meshs.data(), 0, _meshsNb * sizeof(void*));
    _meshsNb = 0;

    std::memset(_lights.data(), 0, _lightsNb * sizeof(void*));
    _lightsNb = 0;
}

void Queue::removeDirtyProperty() {
/*    for (std::size_t i = 0; i < _meshsNb; ++i) {
        _meshs[i]->isDirty(false);
    }*/
}

} // Render
} // Graphics
} // lug
