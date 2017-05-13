#include <lug/Graphics/Vulkan/Render/Pipeline.hpp>

#include <memory>

#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

Pipeline::Pipeline(Id id) : Resource(Resource::Type::Pipeline), _id(id) {}

bool Pipeline::init() {
    // TODO: Initialize pipeline
    return false;
}

Resource::SharedPtr<Pipeline> Pipeline::create(Renderer& renderer, Id id) {
    if (renderer.containsPipeline(id)) {
        return renderer.getPipeline(id);
    }

    std::unique_ptr<Resource> resource{new Pipeline(id)};
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
