#include <queue>
#include <string>
#include <lug/Window/Unix/WindowImplX11.hpp>

lug::Window::priv::WindowImpl::WindowImpl(Window* win): _parent{win} {}

bool lug::Window::priv::WindowImpl::create(const std::string& title, Style style) {
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

    _window = XCreateSimpleWindow(_display, parent, 0, 0, _parent->_mode.width, _parent->_mode.height, 90, 2, blackColor);

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

Bool selectEvents(Display* display, XEvent* event, XPointer arg)
{
    if (event->type == ClientMessage || event->type == DestroyNotify)
        return True;
    else
        return False;
}

bool lug::Window::priv::WindowImpl::pollEvent(lug::Window::Event& event) {
    XEvent xEvent;

    if (XCheckIfEvent(_display, &xEvent, selectEvents, nullptr) == True)
    {
        switch (xEvent.type) {
            case ClientMessage:
                if (xEvent.xclient.message_type == wmProtocols && xEvent.xclient.data.l[0] == wmDeleteWindow) {
                    event.type = lug::Window::EventType::CLOSE;
                }
                break;
            case DestroyNotify:
                event.type = lug::Window::EventType::DESTROY;
                break;
            default:
                return false;
        }
    }
    else
        return false;

    return true;
}
