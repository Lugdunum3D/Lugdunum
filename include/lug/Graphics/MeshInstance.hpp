#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/MovableObject.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API MeshInstance final : public lug::Graphics::MovableObject {
public:
    MeshInstance(const std::string& name);

    MeshInstance(const MeshInstance&) = delete;
    MeshInstance(MeshInstance&&) = delete;

    MeshInstance& operator=(const MeshInstance&) = delete;
    MeshInstance& operator=(MeshInstance&&) = delete;

    ~MeshInstance() = default;

private:
    Mesh::Material _material;
    // TODO: Transformations
};

} // Graphics
} // lug
