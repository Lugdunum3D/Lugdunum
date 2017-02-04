#pragma once

#include <vector>

namespace lug {
namespace Graphics {

class Light;
class MovableObject;

class RenderQueue {
public:
    RenderQueue() = default;

    RenderQueue(const RenderQueue&) = delete;
    RenderQueue(RenderQueue&&) = delete;

    RenderQueue& operator=(const RenderQueue&) = delete;
    RenderQueue& operator=(RenderQueue&&) = delete;

    ~RenderQueue() = default;

    void addMovableObject(MovableObject* object);
    void clear();

    const std::vector<MovableObject*>& getObjects() const;
    std::size_t getObjectsNb() const;

    const std::vector<Light*>& getLights() const;
    std::size_t getLightsNb() const;

private:
    std::size_t _size{0};
    std::vector<MovableObject*> _queue{4000}; // TODO: Change that

    std::vector<Light*> _lightsQueue{50}; // TODO: Change that
    std::size_t _lightsNb{0};
};

#include <lug/Graphics/RenderQueue.inl>

} // Graphics
} // lug
