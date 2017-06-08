inline const std::string& Mesh::getName() const {
    return _name;
}

inline void Mesh::setName(const std::string &name) {
    _name = name;
}

inline const std::vector<Mesh::PrimitiveSet>& Mesh::getPrimitiveSets() const {
    return _primitiveSets;
}
