#pragma once

#include <unordered_map>
#include <vector>
#include <lug/Graphic/Export.hpp>
#include <lug/Graphic/Vulkan/Vulkan.hpp>

namespace lug {
namespace Graphic {

#define LUG_GRAPHIC_MODULES(macro) \
    macro(Core)

// TODO: Renderer's modules and not Graphic's (put the modules in the renderer)
struct Module {
    // TODO: Complete requirements
    struct Requirements {
        const std::vector<const char*> mandatoryInstanceExtensions;
        const std::vector<const char*> mandatoryInstanceLayers;

        const std::vector<const char*> optionnalInstanceExtensions;
        const std::vector<const char*> optionnalInstanceLayers;
    };

    enum class Type : uint8_t {
        #define LUG_DEFINE_GRAPHIC_MODULE_ENUM(name) name
        LUG_GRAPHIC_MODULES(LUG_DEFINE_GRAPHIC_MODULE_ENUM)
        #undef LUG_DEFINE_GRAPHIC_MODULE_ENUM
    };

    const char* const name;
    const Type type;
    const Requirements requirements;
};

const Module::Requirements CoreRequirements{
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

extern const std::unordered_map<Module::Type, Module> LUG_GRAPHIC_API modules;

} // Graphic
} // lug
