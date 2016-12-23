#pragma once

#include <lug/Window/Window.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API RenderWindow: public lug::Window::Window {
public:
    RenderWindow() = default;

    RenderWindow(const RenderWindow&) = delete;
    RenderWindow(RenderWindow&&) = delete;

    RenderWindow& operator=(const RenderWindow&) = delete;
    RenderWindow& operator=(RenderWindow&&) = delete;

    virtual ~RenderWindow() = default;

    virtual bool beginFrame() = 0;
    virtual bool endFrame() = 0;

    virtual void clearScreen(float color[4]) = 0;
};

} // Graphics
} // lug
