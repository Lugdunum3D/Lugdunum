inline const Render::Model* ModelInstance::getModel() const {
    return _model;
}
inline Render::Model* ModelInstance::getModel() {
    return _model;
}

inline const std::vector<std::unique_ptr<MeshInstance>>& ModelInstance::getMeshsInstances() const {
    return _meshsInstances;
}
