inline void Scene::setSkyBox(Resource::SharedPtr<Render::SkyBox> skyBox) {
    _skyBox = skyBox;
}

inline Node& Scene::getRoot() {
    return _root;
}

inline const Node& Scene::getRoot() const {
    return _root;
}


inline const Resource::SharedPtr<Render::SkyBox> Scene::getSkyBox() const {
    return _skyBox;
}
