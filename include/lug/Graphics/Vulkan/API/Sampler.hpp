#pragma once

#include <lug/Graphics/Export.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

namespace Builder {
class Sampler;
} // Builder

class Device;

class LUG_GRAPHICS_API Sampler {
    friend class Builder::Sampler;

public:
    Sampler() = default;

    Sampler(const Sampler&) = delete;
    Sampler(Sampler&& sampler);

    Sampler& operator=(const Sampler&) = delete;
    Sampler& operator=(Sampler&& sampler);

    ~Sampler();

    explicit operator VkSampler() const {
        return _sampler;
    }

    void destroy();

private:
    explicit Sampler(VkSampler sampler, const Device* device);

private:
    VkSampler _sampler{VK_NULL_HANDLE};
    const Device* _device{nullptr};
};

} // API
} // Vulkan
} // Graphics
} // lug
