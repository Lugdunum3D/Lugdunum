#pragma once

#include <map>

#include <lug/Graphics/Vulkan/API/Builder/DescriptorPool.hpp>
#include <lug/Graphics/Vulkan/API/Builder/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/API/DescriptorPool.hpp>
#include <lug/Graphics/Vulkan/API/DescriptorSetLayout.hpp>
#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/DescriptorSet.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {

class Renderer;

namespace Render {
namespace DescriptorSetPool {

// TODO: Use another method than a global
extern API::DescriptorPool descriptorPool;

template <size_t maxSets>
class LUG_GRAPHICS_API DescriptorSetPool {
public:
    DescriptorSetPool(Renderer& renderer);

    DescriptorSetPool(const DescriptorSetPool&) = delete;
    DescriptorSetPool(DescriptorSetPool&&) = delete;

    DescriptorSetPool& operator=(const DescriptorSetPool&) = delete;
    DescriptorSetPool& operator=(DescriptorSetPool&&) = delete;

    ~DescriptorSetPool();

    bool init();

    std::tuple<bool, const DescriptorSet*> allocate(size_t hash, const API::DescriptorSetLayout& descriptorSetLayout);
    void free(const DescriptorSet* descriptorSet);

private:
    DescriptorSet* allocateNewDescriptorSet(const API::DescriptorSetLayout& descriptorSetLayout);

protected:
    Renderer& _renderer;

    size_t _descriptorSetsCount{0};
    std::array<DescriptorSet, maxSets> _descriptorSets;

    size_t _freeDescriptorSetsCount{0};
    std::array<DescriptorSet*, maxSets> _freeDescriptorSets;

    std::map<size_t, DescriptorSet*> _descriptorSetsInUse;
};

#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/DescriptorSetPool.inl>

} // DescriptorSetPool
} // Render
} // Vulkan
} // Graphics
} // lug
