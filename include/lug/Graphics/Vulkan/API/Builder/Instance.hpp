#pragma once

#include <memory>
#include <string>
#include <vector>

#include <lug/Core/Version.hpp>
#include <lug/Graphics/Vulkan/API/Instance.hpp>

namespace lug {
namespace Graphics {
namespace Vulkan {
namespace API {
namespace Builder {

class Instance {
public:
    struct ApplicationInfo {
        std::string appName;
        Core::Version appVersion{0, 0, 0};

        std::string engineName;
        Core::Version engineVersion{0, 0, 0};

        Core::Version apiVersion{0, 0, 0};
    };

public:
    Instance() = default;

    Instance(const Instance&&) = delete;
    Instance(Instance&&) = delete;

    Instance& operator=(const Instance&&) = delete;
    Instance& operator=(Instance&&) = delete;

    ~Instance() = default;

    // Setters
    void setApplicationInfo(const std::string& name, const Core::Version& version = {0, 0, 0});
    void setEngineInfo(const std::string& name, const Core::Version& version = {0, 0, 0});
    void setRequiredVulkanVersion(const Core::Version& version = {0, 0, 0});

    void setLayers(const std::vector<const char*>& layers);
    void setExtensions(const std::vector<const char*>& extensions);

    // Build methods
    bool build(API::Instance& instance, VkResult* returnResult = nullptr);
    std::unique_ptr<API::Instance> build(VkResult* returnResult = nullptr);

private:
    ApplicationInfo _appInfo;

    std::vector<const char*> _layers;
    std::vector<const char*> _extensions;
};

#include <lug/Graphics/Vulkan/API/Builder/Instance.inl>

} // Builder
} // API
} // Vulkan
} // Graphics
} // lug
