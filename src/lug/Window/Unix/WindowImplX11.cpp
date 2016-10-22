#include <queue>
#include <string>
#include <lug/Window/Unix/WindowImplX11.hpp>

bool lug::Window::priv::WindowImpl::create(uint16_t width, uint16_t height, const std::string& title, Style style) {
    uint32_t blackColor = 0;
    uint8_t screen = 0;
    GC _graphic_context;

    _display = XOpenDisplay(nullptr);
    if (_display == nullptr) {
            return false;
    }

    screen = DefaultScreen(_display);
    _graphic_context = DefaultGC(_display, screen);
    ::Window parent = RootWindow(_display, screen);

    blackColor = BlackPixel(_display, DefaultScreen(_display));

    _window = XCreateSimpleWindow(_display, parent, 0, 0, width, height, 90, 2, blackColor);

    if (!_window) {
        XCloseDisplay(_display);
        return false;
    }

    XSelectInput(_display, _window, ExposureMask | KeyPress);
    XStoreName(_display, _window, title.c_str());
    XMapWindow(_display, _window);

    wmProtocols = XInternAtom(_display, "WM_PROTOCOLS", False);
    wmDeleteWindow = XInternAtom(_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(_display, _window, &wmDeleteWindow, 1);

    return true;
}

void lug::Window::priv::WindowImpl::close() {
    if (_display != nullptr) {
        XCloseDisplay(_display);
        XDestroyWindow(_display, _window);
        _display = nullptr;
        _window = 0;
    }
}

void lug::Window::priv::WindowImpl::processEvents(std::queue<lug::Window::Event>& events) {
    XEvent      event;

    XNextEvent(_display, &event);

    switch (event.type) {
        case ClientMessage:
            if (event.xclient.message_type == wmProtocols && event.xclient.data.l[0] == wmDeleteWindow) {
                events.push({lug::Window::EventType::CLOSE});
            }
            break;
        default:
            break;
    }
}
