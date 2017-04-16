#pragma once

#include <cstdint>
#include <memory>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Camera.hpp>
#include <lug/Graphics/Render/Technique/Type.hpp>

namespace lug {
namespace Graphics {
namespace Render {

class Camera;
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
        Technique::Type renderTechniqueType;
        Viewport viewport;
        Scissor scissor;
        std::unique_ptr<Camera> camera;
        // TODO: Clear color
    };

public:
    View(const Target* renderTarget);

    View(const View&) = delete;
    View(View&&) = delete;

    View& operator=(const View&) = delete;
    View& operator=(View&&) = delete;

    virtual ~View() = default;

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
    virtual bool endFrame() = 0;

protected:
    const Target* _renderTarget;
    InitInfo _info;

    Viewport _viewport{};
    Scissor _scissor{};
    std::unique_ptr<Camera> _camera{nullptr};
};

#include <lug/Graphics/Render/View.inl>

} // Render
} // Graphics
} // lug
