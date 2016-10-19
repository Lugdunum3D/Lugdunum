#include <lug/Window/Window.hpp>

#if defined(LUG_SYSTEM_WINDOWS)

    #include <lug/Window/Win32/WindowImplWin32.hpp>

#elif defined(LUG_SYSTEM_LINUX)

    #include <lug/Window/Unix/WindowImplX11.hpp>

#elif defined(LUG_SYSTEM_ANDROID)

    #include <lug/Window/Android/WindowImplAndroid.hpp>

#endif

lug::Window::Window::Window() : _impl{new lug::Window::priv::WindowImpl(this)} {
}

lug::Window::Window::Window(Window&& rhs) : _impl{rhs._impl} {
    rhs._impl = nullptr;
}

lug::Window::Window& lug::Window::Window::operator=(Window&& rhs) {
    _impl = rhs._impl;
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

    return win;
}

bool lug::Window::Window::createWindow(uint16_t width, uint16_t height, const std::string& title, Style style) {
    if (_impl != nullptr) {

        // Specifiy the width and height of our window, for now it's the only thing we can specify along with the title and style that is
        _mode.width = width;
        _mode.height = height;

        return _impl->create(title, style);
    }

    return false;
}

bool lug::Window::Window::isOpen() const {
    return _impl != nullptr;
}

bool lug::Window::Window::pollEvent(lug::Window::Event& event) {
    if (_impl != nullptr) {
        const bool value = _impl->pollEvent(event);

        // If we've receive DESTROY even just close window without returning the event to the user
        if (value && event.type == lug::Window::EventType::DESTROY) {
            close();
            return false;
        }
        return value;
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
