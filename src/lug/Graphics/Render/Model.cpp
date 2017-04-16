#include <lug/Graphics/Render/Model.hpp>

namespace lug {
namespace Graphics {
namespace Render {

Model::Mesh::Mesh(const std::string& name, uint32_t verticesOffset, uint32_t indicesOffset) :
    ::lug::Graphics::Render::Mesh(name), verticesOffset(verticesOffset), indicesOffset(indicesOffset) {}

bool Model::Mesh::load() {
    return true;
}

bool Model::Mesh::isModelMesh() const {
    return true;
}

Model::Model(const std::string& name) : _name(name) {}

void Model::addMesh(std::unique_ptr<Mesh> mesh) {
    _meshs.push_back(std::move(mesh));
}

uint32_t Model::getIndicesSize() const {
    uint32_t size = 0;

    for (const auto& mesh : _meshs) {
        size += static_cast<uint32_t>(mesh->indices.size());
    }

    return size;
}

uint32_t Model::getVerticesSize() const {
    uint32_t size = 0;

    for (const auto& mesh : _meshs) {
        size += static_cast<uint32_t>(mesh->vertices.size());
    }

    return size;
}

const std::vector<std::unique_ptr<Model::Mesh>>& Model::getMeshs() const {
    return _meshs;
}

}
} // Graphics
} // lug
