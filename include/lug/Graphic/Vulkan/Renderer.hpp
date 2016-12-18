#pragma once

#include <unordered_map>
#include <lug/Graphic/Export.hpp>
#include <lug/Graphic/Module.hpp>
#include <lug/Graphic/Renderer.hpp>
#include <lug/Graphic/Vulkan/Instance.hpp>
#include <lug/Graphic/Vulkan/Loader.hpp>
#include <lug/Graphic/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphic {

class Graphic;

namespace Vulkan {

class LUG_GRAPHIC_API Renderer final : public ::lug::Graphic::Renderer {
public:
    struct Requirements {
        const std::vector<const char*> mandatoryInstanceExtensions;
        const std::vector<const char*> mandatoryInstanceLayers;

        const std::vector<const char*> optionnalInstanceExtensions;
        const std::vector<const char*> optionnalInstanceLayers;
    };

public:
    Renderer(Graphic&);

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    ~Renderer() = default;

    void init() override final;

private:
    Loader _loader; // Need to be at the beginning, we don't want to unload Vulkan functions too early
    Graphic& _graphic;

    Instance _instance;
    InstanceInfo _instanceInfo;

private:
    static const std::unordered_map<Module::Type, Requirements> modulesRequirements;
};

const Renderer::Requirements CoreRequirements{
    // mandatoryInstanceExtensions
    {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        LUG_VK_KHR_PLATFORM_SURFACE_EXTENSION_NAME
    },

    // mandatoryInstanceLayers
    {},

    // optionnalInstanceExtensions
    {},

    // optionnalInstanceLayers
    {}
};

} // Vulkan
} // Graphic
} // lug
