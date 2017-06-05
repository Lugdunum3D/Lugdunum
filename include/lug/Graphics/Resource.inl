// Shared ptr

template <typename T>
constexpr Resource::SharedPtr<T>::SharedPtr(T* pointer) : _resource(pointer) {
    if (_resource) {
        // TODO: Increment reference counting
    }
}

template <typename T>
Resource::SharedPtr<T>::SharedPtr(const Resource::SharedPtr<T>& rhs) : _resource(rhs._resource) {
    if (_resource) {
        // TODO: Increment reference counting
    }
}

template <typename T>
Resource::SharedPtr<T>::SharedPtr(Resource::SharedPtr<T>&& rhs) : _resource(rhs._resource) {
    rhs._resource = nullptr;
}

template <typename T>
Resource::SharedPtr<T>& Resource::SharedPtr<T>::operator=(const Resource::SharedPtr<T>& rhs) {
    if (_resource) {
        // TODO: Decrement reference counting
    }

    _resource = rhs._resource;

    if (_resource) {
        // TODO: Increment reference counting
    }

    return *this;
}

template <typename T>
Resource::SharedPtr<T>& Resource::SharedPtr<T>::operator=(Resource::SharedPtr<T>&& rhs) {
    if (_resource) {
        // TODO: Decrement reference counting
    }

    _resource = rhs._resource;
    rhs._resource = nullptr;

    return *this;
}

template <typename T>
Resource::SharedPtr<T>::~SharedPtr() {
    if (_resource) {
        // TODO: Decrement reference counting
    }

    _resource = nullptr;
}

template <typename T>
T* Resource::SharedPtr<T>::get() const {
    return _resource;
}

template <typename T>
template <typename RhsT>
Resource::SharedPtr<T> Resource::SharedPtr<T>::cast(const Resource::SharedPtr<RhsT>& rhs) {
    return dynamic_cast<T*>(rhs._resource);
}

// Weak ptr

template <typename T>
constexpr Resource::WeakPtr<T>::WeakPtr(T* pointer) : _resource(pointer) {}

template <typename T>
constexpr Resource::WeakPtr<T>::WeakPtr(const SharedPtr<T>& rhs) : _resource(&(*rhs)) {}

template <typename T>
Resource::WeakPtr<T>::WeakPtr(const Resource::WeakPtr<T>& rhs) : _resource(rhs._resource) {}

template <typename T>
Resource::WeakPtr<T>::WeakPtr(Resource::WeakPtr<T>&& rhs) : _resource(rhs._resource) {
    rhs._resource = nullptr;
}

template <typename T>
Resource::WeakPtr<T>& Resource::WeakPtr<T>::operator=(const Resource::WeakPtr<T>& rhs) {
    _resource = rhs._resource;

    return *this;
}

template <typename T>
Resource::WeakPtr<T>& Resource::WeakPtr<T>::operator=(Resource::WeakPtr<T>&& rhs) {
    _resource = rhs._resource;
    rhs._resource = nullptr;

    return *this;
}

template <typename T>
Resource::WeakPtr<T>::~WeakPtr() {
    _resource = nullptr;
}

template <typename T>
Resource::SharedPtr<T> Resource::WeakPtr<T>::lock() const {
    // TODO: Check that the resource count is greater than 0
    return _resource;
}

template <typename T>
template <typename RhsT>
Resource::WeakPtr<T> Resource::WeakPtr<T>::cast(const Resource::WeakPtr<RhsT>& rhs) {
    return dynamic_cast<T*>(rhs._resource);
}

// Resource

template <typename T>
T& Resource::SharedPtr<T>::operator*() const {
    return *_resource;
}

template <typename T>
T* Resource::SharedPtr<T>::operator->() const {
    return _resource;
}

inline Resource::Type Resource::getType() const {
    return static_cast<Resource::Type>(_handle.type);
}

inline Resource::Handle Resource::getHandle() const {
    return _handle;
}

inline const std::string& Resource::getName() const {
    return _name;
}

inline void Resource::setName(const std::string &name) {
    _name = name;
}
