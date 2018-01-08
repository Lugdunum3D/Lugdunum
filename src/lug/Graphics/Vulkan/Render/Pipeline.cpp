#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>

#include <memory>

#include <lug/Graphics/Render/Mesh.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/GraphicsPipeline.hpp>
#include <lug/Graphics/Vulkan/API/Builder/PipelineLayout.hpp>
#include <lug/Graphics/Vulkan/API/Builder/RenderPass.hpp>
#include <lug/Graphics/Vulkan/API/Builder/ShaderModule.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/System/Exception.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Pipeline::Pipeline(Renderer& renderer, Id id) : Resource(Resource::Type::Pipeline, "Pipeline"), _renderer(renderer), _id(id) {}

bool Pipeline::init() {
    Pipeline::Type type = _id.getType();

    switch(type) {
        case Pipeline::Type::Model:
            return initModel();
        case Pipeline::Type::Skybox:
            return initSkybox();
        case Pipeline::Type::IrradianceMap:
            return initIrradianceMap();
        case Pipeline::Type::PrefilteredMap:
            return initPrefilteredMap();
        case Pipeline::Type::BrdfLut:
            return initBrdfLut();
        default:
            LUG_LOG.error("Vulkan::Render::Pipeline::init: Unknown type {}", static_cast<uint32_t>(_id.getType()));
            return false;
    }
}

Resource::SharedPtr<Pipeline> Pipeline::create(Renderer& renderer, Id id) {
    if (renderer.containsPipeline(id)) {
        return renderer.getPipeline(id);
    }

    std::unique_ptr<Resource> resource{new Pipeline(renderer, id)};
    Pipeline* pipeline = static_cast<Pipeline*>(resource.get());

    if (!pipeline->init()) {
        return nullptr;
    }

    Resource::SharedPtr<Pipeline> sharedPtrPipeline = renderer.getResourceManager()->add<Pipeline>(std::move(resource));
    renderer.addPipeline(sharedPtrPipeline);

    return sharedPtrPipeline;
}

} // Render
} // Vulkan
} // Graphics
} // lug
