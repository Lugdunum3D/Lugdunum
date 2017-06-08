#pragma once

#include <utility>
#include <vector>

#include <lug/Graphics/Scene/Node.hpp>

namespace lug {
namespace Graphics {

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

    void addMeshInstance(Scene::Node& node, const Scene::Node::MeshInstance& meshInstance);
    void addLight(Scene::Node& node, Resource::SharedPtr<Render::Light> light);
    void clear();
    void removeDirtyProperty();

    const std::vector<std::pair<Scene::Node*, Scene::Node::MeshInstance>>& getMeshs() const;
    std::size_t getMeshsNb() const;

    const std::vector<std::pair<Scene::Node*, Resource::SharedPtr<Render::Light>>>& getLights() const;
    std::size_t getLightsNb() const;

private:
    std::size_t _meshsNb{0};
    // TODO(nokitoo): store pointers to MeshInstance ?
    // (Need to change Node MeshsInstance vector to list)
    std::vector<std::pair<Scene::Node*, Scene::Node::MeshInstance>> _meshs{4000}; // TODO: Change that

    std::vector<std::pair<Scene::Node*, Resource::SharedPtr<Render::Light>>> _lights{50}; // TODO: Change that
    std::size_t _lightsNb{0};
};

#include <lug/Graphics/Render/Queue.inl>

} // Render
} // Graphics
} // lug
