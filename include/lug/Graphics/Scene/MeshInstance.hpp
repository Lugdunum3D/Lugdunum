#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Scene/MovableObject.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

class ModelInstance;

class LUG_GRAPHICS_API MeshInstance final : public ::lug::Graphics::Scene::MovableObject {
public:
    MeshInstance(const std::string& name, Render::Mesh* mesh, ModelInstance* modelInstance = nullptr);

    MeshInstance(const MeshInstance&) = delete;
    MeshInstance(MeshInstance&&) = delete;

    MeshInstance& operator=(const MeshInstance&) = delete;
    MeshInstance& operator=(MeshInstance&&) = delete;

    ~MeshInstance();

    void needUpdate() override final;

    const Render::Mesh* getMesh() const;
    Render::Mesh* getMesh();

    const ModelInstance* getModelInstance() const;
    ModelInstance* getModelInstance();

private:
    Render::Mesh* _mesh{nullptr};
    ModelInstance* _modelInstance;
    // Mesh::Material* _material{nullptr};
};

#include <lug/Graphics/Scene/MeshInstance.inl>

} // Scene
} // Graphics
} // lug
