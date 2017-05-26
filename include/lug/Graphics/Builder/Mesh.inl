inline void Mesh::PrimitiveSet::setMaterial(Resource::SharedPtr<Render::Material> material) {
    _material = material;
}

inline void Mesh::PrimitiveSet::setMode(Render::Mesh::PrimitiveSet::Mode mode) {
    _mode = mode;
}

inline void Mesh::PrimitiveSet::addAttributeBuffer(void* data, uint32_t size, Render::Mesh::PrimitiveSet::Attribute::Type type) {
    Render::Mesh::PrimitiveSet::Attribute attribute;

    attribute.type = type;
    attribute.buffer.size = size;
    attribute.buffer.data = new char[size];

    std::memcpy(attribute.buffer.data, static_cast<char*>(data), size);

    _attributes.push_back(std::move(attribute));
}

inline Render::Mesh::PrimitiveSet::Mode Mesh::PrimitiveSet::getMode() const {
    return _mode;
}

inline Resource::SharedPtr<Render::Material> Mesh::PrimitiveSet::getMaterial() const {
    return _material;
}

inline const std::vector<Render::Mesh::PrimitiveSet::Attribute>& Mesh::PrimitiveSet::getAttributes() const {
    return _attributes;
}

inline Mesh::PrimitiveSet* Mesh::addPrimitiveSet() {
    _primitiveSets.push_back(Mesh::PrimitiveSet());
    return &_primitiveSets.back();
}

inline void Mesh::setName(const std::string& name) {
    _name = name;
}
