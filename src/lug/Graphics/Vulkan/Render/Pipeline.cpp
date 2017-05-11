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
    // TODO: Check if the Pipeline already exist in the Renderer

    std::unique_ptr<Resource> resource{new Pipeline(id)};
    Pipeline* pipeline = static_cast<Pipeline*>(resource.get());

    if (!pipeline->init()) {
        return nullptr;
    }

    Resource::SharedPtr<Pipeline> sharedPtrPipeline = renderer.getResourceManager()->add<Pipeline>(std::move(resource));

    // TODO: Add pipeline to Renderer

    return sharedPtrPipeline;
}

} // Render
} // Vulkan
} // Graphics
} // lug
