#pragma once

#include <map>
#include <vector>

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Render/Queue.hpp>
#include <lug/Graphics/Vulkan/Render/Material.hpp>
#include <lug/Graphics/Vulkan/Render/Mesh.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {

class LUG_GRAPHICS_API Queue final : public ::lug::Graphics::Render::Queue {
public:
    struct PrimitiveSetInstance {
        Scene::Node* node;
        const Render::Mesh::PrimitiveSet* primitiveSet;
        Render::Material* material;
    };

public:
    Queue() = default;

    Queue(const Queue&) = delete;
    Queue(Queue&&) = delete;

    Queue& operator=(const Queue&) = delete;
    Queue& operator=(Queue&&) = delete;

    ~Queue() = default;

    void addMeshInstance(Scene::Node& node) override final;
    void addLight(Scene::Node& node) override final;
    void clear() override final;

    const std::map<Render::Pipeline::Id, std::vector<PrimitiveSetInstance>> getPrimitiveSets() const;

    const std::vector<Scene::Node*> getLights() const;
    std::size_t getLightsCount() const;

private:
    // TODO: Use a custom frame allocator for the content of the queue
    // TODO: Also sort by material ?
    std::map<Render::Pipeline::Id, std::vector<PrimitiveSetInstance>> _primitiveSets;

    std::vector<Scene::Node*> _lights{50};
    std::size_t _lightsCount{0};
};

} // Render
} // Vulkan
} // Graphics
} // lug
