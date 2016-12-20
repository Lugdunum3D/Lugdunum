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

    virtual bool init() = 0;
};

} // Graphics
} // lug
