inline uint32_t Application::Version::createVersion() const {
    return (major << 22) | (minor << 12) | patch;
}
