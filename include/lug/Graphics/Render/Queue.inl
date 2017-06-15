inline const std::vector<Scene::Node*>& Queue::getMeshs() const {
    return _meshs;

}

inline std::size_t Queue::getMeshsNb() const {
    return _meshsNb;
}

inline const std::vector<Scene::Node*>& Queue::getLights() const {
    return _lights;

}

inline std::size_t Queue::getLightsNb() const {
    return _lightsNb;
}
