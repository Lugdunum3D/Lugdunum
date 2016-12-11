inline Handle open(const char* name) { // TODO: Handle errors
    Handle handle;

#if defined(LUG_SYSTEM_WINDOWS)
    handle = LoadLibraryA(name);
#else
    handle = dlopen(name, RTLD_LAZY | RTLD_LOCAL);
#endif

    return handle;
}

inline void close(Handle handle) {
#if defined(LUG_SYSTEM_WINDOWS)
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
}

template<typename Function>
inline Function sym(Handle handle, const char *name) { // TODO: Handle errors
#if defined(LUG_SYSTEM_WINDOWS)
    return reinterpret_cast<Function>(GetProcAddress(handle, name));
#else
    return reinterpret_cast<Function>(dlsym(handle, name));
#endif
}
