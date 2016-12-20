#pragma once

#include <ostream>
#include <unordered_map>
#include <vector>
#include <lug/Graphics/Export.hpp>

namespace lug {
namespace Graphics {

#define LUG_GRAPHICS_MODULES(macro) \
    macro(Core)

struct Module {
    enum class Type : uint8_t {
        #define LUG_DEFINE_GRAPHICS_MODULE_ENUM(name) name
        LUG_GRAPHICS_MODULES(LUG_DEFINE_GRAPHICS_MODULE_ENUM)
        #undef LUG_DEFINE_GRAPHICS_MODULE_ENUM
    };

    const char* const name;
    const Type type;
};

extern const std::unordered_map<Module::Type, Module> LUG_GRAPHICS_API modules;

std::ostream& operator<<(std::ostream& ss, const Module& module);
std::ostream& operator<<(std::ostream& ss, const Module::Type& type);

#include <lug/Graphics/Module.inl>

} // Graphics
} // lug
