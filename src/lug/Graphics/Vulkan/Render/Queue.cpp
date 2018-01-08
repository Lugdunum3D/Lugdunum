#include <lug/Graphics/Vulkan/Render/Queue.hpp>

#include <lug/Graphics/Vulkan/Render/Material.hpp>
#include <lug/Graphics/Scene/Node.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

void Queue::addMeshInstance(Scene::Node& node, const lug::Graphics::Renderer& renderer) {
    auto meshInstance = node.getMeshInstance();

    uint32_t i = 0;
    for (auto& primitiveSet : meshInstance->mesh->getPrimitiveSets()) {
        Resource::SharedPtr<Render::Material> material = Resource::SharedPtr<Render::Material>::cast(meshInstance->materials[i] ? meshInstance->materials[i] : primitiveSet.material);

        if (!material) {
            continue;
        }

        // Determine the pipeline id
        Pipeline::Id pipelineId = 0;
        {
            Render::Mesh::PrimitiveSetData* primitiveSetData = static_cast<Render::Mesh::PrimitiveSetData*>(primitiveSet._data);
            Pipeline::Id::Model::PrimitivePart pipelineIdPrimitivePart = primitiveSetData->pipelineIdPrimitivePart;
            Pipeline::Id::Model::MaterialPart pipelineIdMaterialPart = material->getPipelineId();

            Pipeline::Id::Model::ExtraPart pipelineIdExtraPart;
            pipelineIdExtraPart.displayMode = static_cast<uint32_t>(renderer.getDisplayMode());
            pipelineIdExtraPart.antialiasing = static_cast<uint32_t>(renderer.getAntialiasing());
            pipelineIdExtraPart.irradianceMapInfo = static_cast<uint32_t>(material->getIrradianceMap() && material->getIrradianceMap()->getEnvironnementTexture() ? 1 : 0);
            pipelineIdExtraPart.prefilteredMapInfo = static_cast<uint32_t>(material->getPrefilteredMap() && material->getPrefilteredMap()->getEnvironnementTexture() ? 1 : 0);

            pipelineId = Pipeline::Id::createModel(pipelineIdPrimitivePart, pipelineIdMaterialPart, pipelineIdExtraPart);
        }

        // Add in the list
        _primitiveSets[pipelineId].push_back(Queue::PrimitiveSetInstance{
            /* node */ &node,
            /* primitiveSet */ &primitiveSet,
            /* material */ material.get()
        });

        ++i;
    }
}

void Queue::addLight(Scene::Node& node) {
    _lights[_lightsCount] = &node;
    ++_lightsCount;
}

void Queue::addSkyBox(Resource::SharedPtr<::lug::Graphics::Render::SkyBox> skyBox) {
    _skyBox = Resource::SharedPtr<Render::SkyBox>::cast(skyBox);
}

void Queue::clear() {
    _primitiveSets.clear();
    _lightsCount = 0;
}

const std::map<Render::Pipeline::Id, std::vector<Queue::PrimitiveSetInstance>> Queue::getPrimitiveSets() const {
    return _primitiveSets;
}

const std::vector<Scene::Node*> Queue::getLights() const {
    return _lights;
}

std::size_t Queue::getLightsCount() const {
    return _lightsCount;
}

const Resource::SharedPtr<Render::SkyBox> Queue::getSkyBox() const {
    return _skyBox;
}

} // Render
} // Vulkan
} // Graphics
} // lug
