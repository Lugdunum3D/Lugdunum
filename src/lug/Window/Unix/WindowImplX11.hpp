#pragma once

#include <lug/Window/Window.hpp> // I include Window.hpp before Xlib because Xlib is dumb
#include <X11/Xlib.h>

namespace lug {
namespace Window {
namespace priv {

class WindowImpl {
public:
    WindowImpl() = default;
    ~WindowImpl() = default;

    WindowImpl(const WindowImpl&) = delete;
    WindowImpl(WindowImpl&&) = delete;
    WindowImpl& operator=(const WindowImpl&) = delete;
    WindowImpl& operator=(WindowImpl&&) = delete;

    bool create(uint16_t width, uint16_t height, const std::string& title, Style style);
    void close();
    void processEvents(std::queue<lug::Window::Event>& events);

private:
	Display *_display;
	::Window  _window;
    Atom wmProtocols;
    Atom wmDeleteWindow;
};

} // namespace priv
} // namespace Window
} // namespace lug
