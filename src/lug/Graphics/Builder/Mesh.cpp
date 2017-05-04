#include <algorithm>

#include <lug/Graphics/Builder/Mesh.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

Mesh::Mesh(Renderer& renderer) : _renderer(renderer) {}

void Mesh::PrimitiveSet::setMaterial(Resource::SharedPtr<Render::Material> material) {
    _material = material;
}

void Mesh::PrimitiveSet::setMode(Render::Mesh::PrimitiveSet::Mode mode) {
    _mode = mode;
}

void Mesh::PrimitiveSet::addAttributeBuffer(void* data, uint32_t size, Render::Mesh::PrimitiveSet::Attribute::Type type) {
    Render::Mesh::PrimitiveSet::Attribute attribute;

    attribute.type = type;
    attribute.buffer.size = size;
    attribute.buffer.data = new char[size];

    std::copy(static_cast<char*>(data), static_cast<char*>(data) + size, attribute.buffer.data);

    _attributes.push_back(std::move(attribute));
}

Render::Mesh::PrimitiveSet::Mode Mesh::PrimitiveSet::getMode() const {
    return _mode;
}

Resource::SharedPtr<Render::Material> Mesh::PrimitiveSet::getMaterial() const {
    return _material;
}

const std::vector<Render::Mesh::PrimitiveSet::Attribute>& Mesh::PrimitiveSet::getAttributes() const {
    return _attributes;
}

Mesh::PrimitiveSet* Mesh::addPrimitiveSet() {
    _primitiveSets.push_back(Mesh::PrimitiveSet());
    return &_primitiveSets.back();
}

void Mesh::setName(const std::string& name) {
    _name = name;
}


} // Builder
} // Graphics
} // lug
