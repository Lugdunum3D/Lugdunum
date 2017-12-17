#pragma once

#include <cstdint>
#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Camera/Camera.hpp>

namespace lug {
namespace Graphics {
namespace Render {

namespace Camera {
class Camera;
} // Camera

class Target;

class LUG_GRAPHICS_API View {
public:
    // x, y, width, height => percentage of the screen
    // TODO: Declare offset and extent outside
    class Viewport {
    public:
        struct {
            float x;
            float y;
        } offset;

        struct {
            float width;
            float height;
        } extent;

        float minDepth;
        float maxDepth;

        inline float getRatio() const;
    };

    struct Scissor {
        struct {
            float x;
            float y;
        } offset;

        struct {
            float width;
            float height;
        } extent;
    };

    struct InitInfo {
        Viewport viewport;
        Scissor scissor;
        Resource::SharedPtr<Camera::Camera> camera;
        // TODO: Clear color
    };

public:
    View(const Target* renderTarget);

    View(const View&) = delete;
    View(View&&) = delete;

    View& operator=(const View&) = delete;
    View& operator=(View&&) = delete;

    virtual ~View() = default;

    /**
     * @brief      Init the render view
     *
     * @param      initInfo  The init information
     */
    void init(InitInfo& initInfo);

    /**
     * @brief      Gets the init information.
     *
     * @return     The init information.
     */
    InitInfo& getInfo();
    /**
     * @brief      Gets the init information.
     *
     * @return     The init information.
     */
    const InitInfo& getInfo() const;

    /**
     * @brief      Gets the viewport.
     *
     * @return     The viewport.
     */
    const Viewport& getViewport() const;
    /**
     * @brief      Gets the scissor.
     *
     * @return     The scissor.
     */
    const Scissor& getScissor() const;

    /**
     * @brief      Gets the clear color.
     *
     * @return     The clear color.
     */
    const Math::Vec3f& getClearColor() const;
    /**
     * @brief      Sets the clear color.
     *
     * @param[in]  color  The color
     */
    void setClearColor(const Math::Vec3f& color);

    /**
     * @brief      Attach a camera to the render view
     *
     * @param[in]  camera  The camera
     */
    void attachCamera(Resource::SharedPtr<Camera::Camera> camera);
    /**
     * @brief      Gets the camera.
     *
     * @return     The camera.
     */
    Resource::SharedPtr<Camera::Camera> getCamera() const;

    /**
     * @brief      Update the render queue
     */
    void update();

    virtual void destroy() = 0;
    virtual bool endFrame() = 0;

protected:
    const Target* _renderTarget;
    InitInfo _info;

    Viewport _viewport{};
    Scissor _scissor{};
    Math::Vec3f _clearColor{0.0f, 0.0f, 0.0f};
    Resource::SharedPtr<Camera::Camera> _camera{nullptr};
};

#include <lug/Graphics/Render/View.inl>

} // Render
} // Graphics
} // lug
