#pragma once

#include <memory>
#include <string>
#include <vector>
#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Mesh.hpp>

namespace lug {
namespace Graphics {
namespace Render {

class LUG_GRAPHICS_API Model {
public:
    class Mesh final : public ::lug::Graphics::Render::Mesh {
    public:
        Mesh(const std::string& name, uint32_t verticesOffset, uint32_t indicesOffset);

        Mesh(const Mesh&) = delete;
        Mesh(Mesh&&) = delete;

        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&) = delete;

        ~Mesh() override final = default;

        bool load() override final;

        bool isModelMesh() const override final;

    public:
        uint32_t verticesOffset;
        uint32_t indicesOffset;
    };

public:
    Model(const std::string& name);

    Model(const Model&) = delete;
    Model(Model&&) = delete;

    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) = delete;

    virtual ~Model() = default;

    virtual bool load() = 0;

    void addMesh(std::unique_ptr<Mesh> mesh);

    uint32_t getIndicesSize() const;
    uint32_t getVerticesSize() const;
    const std::vector<std::unique_ptr<Mesh>>& getMeshs() const;

protected:
    bool _loaded{false};
    std::vector<std::unique_ptr<Mesh>> _meshs;

private:
    std::string _name;
};

} // Render
} // Graphics
} // lug
