#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/MovableObject.hpp>
#include <lug/Graphics/RenderQueue.hpp>
#include <lug/Graphics/Scene.hpp>

namespace lug {
namespace Graphics {

class RenderView;

class LUG_GRAPHICS_API Camera {
public:
    Camera() = default;

    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;

    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;

    virtual ~Camera() = default;

    Scene* getScene() const;

    RenderQueue& getRenderQueue();
    const RenderQueue& getRenderQueue() const;

    virtual void update(const RenderView* renderView) = 0;

protected:
    Scene* _scene{nullptr};
    RenderQueue _renderQueue;
};

#include <lug/Graphics/Camera.inl>

} // Graphics
} // lug
