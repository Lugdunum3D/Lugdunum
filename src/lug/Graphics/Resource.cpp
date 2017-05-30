#include <lug/Graphics/Resource.hpp>

namespace lug {
namespace Graphics {

Resource::Resource(Resource::Type type) {
    _handle.type = static_cast<uint8_t>(type);
    _handle.index = 0;
}

} // Graphics
} // lug
