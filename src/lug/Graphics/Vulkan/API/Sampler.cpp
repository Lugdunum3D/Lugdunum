#include <lug/Graphics/Vulkan/API/Sampler.hpp>

#include <lug/Graphics/Vulkan/API/Device.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

Sampler::Sampler(VkSampler sampler, const Device* device) : _sampler(sampler), _device(device) {}

Sampler::Sampler(Sampler&& sampler) {
    _sampler = sampler._sampler;
    _device = sampler._device;
    sampler._sampler = VK_NULL_HANDLE;
    sampler._device = nullptr;
}

Sampler& Sampler::operator=(Sampler&& sampler) {
    destroy();

    _sampler = sampler._sampler;
    _device = sampler._device;
    sampler._sampler = VK_NULL_HANDLE;
    sampler._device = nullptr;

    return *this;
}

Sampler::~Sampler() {
    destroy();
}

void Sampler::destroy() {
    if (_sampler != VK_NULL_HANDLE) {
        vkDestroySampler(static_cast<VkDevice>(*_device), _sampler, nullptr);
        _sampler = VK_NULL_HANDLE;
    }
}

} // API
} // Vulkan
} // Graphics
} // lug
