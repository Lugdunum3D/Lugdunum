#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Node.hpp>
#include <lug/Graphics/RenderQueue.hpp>
#include <lug/Math/Geometry/Transform.hpp>
#include <lug/Math/Matrix.hpp>

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
     * @brief      Sets the ratio. Usually called automatically by the RenderView
     *             when attached or when the ratio changed.
     *
     * @param[in]  ratio  The ratio
     */
    void setRatio(float ratio);

    /**
     * @brief      Sets the near (distance of near plane).
     *
     * @param[in]  near   The near value.
     */
    void setNear(float near);

    /**
     * @brief      Gets the near (distance of near plane).
     *
     * @return     The near value.
     */
    float getNear() const;

    /**
     * @brief      Sets the far (distance of far plane).
     *
     * @param[in]  far   The far value.
     */
    void setFar(float far);

    /**
     * @brief      Gets the far (distance of far plane)
     *
     * @return     The far value.
     */
    float getFar() const;

    /**
     * @brief      Gets the projection matrix. Computes it if needed, but internally
     *             the projection matrix is cached.
     *
     * @return     The projection matrix.
     */
    const Math::Mat4x4f& getProjectionMatrix();

    /**
     * @brief      Update the render queue of the Camera by fetching
     *             the visible objects of the attached scene.
     *
     * @param[in]  renderView  The render view
     */
    virtual void update(const RenderView* renderView);

protected:
    Scene* _scene{nullptr};
    RenderQueue _renderQueue;
    float _fov;
    Math::Mat4x4f _projMatrix{Math::Mat4x4f::identity()};
    RenderView* _renderView{nullptr};

    float _near{0.1f};
    float _far{100.0f};
    bool _needUpdateProj{true};
};

#include <lug/Graphics/Camera.inl>

} // Graphics
} // lug
