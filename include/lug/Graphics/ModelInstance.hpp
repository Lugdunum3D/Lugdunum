#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/MovableObject.hpp>
#include <lug/Graphics/MeshInstance.hpp>
#include <lug/Graphics/Model.hpp>

namespace lug {
namespace Graphics {

class LUG_GRAPHICS_API ModelInstance final : public lug::Graphics::MovableObject {
public:
    ModelInstance(const std::string& name, Model* model);

    ModelInstance(const ModelInstance&) = delete;
    ModelInstance(ModelInstance&&) = delete;

    ModelInstance& operator=(const ModelInstance&) = delete;
    ModelInstance& operator=(ModelInstance&&) = delete;

    ~ModelInstance() override final = default;

    void needUpdate() override final;

    const Model* getModel() const;
    Model* getModel();
    const std::vector<std::unique_ptr<MeshInstance> >& getMeshsInstances() const;

private:
    std::vector<std::unique_ptr<MeshInstance> > _meshsInstances;
    Model* _model{nullptr};
};

#include <lug/Graphics/ModelInstance.inl>

} // Graphics
} // lug
