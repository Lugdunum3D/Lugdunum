template <typename Function>
inline Function Device::getProcAddr(const char* name) const {
    return reinterpret_cast<Function>(vkGetDeviceProcAddr(_device, name));
}
