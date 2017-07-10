#pragma once

#include <lug/Graphics/Vulkan/API/DescriptorSet.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace Render {
namespace DescriptorSetPool {

template <size_t maxSets>
class DescriptorSetPool;

class LUG_GRAPHICS_API DescriptorSet {
    template <size_t maxSets>
    friend class DescriptorSetPool;

public:
    DescriptorSet() = default;
    DescriptorSet(API::DescriptorSet descriptorSet);

    DescriptorSet(const DescriptorSet&) = delete;
    DescriptorSet(DescriptorSet&&) = delete;

    DescriptorSet& operator=(const DescriptorSet&) = delete;
    DescriptorSet& operator=(DescriptorSet&&) = delete;

    ~DescriptorSet() = default;

    const API::DescriptorSet& getDescriptorSet() const;

    size_t getHash() const;
    void setHash(size_t hash);

private:
    API::DescriptorSet _descriptorSet;

    size_t _hash{0};
    uint32_t _referenceCount{0};
};

#include <lug/Graphics/Vulkan/Render/DescriptorSetPool/DescriptorSet.inl>

} // DescriptorSetPool
} // Render
} // Vulkan
} // Graphics
} // lug
