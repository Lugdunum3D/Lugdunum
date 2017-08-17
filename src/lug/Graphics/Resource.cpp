#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {

Resource::Resource(Resource::Type type, const std::string& name) : _name{name}, _handle{} {
    _handle.type = static_cast<uint8_t>(type);
    _handle.index = 0;
}

} // Graphics
} // lug
