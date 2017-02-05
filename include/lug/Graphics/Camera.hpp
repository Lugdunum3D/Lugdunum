#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Node.hpp>
#include <lug/Graphics/RenderQueue.hpp>

namespace lug {
namespace Graphics {

class Scene;
class RenderView;

/**
 * @brief      Class for camera.
 *
 *             This class represents a Camera in the 3D engine. A scene can be attached
 *             to a Camera. Only one scene can be attached to a Camera. @n
 *             A Camera can be attached to only one RenderView.
 */
class LUG_GRAPHICS_API Camera : public Node {
public:
    Camera(const std::string& name);

    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;

    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;

    virtual ~Camera() = default;

    /**
     * @brief      Gets the scene attached to the Camera.
     *
     * @return     A pointer to the scene.
     */
    Scene* getScene() const;

    /**
     * @brief      Attach the scene to the Camera.
     *
     * @param      scene  The scene to attach.
     */
    void setScene(Scene* scene);

    /**
     * @brief      Gets the render queue of the Camera.
     *
     * @return     A reference to the RenderQueue.
     */
    RenderQueue& getRenderQueue();
    const RenderQueue& getRenderQueue() const;

    /**
     * @brief      Gets the fov (field of view).
     *
     * @return     The fov (in degrees).
     */
    float getFov() const;

    /**
     * @brief      Sets the fov (field of view).
     *
     * @param[in]  fov   The fov (in degrees).
     */
    void setFov(float fov);

    /**
     * @brief      Update the render queue of the Camera by fetching
     *             the visible objects of the attached scene.
     *
     * @param[in]  renderView  The render view
     */
    virtual void update(const RenderView* renderView) = 0;

protected:
    Scene* _scene{nullptr};
    RenderQueue _renderQueue;
    float _fov;
};

#include <lug/Graphics/Camera.inl>

} // Graphics
} // lug
