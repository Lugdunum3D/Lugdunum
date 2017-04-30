template <typename T>
Resource::SharedPtr<T> ResourceManager::get(Resource::Handle handle) {
    if (_resources.size() <= handle.index) {
        return nullptr;
    }

    if (_resources[handle.index]->getHandle() == handle) {
        return _resources[handle.index].get();
    }

    return nullptr;
}
