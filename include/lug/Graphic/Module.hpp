#pragma once

#include <unordered_map>
#include <vector>
#include <lug/Graphic/Export.hpp>

namespace lug {
namespace Graphic {

#define LUG_GRAPHIC_MODULES(macro) \
    macro(Core)

struct Module {
    enum class Type : uint8_t {
        #define LUG_DEFINE_GRAPHIC_MODULE_ENUM(name) name
        LUG_GRAPHIC_MODULES(LUG_DEFINE_GRAPHIC_MODULE_ENUM)
        #undef LUG_DEFINE_GRAPHIC_MODULE_ENUM
    };

    const char* const name;
    const Type type;
};

extern const std::unordered_map<Module::Type, Module> LUG_GRAPHIC_API modules;

} // Graphic
} // lug
