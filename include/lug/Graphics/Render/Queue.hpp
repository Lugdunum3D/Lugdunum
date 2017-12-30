#pragma once

#include <lug/Graphics/Render/SkyBox.hpp>
#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {

class Renderer;

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

    /**
     * @brief      Adds a mesh instance to the queue.
     *
     * @param      node      The mesh node
     * @param[in]  renderer  The renderer
     */
    virtual void addMeshInstance(Scene::Node& node, const Renderer& renderer) = 0;
    /**
     * @brief      Adds a light node to the queue.
     *
     * @param      node  The light node
     */
    virtual void addLight(Scene::Node& node) = 0;
    /**
     * @brief      Adds a sky box to the queue.
     *
     * @param[in]  skyBox  The sky box
     */
    virtual void addSkyBox(Resource::SharedPtr<Render::SkyBox> skyBox) = 0;
    /**
     * @brief      Remove all the lights, meshes, and sky boxes of the queue
     */
    virtual void clear() = 0;
};

} // Render
} // Graphics
} // lug
