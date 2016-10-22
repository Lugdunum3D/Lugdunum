#include <memory>
#include <string>
#include <utility>
#include <lug/Window/Window.hpp>

#if defined(LUG_SYSTEM_WINDOWS)

    #include <lug/Window/Win32/WindowImplWin32.hpp>

#elif defined(LUG_SYSTEM_LINUX)

    #include <lug/Window/Unix/WindowImplX11.hpp>

#elif defined(LUG_SYSTEM_ANDROID)

    #include <lug/Window/Android/WindowImplAndroid.hpp>

#endif

lug::Window::Window::Window() : _impl{new lug::Window::priv::WindowImpl()} {
}

lug::Window::Window::Window(Window&& rhs) : _impl{rhs._impl}, _events{std::move(rhs._events)} {
    rhs._impl = nullptr;
}

lug::Window::Window& lug::Window::Window::operator=(Window&& rhs) {
    _impl = rhs._impl;
    _events = std::move(rhs._events);
    rhs._impl = nullptr;

    return *this;
}

lug::Window::Window::~Window() {
    close();
}

std::unique_ptr<lug::Window::Window> lug::Window::Window::create(uint16_t width, uint16_t height, const std::string& title, Style style) {
    std::unique_ptr<Window> win(new Window());

    if (!win->createWindow(width, height, title, style)) {
        return nullptr;
    }

    return std::move(win);
}

bool lug::Window::Window::createWindow(uint16_t width, uint16_t height, const std::string& title, Style style) {
    if (_impl != nullptr) {
        return _impl->create(width, height, title, style);
    }

    return false;
}

bool lug::Window::Window::isOpen() const {
    return _impl != nullptr ? true : false;
}

bool lug::Window::Window::pollEvent(Event& event) {
    if (_events.empty() && _impl != nullptr) {
        _impl->processEvents(_events);
    }

    if (!_events.empty()) {
        event = _events.front();
        _events.pop();

        return true;
    }

    return false;
}

void lug::Window::Window::close() {
    if (_impl != nullptr) {
        _impl->close();
        delete _impl;
        _impl = nullptr;
    }
}
