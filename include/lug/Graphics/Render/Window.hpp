#pragma once

#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Target.hpp>
#include <lug/Graphics/Render/View.hpp>
#include <lug/Window/Window.hpp>
#include <lug/System/Time.hpp>

namespace lug {
namespace Graphics {
namespace Render {

class LUG_GRAPHICS_API Window: public ::lug::Window::Window, public ::lug::Graphics::Render::Target {
public:
    struct InitInfo {
        lug::Window::Window::InitInfo windowInitInfo;
        std::vector<View::InitInfo> renderViewsInitInfo;
    };

public:
    Window() = default;

    Window(const Window&) = delete;
    Window(Window&&) = delete;

    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    virtual ~Window() = default;

    virtual bool beginFrame(const lug::System::Time &elapsedTime) = 0;
    virtual bool endFrame() = 0;
};

} // Render
} // Graphics
} // lug
