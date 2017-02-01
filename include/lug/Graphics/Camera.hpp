#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Node.hpp>
#include <lug/Graphics/RenderQueue.hpp>

namespace lug {
namespace Graphics {

class Scene;
class RenderView;

class LUG_GRAPHICS_API Camera : public Node {
public:
    Camera(const std::string& name);

    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;

    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;

    virtual ~Camera() = default;

    Scene* getScene() const;
    void setScene(Scene* scene);

    RenderQueue& getRenderQueue();
    const RenderQueue& getRenderQueue() const;

    float getFov() const;
    void setFov(float fov);

    virtual void update(const RenderView* renderView) = 0;

protected:
    Scene* _scene{nullptr};
    RenderQueue _renderQueue;
    float _fov;
};

#include <lug/Graphics/Camera.inl>

} // Graphics
} // lug
