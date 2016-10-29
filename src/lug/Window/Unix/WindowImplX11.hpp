#pragma once

#include <lug/Window/Window.hpp> // I include Window.hpp before Xlib because Xlib is dumb
#include <X11/Xlib.h>

namespace lug {
namespace Window {
namespace priv {

class WindowImpl {
public:
    WindowImpl(Window*);
    ~WindowImpl() = default;

    WindowImpl(const WindowImpl&) = delete;
    WindowImpl(WindowImpl&&) = delete;
    WindowImpl& operator=(const WindowImpl&) = delete;
    WindowImpl& operator=(WindowImpl&&) = delete;

    bool create(const std::string& title, Style style);
    void close();

    bool pollEvent(lug::Window::Event& event);

private:
    Window* _parent{nullptr};

	Display* _display;
	::Window  _window;
    Atom wmProtocols;
    Atom wmDeleteWindow;
};

} // namespace priv
} // namespace Window
} // namespace lug
