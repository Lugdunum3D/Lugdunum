inline void FreeMovement::setTargetNode(lug::Graphics::Scene::Node& target) {
    _target = &target;
}

inline void FreeMovement::setEventSource(lug::Window::Window& eventSource) {
    _eventSource = &eventSource;
    _lastMousePos = _eventSource->getMousePos();
}
