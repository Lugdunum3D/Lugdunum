#include <lug/Graphics/RenderQueue.hpp>
#include <cstring>

namespace lug {
namespace Graphics {

void RenderQueue::addMovableObject(MovableObject* object) {
    _queue[_size] = object;
    ++_size;
}

void RenderQueue::clear() {
    std::memset(_queue.data(), 0, _size * sizeof(void*));
    _size = 0;
}

} // Graphics
} // lug
