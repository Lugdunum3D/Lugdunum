#pragma once

#include <vector>

namespace lug {
namespace Graphics {

class Light;
class MeshInstance;
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
    void removeDirtyProperty();

    const std::vector<MeshInstance*>& getMeshs() const;
    std::size_t getMeshsNb() const;

    const std::vector<Light*>& getLights() const;
    std::size_t getLightsNb() const;

private:
    std::size_t _meshsNb{0};
    std::vector<MeshInstance*> _meshs{4000}; // TODO: Change that

    std::vector<Light*> _lights{50}; // TODO: Change that
    std::size_t _lightsNb{0};
};

#include <lug/Graphics/RenderQueue.inl>

} // Graphics
} // lug
