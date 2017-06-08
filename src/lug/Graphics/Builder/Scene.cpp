#include <lug/Graphics/Builder/Scene.hpp>

#include <lug/Graphics/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Builder {

Scene::Scene(Renderer& renderer) : _renderer(renderer) {}

Resource::SharedPtr<lug::Graphics::Scene::Scene> Scene::build() {
    // Constructor of Material is private, we can't use std::make_unique
    std::unique_ptr<Resource> resource{new lug::Graphics::Scene::Scene(_name)};

    return _renderer.getResourceManager()->add<::lug::Graphics::Scene::Scene>(std::move(resource));
}

} // Builder
} // Graphics
} // lug
