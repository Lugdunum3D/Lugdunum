#include <lug/Window/Window.hpp>

#if defined(LUG_SYSTEM_WINDOWS)
    #include <lug/Window/Win32/WindowImplWin32.hpp>
#elif defined(LUG_SYSTEM_LINUX)
    #include <lug/Window/Unix/WindowImplX11.hpp>
#elif defined(LUG_SYSTEM_ANDROID)
    #include <lug/Window/Android/WindowImplAndroid.hpp>
#endif

namespace lug {
namespace Window {

lug::Window::Window::Window() : _impl{new lug::Window::priv::WindowImpl(this)} {
}

lug::Window::Window::~Window() {
    close();
}

std::unique_ptr<lug::Window::Window> lug::Window::Window::create(const InitInfo& initInfo) {
    std::unique_ptr<Window> win(new Window());

    if (!win->init(initInfo)) {
        return nullptr;
    }

    return win;
}

bool lug::Window::Window::init(const InitInfo& initInfo) {
    if (_impl != nullptr) {

        // Specifiy the width and height of our window, for now it's the only thing we can specify along with the title and style that is
        _mode.width = initInfo.width;
        _mode.height = initInfo.height;

        return _impl->init(initInfo);
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

} // Window
} // lug
