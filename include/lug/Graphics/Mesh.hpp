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
        Math::Vector<3, float> ambient;
        Math::Vector<3, float> diffuse;
        Math::Vector<3, float> specular;
    };

    struct Vertex{
        Math::Vector<3, float> pos;
        Math::Vector<3, float> normal;
        Math::Vector<2, float> uv;
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
