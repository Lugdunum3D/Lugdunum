#include <lug/Graphics/Module.hpp>

namespace lug {
namespace Graphics {

const std::unordered_map<Module::Type, Module> modules = {
    #define LUG_INIT_GRAPHICS_MODULES(name)  \
    {                                       \
        Module::Type::name, {               \
            #name,                          \
            Module::Type::name              \
        }                                   \
    },

    LUG_GRAPHICS_MODULES(LUG_INIT_GRAPHICS_MODULES)

    #undef LUG_INIT_GRAPHICS_MODULES
};

} // Graphics
} // lug
