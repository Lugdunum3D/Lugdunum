#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Scene/MovableObject.hpp>

namespace lug {
namespace Graphics {

namespace Render {
class Camera;
} // Scene

namespace Scene {

class Node;

class LUG_GRAPHICS_API MovableCamera final : public ::lug::Graphics::Scene::MovableObject {
public:
    MovableCamera(const std::string& name, Render::Camera* camera = nullptr);

    MovableCamera(const MovableCamera&) = delete;
    MovableCamera(MovableCamera&&) = delete;

    MovableCamera& operator=(const MovableCamera&) = delete;
    MovableCamera& operator=(MovableCamera&&) = delete;

    ~MovableCamera() = default;

    void setParent(Node* parent) override final;

    bool attachCamera(Render::Camera* camera);
    void detachCamera();

    void needUpdate() override final;

private:
    Render::Camera *_camera{nullptr};
};

} // Scene
} // Graphics
} // lug
