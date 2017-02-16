inline const std::vector<MeshInstance*>& RenderQueue::getMeshs() const {
    return _meshs;

}

inline std::size_t RenderQueue::getMeshsNb() const {
    return _meshsNb;
}

inline const std::vector<Light*>& RenderQueue::getLights() const {
    return _lights;

}

inline std::size_t RenderQueue::getLightsNb() const {
    return _lightsNb;
}
