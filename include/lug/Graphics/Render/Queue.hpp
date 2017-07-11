#pragma once

#include <lug/Graphics/Render/SkyBox.hpp>
#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {
namespace Scene {
class Node;
}

namespace Render {

class Queue {
public:
    Queue() = default;

    Queue(const Queue&) = delete;
    Queue(Queue&&) = delete;

    Queue& operator=(const Queue&) = delete;
    Queue& operator=(Queue&&) = delete;

    ~Queue() = default;

    virtual void addMeshInstance(Scene::Node& node) = 0;
    virtual void addLight(Scene::Node& node) = 0;
    virtual void addSkyBox(Resource::SharedPtr<Render::SkyBox> skyBox) = 0;
    virtual void clear() = 0;
};

} // Render
} // Graphics
} // lug
