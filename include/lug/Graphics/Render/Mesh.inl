inline const std::string& Mesh::getName() const {
    return _name;
}

inline void Mesh::setName(const std::string &name) {
    _name = name;
}

inline bool Mesh::isModelMesh() const {
    return false;
}
