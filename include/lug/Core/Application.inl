inline uint32_t Application::Version::createVersion() const {
    return (major << 22) | (minor << 12) | patch;
}

inline const Application::Info& Application::getInfo() const {
    return _info;
}

inline lug::Graphics::Graphics& Application::getGraphics() {
    return _graphics;
}

inline const lug::Graphics::Graphics& Application::getGraphics() const {
    return _graphics;
}


inline lug::Graphics::RenderWindow* Application::getWindow() {
    return _window;
}

inline const lug::Graphics::RenderWindow* Application::getWindow() const {
    return _window;
}

inline void Application::setWindowInfo(const lug::Window::Window::InitInfo& initInfo) {
    _windowInitInfo = initInfo;
}

inline const lug::Window::Window::InitInfo& Application::getWindowInfo() const {
    return _windowInitInfo;
}

inline lug::Window::Window::InitInfo& Application::getWindowInfo() {
    return _windowInitInfo;
}

inline void Application::setGraphicsInfo(const lug::Graphics::Graphics::InitInfo& initInfo) {
    _graphicsInitInfo = initInfo;
}

inline const lug::Graphics::Graphics::InitInfo& Application::getGraphicsInfo() const {
    return _graphicsInitInfo;
}

inline lug::Graphics::Graphics::InitInfo& Application::getGraphicsInfo() {
    return _graphicsInitInfo;
}
