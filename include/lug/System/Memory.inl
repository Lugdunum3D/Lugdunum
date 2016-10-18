template <typename T, class Arena>
inline void delete_one(T* object, Arena& arena) {
    object->~T();
    arena.free(object);
}

template <typename T, class Arena, typename std::enable_if<!std::is_pod<T>::value, int>::type = 0>
inline T* new_array(size_t alignment, Arena& arena, size_t nb, const char* file, size_t line) {
    void* const ptr = arena.allocate(sizeof(T) * nb + sizeof(size_t), alignment, alignof(size_t), file, line);

    if (!ptr) {
        return nullptr;
    }

    // Store the size of the array
    size_t* size_ptr = static_cast<size_t*>(ptr);
    *size_ptr = nb;

    // Call the constructors
    T* const user_ptr = reinterpret_cast<T*>(size_ptr + 1);
    for (size_t i = 0; i < nb; ++i)
        new (&user_ptr[i]) T;

    return user_ptr;
}

template <typename T, class Arena, typename std::enable_if<!std::is_pod<T>::value, int>::type = 0>
inline void delete_array(T* ptr, Arena& arena) {
    if (!ptr) {
        return;
    }

    // Get back the size of the array
    size_t* size_ptr = reinterpret_cast<size_t*>(ptr);
    const size_t nb = size_ptr[-1];

    // Call the destructors in reverse order
    for (size_t i = nb; i > 0; --i)
        ptr[i - 1].~T();

    arena.free(&size_ptr[-1]);
}

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
inline T* new_array(size_t alignment, Arena& arena, size_t nb, const char* file, size_t line) {
    return static_cast<T*>(arena.allocate(sizeof(T) * nb, alignment, 0, file, line));
}

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
inline void delete_array(T* ptr, Arena& arena) {
    arena.free(ptr);
}
