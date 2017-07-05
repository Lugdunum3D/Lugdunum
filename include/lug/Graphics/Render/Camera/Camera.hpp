#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Node.hpp>
#include <lug/Graphics/Render/DirtyObject.hpp>
#include <lug/Graphics/Render/Queue.hpp>
#include <lug/Graphics/Resource.hpp>
#include <lug/Math/Matrix.hpp>

namespace lug {
namespace Graphics {

namespace Scene {
class Scene;
class Node;
} // Scene

namespace Render {

class View;

namespace Camera {

/**
 * @brief      Class for camera.
 *
 *             This class represents a Camera in the 3D engine.
 *             A Camera can be attached to only one Render::View and one Scene::Node.
 */
class LUG_GRAPHICS_API Camera : public Resource, public DirtyObject {
public:
    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;

    Camera& operator=(const Camera&) = delete;
    Camera& operator=(Camera&&) = delete;

    virtual ~Camera() = default;

    /**
     * @brief      Sets the znear (distance of near plane).
     *
     * @param[in]  znear    The near value.
     */
    void setZNear(float znear);

    /**
     * @brief      Gets the znear (distance of near plane).
     *
     * @return     The near value.
     */
    float getZNear() const;

    /**
     * @brief      Sets the zfar (distance of far plane).
     *
     * @param[in]  zfar     The far value.
     */
    void setZFar(float zfar);

    /**
     * @brief      Gets the zfar (distance of far plane)
     *
     * @return     The zfar value.
     */
    float getZFar() const;

    /**
     * @brief      Gets the projection matrix. Computes it if needed, but internally
     *             the projection matrix is cached.
     *
     * @return     The projection matrix.
     */
    const Math::Mat4x4f& getProjectionMatrix();

    /**
     * @brief      Gets the view matrix. Computes it if needed, but internally
     *             the view matrix is cached.
     *
     * @return     The view matrix.
     */
    const Math::Mat4x4f& getViewMatrix();

    /**
     * @brief      Update the render queue of the Camera by fetching
     *             the visible objects of the attached scene.
     *
     * @param[in]  renderView  The render view
     * @param[in]  renderQueue The render queue
     */
    void update(const View& renderView, Queue& renderQueue);

    void setRenderView(View* renderView);

    Scene::Node* getParent() const;
    void setParent(Scene::Node* parent);

    void needUpdateProj();
    void needUpdateView();

    void lookAt(const Math::Vec3f& targetPosition, const Math::Vec3f& up, Node::TransformSpace space = Node::TransformSpace::Local);

protected:
    explicit Camera(const std::string& name);

    virtual void updateProj() = 0;
    void updateView();

protected:
    Scene::Node* _parent{nullptr};

    View* _renderView{nullptr};

    float _znear{0.1f};
    float _zfar{100.0f};

    bool _needUpdateProj{true};
    bool _needUpdateView{true};

    Math::Mat4x4f _projMatrix{Math::Mat4x4f::identity()};
    Math::Mat4x4f _viewMatrix{Math::Mat4x4f::identity()};
};

#include <lug/Graphics/Render/Camera/Camera.inl>

} // Camera
} // Render
} // Graphics
} // lug
