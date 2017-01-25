#pragma once

#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/RenderTarget.hpp>
#include <lug/Window/Window.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API RenderWindow: public lug::Window::Window, public RenderTarget {
public:
    struct InitInfo {
        lug::Window::Window::InitInfo windowInitInfo;
        std::vector<RenderView::InitInfo> renderViewsInitInfo;
    };

public:
    RenderWindow() = default;

    RenderWindow(const RenderWindow&) = delete;
    RenderWindow(RenderWindow&&) = delete;

    RenderWindow& operator=(const RenderWindow&) = delete;
    RenderWindow& operator=(RenderWindow&&) = delete;

    virtual ~RenderWindow() = default;

    virtual bool beginFrame() = 0;
    virtual bool endFrame() = 0;
};

} // Graphics
} // lug
