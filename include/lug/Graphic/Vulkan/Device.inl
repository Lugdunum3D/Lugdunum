template <typename Function>
inline Function Device::getProcAddr(const char* name) {
    return reinterpret_cast<Function>(vkGetDeviceProcAddr(_device, name));
}
