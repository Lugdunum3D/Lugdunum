#pragma once

#include <vector>

namespace lug {
namespace Graphics {

namespace Light {
class Light;
} // Light

namespace Scene {
class MeshInstance;
class MovableObject;
} // Scene

namespace Render {

class Queue {
public:
    Queue() = default;

    Queue(const Queue&) = delete;
    Queue(Queue&&) = delete;

    Queue& operator=(const Queue&) = delete;
    Queue& operator=(Queue&&) = delete;

    ~Queue() = default;

    void addMovableObject(Scene::MovableObject* object);
    void clear();
    void removeDirtyProperty();

    const std::vector<Scene::MeshInstance*>& getMeshs() const;
    std::size_t getMeshsNb() const;

    const std::vector<Light::Light*>& getLights() const;
    std::size_t getLightsNb() const;

private:
    std::size_t _meshsNb{0};
    std::vector<Scene::MeshInstance*> _meshs{4000}; // TODO: Change that

    std::vector<Light::Light*> _lights{50}; // TODO: Change that
    std::size_t _lightsNb{0};
};

#include <lug/Graphics/Render/Queue.inl>

} // Render
} // Graphics
} // lug
