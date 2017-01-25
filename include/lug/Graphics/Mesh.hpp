#pragma once

#include <cstdint>
#include <vector>
#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API Mesh {
public:
    // TODO: Replace float arrays with math vector
    struct Material {
        float ambient[3];
        float diffuse[3];
        float specular[3];
    };

    // TODO: Replace float arrays with math vector
    struct Vertex{
        float pos[3];
        float normal[3];
        float uv[2];
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
