template <typename T, class Arena, class ...Args>
inline T* new_one(size_t alignment, const char* file, size_t line, Arena& arena, Args&&... args) {
    void* ptr = arena.allocate(sizeof(T), alignment, 0, file, line);

    if (!ptr) {
        return nullptr;
    }

    return new (ptr) T{std::forward<Args>(args)...};
}

template <typename T, class Arena>
inline void delete_one(T* object, Arena& arena) {
    if (object) {
        object->~T();
    }

    arena.free(object);
}

template <typename T, class Arena, class ...Args, typename std::enable_if<!std::is_pod<T>::value, int>::type>
inline T* new_array(size_t alignment, size_t nb, const char* file, size_t line, Arena& arena, Args&&... args) {
    void* const ptr = arena.allocate(sizeof(T) * nb + sizeof(size_t), alignment, alignof(size_t), file, line);

    if (!ptr) {
        return nullptr;
    }

    // Store the size of the array
    size_t* size_ptr = static_cast<size_t*>(ptr);
    *size_ptr = nb;

    // Call the constructors
    T* const user_ptr = reinterpret_cast<T*>(size_ptr + 1);
    for (size_t i = 0; i < nb; ++i) {
        new (&user_ptr[i]) T{std::forward<Args>(args)...};
    }

    return user_ptr;
}

template <typename T, class Arena, typename std::enable_if<!std::is_pod<T>::value, int>::type>
inline void delete_array(T* ptr, Arena& arena) {
    if (!ptr) {
        return;
    }

    // Get back the size of the array
    size_t* size_ptr = reinterpret_cast<size_t*>(ptr);
    const size_t nb = size_ptr[-1];

    // Call the destructors in reverse order
    for (size_t i = nb; i > 0; --i) {
        ptr[i - 1].~T();
    }

    arena.free(&size_ptr[-1]);
}

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type>
inline T* new_array(size_t alignment, size_t nb, const char* file, size_t line, Arena& arena) {
    return new(arena.allocate(sizeof(T) * nb, alignment, 0, file, line)) T{};
}

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type>
inline void delete_array(T* ptr, Arena& arena) {
    arena.free(ptr);
}


// make_unique of single object
template <typename T, class Arena, typename ...Args>
inline typename priv::make_unique_if<T>::SingleObject make_unique(Arena& arena, Args&&... args) {
    return make_unique_align<T>(arena, alignof(T), std::forward<Args>(args)...);
}

template <typename T, class Arena, typename ...Args>
typename priv::make_unique_if<T>::SingleObject make_unique_align(Arena& arena, size_t alignment, Args&&... args) {
    auto deleter = [&arena](T* ptr) {
        LUG_DELETE(ptr, arena);
    };

    return typename priv::make_unique_if<T>::SingleObject(
        LUG_NEW_ALIGN(T, alignment, arena, std::forward<Args>(args)...),
        deleter
    );
}

// make_unique of dynamic array (args only for non POD types)
template <typename T, class Arena, typename ...Args, typename std::enable_if<!std::is_pod<T>::value, int>::type>
inline typename priv::make_unique_if<T>::UnknownBound make_unique(Arena& arena, size_t size, Args&&... args) {
    return make_unique_align<T>(arena, alignof(T), size, std::forward<Args>(args)...);
}

template <typename T, class Arena, typename ...Args, typename std::enable_if<!std::is_pod<T>::value, int>::type>
typename priv::make_unique_if<T>::UnknownBound make_unique_align(Arena& arena, size_t alignment, size_t size, Args&&... args) {
    using U = typename std::remove_all_extents<T>::type;

    auto deleter = [&arena](U* ptr) {
        LUG_DELETE_ARRAY(ptr, arena);
    };

    return typename priv::make_unique_if<T>::UnknownBound(
        LUG_NEW_ARRAY_ALIGN_SIZE(T, alignment, size, arena, std::forward<Args>(args)...),
        deleter
    );
}

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type>
inline typename priv::make_unique_if<T>::UnknownBound make_unique(Arena& arena, size_t size) {
    return make_unique_align<T>(arena, alignof(T), size);
}

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type>
typename priv::make_unique_if<T>::UnknownBound make_unique_align(Arena& arena, size_t alignment, size_t size) {
    using U = typename std::remove_all_extents<T>::type;

    auto deleter = [&arena](U* ptr) {
        LUG_DELETE_ARRAY(ptr, arena);
    };

    return typename priv::make_unique_if<T>::UnknownBound(
        LUG_NEW_ARRAY_ALIGN_SIZE(T, alignment, size, arena),
        deleter
    );
}

// make_unique of static array (args only for non POD types)
template <typename T, class Arena, typename ...Args, typename std::enable_if<!std::is_pod<T>::value, int>::type>
inline typename priv::make_unique_if<T>::KnownBound make_unique(Arena& arena, Args&&... args) {
    return make_unique_align<T>(arena, alignof(T), std::forward<Args>(args)...);
}

template <typename T, class Arena, typename ...Args, typename std::enable_if<!std::is_pod<T>::value, int>::type>
typename priv::make_unique_if<T>::KnownBound make_unique_align(Arena& arena, size_t alignment, Args&&... args) {
    using U = typename std::remove_all_extents<T>::type;

    auto deleter = [&arena](U* ptr) {
        LUG_DELETE_ARRAY(ptr, arena);
    };

    return typename priv::make_unique_if<T>::KnownBound(
        LUG_NEW_ARRAY_ALIGN(T, alignment, arena, std::forward<Args>(args)...),
        deleter
    );
}

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type>
typename priv::make_unique_if<T>::KnownBound make_unique(Arena& arena) {
    return make_unique_align<T>(arena, alignof(T));
}

template <typename T, class Arena, typename std::enable_if<std::is_pod<T>::value, int>::type>
typename priv::make_unique_if<T>::KnownBound make_unique_align(Arena& arena, size_t alignment) {
    using U = typename std::remove_all_extents<T>::type;

    auto deleter = [&arena](U* ptr) {
        LUG_DELETE_ARRAY(ptr, arena);
    };

    return typename priv::make_unique_if<T>::KnownBound(
        LUG_NEW_ARRAY_ALIGN(T, alignment, arena),
        deleter
    );
}
