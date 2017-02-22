template <typename Function>
inline Function Instance::getProcAddr(const char* name) const {
    return reinterpret_cast<Function>(vkGetInstanceProcAddr(_instance, name));
}
