#include <lug/Graphics/Builder/Mesh.hpp>

#include <lug/Graphics/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

Mesh::Mesh(Renderer& renderer) : _renderer(renderer) {}

void Mesh::PrimitiveSet::addAttributeBuffer(const void* data, uint32_t elementSize, uint32_t elementsCount, Render::Mesh::PrimitiveSet::Attribute::Type type) {
    Render::Mesh::PrimitiveSet::Attribute attribute;

    attribute.type = type;
    attribute.buffer.size = elementSize * elementsCount;
    attribute.buffer.data = new char[attribute.buffer.size];
    attribute.buffer.elementsCount = elementsCount;

    std::memcpy(attribute.buffer.data, static_cast<const char*>(data), attribute.buffer.size);

    _attributes.push_back(std::move(attribute));
}

Resource::SharedPtr<Render::Mesh> Mesh::build() {
    switch (_renderer.getType()) {
        case Renderer::Type::Vulkan:
            return lug::Graphics::Vulkan::Builder::Mesh::build(*this);
    }

    return nullptr;
}

} // Builder
} // Graphics
} // lug
