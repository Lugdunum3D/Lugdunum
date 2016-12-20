template <typename Function>
inline Function Instance::getProcAddr(const char* name) {
    return reinterpret_cast<Function>(vkGetInstanceProcAddr(_instance, name));
}
