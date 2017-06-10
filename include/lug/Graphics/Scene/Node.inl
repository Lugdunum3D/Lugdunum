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

inline const Resource::SharedPtr<Render::Light>& Node::getLight() const {
    return _light;
}

inline const Node::MeshInstance& Node::getMeshInstance() const {
    return _meshInstance;
}

inline const Resource::SharedPtr<Render::Camera::Camera>& Node::getCamera() const {
    return _camera;
}
