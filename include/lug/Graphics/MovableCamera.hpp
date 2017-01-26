#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/MovableObject.hpp>

namespace lug {
namespace Graphics {

class Camera;

class LUG_GRAPHICS_API MovableCamera final : public lug::Graphics::MovableObject {
public:
    MovableCamera(const std::string& name);

    MovableCamera(const MovableCamera&) = delete;
    MovableCamera(MovableCamera&&) = delete;

    MovableCamera& operator=(const MovableCamera&) = delete;
    MovableCamera& operator=(MovableCamera&&) = delete;

    ~MovableCamera() = default;

    void setParent(SceneNode* parent) override final;

    bool attachCamera(Camera* camera);
    void detachCamera();

    void needUpdate() override final;

private:
    Camera *_camera{nullptr};
};

} // Graphics
} // lug
