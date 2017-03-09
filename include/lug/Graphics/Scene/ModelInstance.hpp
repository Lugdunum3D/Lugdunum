#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Model.hpp>
#include <lug/Graphics/Scene/MovableObject.hpp>
#include <lug/Graphics/Scene/MeshInstance.hpp>

namespace lug {
namespace Graphics {
namespace Scene {

class LUG_GRAPHICS_API ModelInstance final : public ::lug::Graphics::Scene::MovableObject {
public:
    ModelInstance(const std::string& name, Render::Model* model);

    ModelInstance(const ModelInstance&) = delete;
    ModelInstance(ModelInstance&&) = delete;

    ModelInstance& operator=(const ModelInstance&) = delete;
    ModelInstance& operator=(ModelInstance&&) = delete;

    ~ModelInstance() override final = default;

    void needUpdate() override final;

    const Render::Model* getModel() const;
    Render::Model* getModel();
    const std::vector<std::unique_ptr<MeshInstance>>& getMeshsInstances() const;

private:
    std::vector<std::unique_ptr<MeshInstance>> _meshsInstances;
    Render::Model* _model{nullptr};
};

#include <lug/Graphics/Scene/ModelInstance.inl>

} // Scene
} // Graphics
} // lug
