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
    };

public:
    Mesh(const std::string& name);

    Mesh(const Mesh&) = delete;
    Mesh(Mesh&&) = delete;

    Mesh& operator=(const Mesh&) = delete;
    Mesh& operator=(Mesh&&) = delete;

    const std::string& getName() const;
    void setName(const std::string &name);

    virtual ~Mesh() = default;

    virtual bool load() = 0;

public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

protected:
    Material _baseMaterial;

    bool _loaded{false};
    std::string _name;
};

#include <lug/Graphics/Mesh.inl>

} // Graphics
} // lug
