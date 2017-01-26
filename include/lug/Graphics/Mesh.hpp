#pragma once

#include <cstdint>
#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Math/Vector.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API Mesh {
public:
    struct Material {
        Math::Vec3f ambient;
        Math::Vec3f diffuse;
        Math::Vec3f specular;
    };

    struct Vertex{
        Math::Vec3f pos;
        Math::Vec3f normal;
        Math::Vec2f uv;
    };

public:
    Mesh(const std::string& name);

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) = delete;

    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&&) = delete;

    ~Mesh() = default;

private:
    std::vector<Vertex> _vertices;
    std::vector<uint32_t> _indices;
    Material _baseMaterial;
};

} // Graphics
} // lug
