#include <lug/Window/Window.hpp>

#if defined(LUG_SYSTEM_WINDOWS)

#include <lug/Window/Win32/WindowImplWin32.hpp>

#elif defined(LUG_SYSTEM_LINUX)

#include <lug/Window/Unix/WindowImplX11.hpp>

#elif defined(LUG_SYSTEM_ANDROID)

#include <lug/Window/Android/WindowImplAndroid.hpp>

#endif

lug::Window::Window::Window() : _impl{ new lug::Window::priv::WindowImpl(this) } {
    initKeyState();
}

lug::Window::Window::Window(Window&& rhs) : _impl{ rhs._impl } {
    rhs._impl = nullptr;
    _keyState = std::move(rhs._keyState);
}

lug::Window::Window& lug::Window::Window::operator=(Window&& rhs) {
    _impl = rhs._impl;
    rhs._impl = nullptr;
    _keyState = std::move(rhs._keyState);

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

void lug::Window::Window::initKeyState() {
    _keyState[lug::Window::Keyboard::Key::A] = false;
    _keyState[lug::Window::Keyboard::Key::B] = false;
    _keyState[lug::Window::Keyboard::Key::C] = false;
    _keyState[lug::Window::Keyboard::Key::D] = false;
    _keyState[lug::Window::Keyboard::Key::E] = false;
    _keyState[lug::Window::Keyboard::Key::F] = false;
    _keyState[lug::Window::Keyboard::Key::G] = false;
    _keyState[lug::Window::Keyboard::Key::H] = false;
    _keyState[lug::Window::Keyboard::Key::I] = false;
    _keyState[lug::Window::Keyboard::Key::J] = false;
    _keyState[lug::Window::Keyboard::Key::K] = false;
    _keyState[lug::Window::Keyboard::Key::L] = false;
    _keyState[lug::Window::Keyboard::Key::M] = false;
    _keyState[lug::Window::Keyboard::Key::N] = false;
    _keyState[lug::Window::Keyboard::Key::O] = false;
    _keyState[lug::Window::Keyboard::Key::P] = false;
    _keyState[lug::Window::Keyboard::Key::Q] = false;
    _keyState[lug::Window::Keyboard::Key::R] = false;
    _keyState[lug::Window::Keyboard::Key::S] = false;
    _keyState[lug::Window::Keyboard::Key::T] = false;
    _keyState[lug::Window::Keyboard::Key::U] = false;
    _keyState[lug::Window::Keyboard::Key::V] = false;
    _keyState[lug::Window::Keyboard::Key::W] = false;
    _keyState[lug::Window::Keyboard::Key::X] = false;
    _keyState[lug::Window::Keyboard::Key::Y] = false;
    _keyState[lug::Window::Keyboard::Key::Z] = false;

    _keyState[lug::Window::Keyboard::Key::Num0] = false;
    _keyState[lug::Window::Keyboard::Key::Num1] = false;
    _keyState[lug::Window::Keyboard::Key::Num2] = false;
    _keyState[lug::Window::Keyboard::Key::Num3] = false;
    _keyState[lug::Window::Keyboard::Key::Num4] = false;
    _keyState[lug::Window::Keyboard::Key::Num5] = false;
    _keyState[lug::Window::Keyboard::Key::Num6] = false;
    _keyState[lug::Window::Keyboard::Key::Num7] = false;
    _keyState[lug::Window::Keyboard::Key::Num8] = false;
    _keyState[lug::Window::Keyboard::Key::Num9] = false;

    _keyState[lug::Window::Keyboard::Key::Escape] = false;

    _keyState[lug::Window::Keyboard::Key::LControl] = false;
    _keyState[lug::Window::Keyboard::Key::LShift] = false;
    _keyState[lug::Window::Keyboard::Key::LAlt] = false;
    _keyState[lug::Window::Keyboard::Key::LSystem] = false;
    _keyState[lug::Window::Keyboard::Key::RControl] = false;
    _keyState[lug::Window::Keyboard::Key::RShift] = false;
    _keyState[lug::Window::Keyboard::Key::RAlt] = false;
    _keyState[lug::Window::Keyboard::Key::RSystem] = false;

    _keyState[lug::Window::Keyboard::Key::Menu] = false;
    _keyState[lug::Window::Keyboard::Key::LBracket] = false;
    _keyState[lug::Window::Keyboard::Key::RBracket] = false;
    _keyState[lug::Window::Keyboard::Key::SemiColon] = false;
    _keyState[lug::Window::Keyboard::Key::Comma] = false;
    _keyState[lug::Window::Keyboard::Key::Period] = false;
    _keyState[lug::Window::Keyboard::Key::Quote] = false;
    _keyState[lug::Window::Keyboard::Key::QuoteDouble] = false;
    _keyState[lug::Window::Keyboard::Key::Slash] = false;
    _keyState[lug::Window::Keyboard::Key::BackSlash] = false;
    _keyState[lug::Window::Keyboard::Key::Tilde] = false;
    _keyState[lug::Window::Keyboard::Key::Equal] = false;
    _keyState[lug::Window::Keyboard::Key::Dash] = false;
    _keyState[lug::Window::Keyboard::Key::Ampersand] = false;
    _keyState[lug::Window::Keyboard::Key::Space] = false;
    _keyState[lug::Window::Keyboard::Key::Return] = false;
    _keyState[lug::Window::Keyboard::Key::BackSpace] = false;
    _keyState[lug::Window::Keyboard::Key::Tab] = false;
    _keyState[lug::Window::Keyboard::Key::PageUp] = false;
    _keyState[lug::Window::Keyboard::Key::PageDown] = false;
    _keyState[lug::Window::Keyboard::Key::End] = false;
    _keyState[lug::Window::Keyboard::Key::Home] = false;
    _keyState[lug::Window::Keyboard::Key::Insert] = false;
    _keyState[lug::Window::Keyboard::Key::Delete] = false;
    _keyState[lug::Window::Keyboard::Key::Add] = false;
    _keyState[lug::Window::Keyboard::Key::Subtract] = false;
    _keyState[lug::Window::Keyboard::Key::Multiply] = false;
    _keyState[lug::Window::Keyboard::Key::Divide] = false;
    _keyState[lug::Window::Keyboard::Key::Left] = false;
    _keyState[lug::Window::Keyboard::Key::Right] = false;
    _keyState[lug::Window::Keyboard::Key::Up] = false;
    _keyState[lug::Window::Keyboard::Key::Down] = false;

    _keyState[lug::Window::Keyboard::Key::Numpad0] = false;
    _keyState[lug::Window::Keyboard::Key::Numpad1] = false;
    _keyState[lug::Window::Keyboard::Key::Numpad2] = false;
    _keyState[lug::Window::Keyboard::Key::Numpad3] = false;
    _keyState[lug::Window::Keyboard::Key::Numpad4] = false;
    _keyState[lug::Window::Keyboard::Key::Numpad5] = false;
    _keyState[lug::Window::Keyboard::Key::Numpad6] = false;
    _keyState[lug::Window::Keyboard::Key::Numpad7] = false;
    _keyState[lug::Window::Keyboard::Key::Numpad8] = false;
    _keyState[lug::Window::Keyboard::Key::Numpad9] = false;

    _keyState[lug::Window::Keyboard::Key::F1] = false;
    _keyState[lug::Window::Keyboard::Key::F2] = false;
    _keyState[lug::Window::Keyboard::Key::F3] = false;
    _keyState[lug::Window::Keyboard::Key::F4] = false;
    _keyState[lug::Window::Keyboard::Key::F5] = false;
    _keyState[lug::Window::Keyboard::Key::F6] = false;
    _keyState[lug::Window::Keyboard::Key::F7] = false;
    _keyState[lug::Window::Keyboard::Key::F8] = false;
    _keyState[lug::Window::Keyboard::Key::F9] = false;
    _keyState[lug::Window::Keyboard::Key::F10] = false;
    _keyState[lug::Window::Keyboard::Key::F11] = false;
    _keyState[lug::Window::Keyboard::Key::F12] = false;
    _keyState[lug::Window::Keyboard::Key::F13] = false;
    _keyState[lug::Window::Keyboard::Key::F14] = false;
    _keyState[lug::Window::Keyboard::Key::F15] = false;
    _keyState[lug::Window::Keyboard::Key::Pause] = false;
    _keyState[lug::Window::Keyboard::Key::CapsLock] = false;
}

bool lug::Window::Window::isOpen() const {
    return _impl != nullptr;
}

bool lug::Window::Window::pollEvent(lug::Window::Event& event) {
    if (_impl != nullptr) {
        const bool value = _impl->pollEvent(event);

        // If we've receive DESTROY just close window without returning the event to the user
        if (value && event.type == lug::Window::EventType::DESTROY) {
            close();
            return false;
        }

        if (event.type == lug::Window::EventType::KEY_DOWN) {
            _keyState[event.key.code] = true;
        }
        else if (event.type == lug::Window::EventType::KEY_UP) {
            _keyState[event.key.code] = false;
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

bool lug::Window::Window::isKeyPressed(Keyboard::Key key) const {
    return (_keyState.at(key));
}

