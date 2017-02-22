#if defined(LUG_SYSTEM_WINDOWS)

inline const char* getLastErrorWindows() {
    static const DWORD size = 200 + 1;
    static WCHAR bufferWide[size];
    static char buffer[size];

    auto lastError = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        lastError,
        0,
        reinterpret_cast<LPSTR>(bufferWide),
        size,
        nullptr
    );

#pragma warning (push)
#pragma warning (disable : 4996)
    std::wcstombs(buffer, bufferWide, size);
#pragma warning (pop)

    return buffer;
}

#endif

inline Handle open(const char* name) {
    Handle handle = nullptr;

#if defined(LUG_SYSTEM_WINDOWS)
    handle = LoadLibraryA(name);
#else
    handle = dlopen(name, RTLD_LAZY | RTLD_LOCAL);
#endif

    if (!handle) {
#if defined(LUG_SYSTEM_WINDOWS)
        LUG_LOG.warn("Library: Can't load the library: {}", getLastErrorWindows());
#else
        LUG_LOG.warn("Library: Can't load the library: {}", dlerror());
#endif
    }

    return handle;
}

inline void close(Handle handle) {
    if (!handle) {
        return;
    }

#if defined(LUG_SYSTEM_WINDOWS)
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
}

template<typename Function>
inline Function sym(Handle handle, const char *name) {
    void* sym = nullptr;

#if defined(LUG_SYSTEM_WINDOWS)
    sym = GetProcAddress(handle, name);
#else
    sym = dlsym(handle, name);
#endif

    if (!sym) {
#if defined(LUG_SYSTEM_WINDOWS)
        LUG_LOG.warn("Library: Can't load the symbol {}: {}", name, getLastErrorWindows());
#else
        LUG_LOG.warn("Library: Can't load the symbol {}: {}", name, dlerror());
#endif
    }

    return reinterpret_cast<Function>(sym);
}
