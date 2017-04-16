inline std::ostream& operator<<(std::ostream& ss, const Module& module) {
    ss << module.name;
    return ss;
}

inline std::ostream& operator<<(std::ostream& ss, const Module::Type& type) {
    ss << modules.at(type).name;
    return ss;
}
