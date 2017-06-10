#pragma once

#include <vector>

namespace lug {
namespace Graphics {
namespace Scene {
class Node;
}

namespace Render {

class Light;

class Queue {
public:
    Queue() = default;

    Queue(const Queue&) = delete;
    Queue(Queue&&) = delete;

    Queue& operator=(const Queue&) = delete;
    Queue& operator=(Queue&&) = delete;

    ~Queue() = default;

    void addMeshInstance(Scene::Node& node);
    void addLight(Scene::Node& node);
    void clear();
    void removeDirtyProperty();

    const std::vector<Scene::Node*>& getMeshs() const;
    std::size_t getMeshsNb() const;

    const std::vector<Scene::Node*>& getLights() const;
    std::size_t getLightsNb() const;

private:
    std::size_t _meshsNb{0};
    // (Need to change Node MeshsInstance vector to list)
    std::vector<Scene::Node*> _meshs{4000}; // TODO: Change that

    std::vector<Scene::Node*> _lights{50}; // TODO: Change that
    std::size_t _lightsNb{0};
};

#include <lug/Graphics/Render/Queue.inl>

} // Render
} // Graphics
} // lug
