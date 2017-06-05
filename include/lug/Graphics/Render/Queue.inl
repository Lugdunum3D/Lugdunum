inline const std::vector<Scene::MeshInstance*>& Queue::getMeshs() const {
    return _meshs;

}

inline std::size_t Queue::getMeshsNb() const {
    return _meshsNb;
}

inline const std::vector<Render::Light*>& Queue::getLights() const {
    return _lights;

}

inline std::size_t Queue::getLightsNb() const {
    return _lightsNb;
}
