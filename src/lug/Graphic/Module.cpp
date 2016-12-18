#include <lug/Graphic/Module.hpp>

namespace lug {
namespace Graphic {

const std::unordered_map<Module::Type, Module> modules = {
    #define LUG_INIT_GRAPHIC_MODULES(name)  \
    {                                       \
        Module::Type::name, {               \
            #name,                          \
            Module::Type::name              \
        }                                   \
    },

    LUG_GRAPHIC_MODULES(LUG_INIT_GRAPHIC_MODULES)

    #undef LUG_INIT_GRAPHIC_MODULES
};

} // Graphic
} // lug
