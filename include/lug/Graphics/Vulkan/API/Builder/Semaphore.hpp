#pragma once

#include <memory>

#include <lug/Graphics/Vulkan/API/Semaphore.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {

class Device;

namespace Builder {

class Semaphore {
public:
    Semaphore(const API::Device& device);

    Semaphore(const Semaphore&&) = delete;
    Semaphore(Semaphore&&) = delete;

    Semaphore& operator=(const Semaphore&&) = delete;
    Semaphore& operator=(Semaphore&&) = delete;

    ~Semaphore() = default;

    // Build methods
    bool build(API::Semaphore& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::Semaphore> build(VkResult* returnResult = nullptr);

private:
    const API::Device& _device;
};

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
