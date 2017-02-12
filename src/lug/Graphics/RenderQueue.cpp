#include <cstring>
#include <lug/Graphics/Light.hpp>
#include <lug/Graphics/MovableObject.hpp>
#include <lug/Graphics/RenderQueue.hpp>

namespace lug {
namespace Graphics {

void RenderQueue::addMovableObject(MovableObject* object) {
    if (object->getType() == MovableObject::Type::LIGHT) {
        _lightsQueue[_lightsNb] = static_cast<Light*>(object);
        ++_lightsNb;
        return;
    }

    _queue[_size] = object;
    ++_size;
}

void RenderQueue::clear() {
    std::memset(_queue.data(), 0, _size * sizeof(void*));
    _size = 0;

    std::memset(_lightsQueue.data(), 0, _lightsNb * sizeof(void*));
    _lightsNb = 0;
}

void RenderQueue::removeDirtyProperty() {
    for (std::size_t i = 0; i < _size; ++i) {
        _queue[i]->isDirty(false);
    }

    for (std::size_t i = 0; i < _lightsNb; ++i) {
        _lightsQueue[i]->isDirty(false);
    }
}

} // Graphics
} // lug
