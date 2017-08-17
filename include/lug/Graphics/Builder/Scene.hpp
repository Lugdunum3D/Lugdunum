#pragma once

#include <vector>
#include <string>

#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Scene/Scene.hpp>

namespace lug {
namespace Graphics {

class Renderer;

namespace Builder {

class LUG_GRAPHICS_API Scene {

public:
    explicit Scene(Renderer& renderer);

    Scene(const Scene&) = delete;
    Scene(Scene&&) = delete;

    Scene& operator=(const Scene&) = delete;
    Scene& operator=(Scene&&) = delete;

    ~Scene() = default;

    /**
     * @brief      Sets the name.
     * @param[in]  name  The name of the scene.
     */
    void setName(const std::string& name);

    Resource::SharedPtr<lug::Graphics::Scene::Scene> build();

protected:
    Renderer& _renderer;

    std::string _name{};
};

#include <lug/Graphics/Builder/Scene.inl>

} // Builder
} // Graphics
} // lug
