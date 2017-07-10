inline Node* Node::getNode(const std::string& name) {
    return static_cast<Node*>(::lug::Graphics::Node::getNode(name));
}

inline const Node* Node::getNode(const std::string& name) const {
    return static_cast<const Node*>(::lug::Graphics::Node::getNode(name));
}

inline Scene& Node::getScene() {
    return _scene;
}

inline const Scene& Node::getScene() const {
    return _scene;
}

inline Render::Light* Node::getLight() {
    return _light.get();
}

inline const Render::Light* Node::getLight() const {
    return _light.get();
}

inline const Node::MeshInstance* Node::getMeshInstance() const {
    return _meshInstance.mesh ? &_meshInstance : nullptr;
}

inline Render::Camera::Camera* Node::getCamera() {
    return _camera.get();
}

inline const Render::Camera::Camera* Node::getCamera() const {
    return _camera.get();
}
