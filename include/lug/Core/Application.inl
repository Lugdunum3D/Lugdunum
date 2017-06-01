inline const Application::Info& Application::getInfo() const {
    return _info;
}

inline lug::Graphics::Graphics& Application::getGraphics() {
    return _graphics;
}

inline const lug::Graphics::Graphics& Application::getGraphics() const {
    return _graphics;
}


inline lug::Graphics::Render::Window* Application::getWindow() {
    return _window;
}

inline const lug::Graphics::Render::Window* Application::getWindow() const {
    return _window;
}

inline void Application::setRenderWindowInfo(lug::Graphics::Render::Window::InitInfo& initInfo) {
    _renderWindowInitInfo = std::move(initInfo);
}

inline const lug::Graphics::Render::Window::InitInfo& Application::getRenderWindowInfo() const {
    return _renderWindowInitInfo;
}

inline lug::Graphics::Render::Window::InitInfo& Application::getRenderWindowInfo() {
    return _renderWindowInitInfo;
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
