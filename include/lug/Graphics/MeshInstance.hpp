#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/MovableObject.hpp>
#include <lug/Graphics/Mesh.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API MeshInstance final : public lug::Graphics::MovableObject {
public:
    MeshInstance(const std::string& name, Mesh* mesh);

    MeshInstance(const MeshInstance&) = delete;
    MeshInstance(MeshInstance&&) = delete;

    MeshInstance& operator=(const MeshInstance&) = delete;
    MeshInstance& operator=(MeshInstance&&) = delete;

    ~MeshInstance();

    void needUpdate() override final;

    const Mesh* getMesh() const;

private:
    Mesh* _mesh{nullptr};
    // Mesh::Material* _material{nullptr};
};

#include <lug/Graphics/MeshInstance.inl>

} // Graphics
} // lug
