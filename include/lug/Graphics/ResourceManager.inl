template <typename T>
Resource::SharedPtr<T> ResourceManager::get(Resource::Handle handle) {
    static_assert(
        std::is_base_of<Resource, T>::value,
        "T must inherit from Resource"
    );

    if (_resources.size() <= handle.index) {
        return nullptr;
    }

    if (_resources[handle.index]->getHandle() == handle) {
        return _resources[handle.index].get();
    }

    return nullptr;
}

template <typename T>
Resource::SharedPtr<T> ResourceManager::add(std::unique_ptr<Resource> resource) {
    static_assert(
        std::is_base_of<Resource, T>::value,
        "T must inherit from Resource"
    );

    resource->_handle.index = _resources.size();
    _resources.push_back(std::move(resource));

    return dynamic_cast<T*>(_resources.back().get());
}
