#include <lug/Window/Unix/WindowImplX11.hpp>
#include <cstring>
#include <string>
#include <queue>
#include <lug/Window/Unix/WmHints.hpp>

namespace lug {
namespace Window {
namespace priv {

lug::Window::priv::WindowImpl::WindowImpl(Window* win): _parent{win} {}

bool WindowImpl::init(const Window::InitInfo& initInfo) {
    _display = XOpenDisplay(nullptr);
    if (_display == nullptr) {
        return false;
    }

    int screen = DefaultScreen(_display);
    ::Window parent = RootWindow(_display, screen);

    uint32_t blackColor = BlackPixel(_display, DefaultScreen(_display));

    _window = XCreateSimpleWindow(_display, parent, 0, 0, initInfo.width, initInfo.height, 90, 2, blackColor);

    if (!_window) {
        XCloseDisplay(_display);
        _display = nullptr;
        return false;
    }

    XSelectInput(_display, _window, ExposureMask | KeyPress | StructureNotifyMask);
    XStoreName(_display, _window, initInfo.title.c_str());
    XMapWindow(_display, _window);

    _wmProtocols = XInternAtom(_display, "WM_PROTOCOLS", false);
    _wmDeleteWindow = XInternAtom(_display, "WM_DELETE_WINDOW", false);
    _wmHints = XInternAtom(_display, "_MOTIF_WM_HINTS", false);

    XSetWMProtocols(_display, _window, &_wmDeleteWindow, 1);
    setWindowDecorations(initInfo.style);

    return true;
}

void lug::Window::priv::WindowImpl::close() {
    if (_display != nullptr) {
        XDestroyWindow(_display, _window);
        XCloseDisplay(_display);
        _display = nullptr;
        _window = 0;
    }
}

Bool selectEvents(Display*, XEvent* event, XPointer) {
    if (event->type == ClientMessage || event->type == DestroyNotify || event->type == ConfigureNotify) {
        return True;
    } else {
        return False;
    }
}

bool lug::Window::priv::WindowImpl::pollEvent(lug::Window::Event& event) {
    XEvent xEvent;

    if (XCheckIfEvent(_display, &xEvent, selectEvents, nullptr) == False) {
        return false;
    }

    switch (xEvent.type) {
        case ClientMessage:
            if (xEvent.xclient.message_type == _wmProtocols && static_cast<Atom>(xEvent.xclient.data.l[0]) == _wmDeleteWindow) {
                event.type = lug::Window::EventType::CLOSE;
            } else {
                return false;
            }

            break;
        case DestroyNotify:
            event.type = lug::Window::EventType::DESTROY;
            break;
        case ConfigureNotify:
            if (xEvent.xconfigure.width != _parent->_mode.width || xEvent.xconfigure.height != _parent->_mode.height) {
                _parent->_mode.width = xEvent.xconfigure.width;
                _parent->_mode.height = xEvent.xconfigure.height;

                event.type = lug::Window::EventType::RESIZE;
            } else {
                return false;
            }

            break;
        default:
            return false;
    }

    return true;
}

Display* lug::Window::priv::WindowImpl::getDisplay() const {
    return _display;
}

::Window lug::Window::priv::WindowImpl::getWindow() const {
    return _window;
}

void lug::Window::priv::WindowImpl::setWindowDecorations(Style style) {
    WMHints hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.flags = MWMHintsFunctions | MWMHintsDecorations;
    hints.decorations = 0;
    hints.functions = 0;

    if (static_cast<bool>(style & Style::Titlebar)) {
        hints.decorations |= MWMDecorBorder | MWMDecorTitle | MWMDecorMinimize | MWMDecorMenu;
        hints.functions   |= MWMFuncMove | MWMFuncMinimize;
    }
    if (static_cast<bool>(style & Style::Resize)) {
        hints.decorations |= MWMDecorMaximize | MWMDecorResizeh;
        hints.functions   |= MWMFuncMaximize | MWMFuncResize;
    }
    if (static_cast<bool>(style & Style::Close)) {
        hints.decorations |= 0;
        hints.functions   |= MWMFuncClose;
    }

    XChangeProperty(_display,
                    _window,
                    _wmHints,
                    _wmHints,
                    32,
                    PropModeReplace,
                    reinterpret_cast<const unsigned char*>(&hints),
                    5);
}

} // namespace priv
} // namespace Window
} // namespace lug
