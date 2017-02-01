#pragma once

#include <cstdint>
#include <memory>
#include <lug/Graphics/Camera.hpp>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/RenderTechnique.hpp>

namespace lug {
namespace Graphics {

class Camera;
class RenderTarget;

class LUG_GRAPHICS_API RenderView {
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
        RenderTechnique::Type renderTechniqueType;
        Viewport viewport;
        Scissor scissor;
        std::unique_ptr<Camera> camera;
        // TODO: Clear color
    };

public:
    RenderView(const RenderTarget* renderTarget);

    RenderView(const RenderView&) = delete;
    RenderView(RenderView&&) = delete;

    RenderView& operator=(const RenderView&) = delete;
    RenderView& operator=(RenderView&&) = delete;

    virtual ~RenderView() = default;

    void init(InitInfo& initInfo);

    InitInfo& getInfo();
    const InitInfo& getInfo() const;

    const Viewport& getViewport() const;
    const Scissor& getScissor() const;

    std::unique_ptr<Camera> attachCamera(std::unique_ptr<Camera> camera);
    std::unique_ptr<Camera> detachCamera();
    Camera* getCamera() const;

    void update();

    virtual void destroy() = 0;

protected:
    const RenderTarget* _renderTarget;
    InitInfo _info;

    Viewport _viewport{};
    Scissor _scissor{};
    std::unique_ptr<Camera> _camera{nullptr};
};

#include <lug/Graphics/RenderView.inl>

} // Graphics
} // lug
