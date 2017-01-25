#pragma once

#include <vector>
#include <lug/Graphics/MovableObject.hpp>

namespace lug {
namespace Graphics {

class RenderQueue {
public:
    RenderQueue() = default;

    RenderQueue(const RenderQueue&) = delete;
    RenderQueue(RenderQueue&&) = delete;

    RenderQueue& operator=(const RenderQueue&) = delete;
    RenderQueue& operator=(RenderQueue&&) = delete;

    ~RenderQueue() = default;

    void addMovableObject(const MovableObject* object);
    void clear();

private:
    std::size_t _size{0};
    std::vector<const MovableObject*> _queue{4000}; // TODO: Change that
};

} // Graphics
} // lug
